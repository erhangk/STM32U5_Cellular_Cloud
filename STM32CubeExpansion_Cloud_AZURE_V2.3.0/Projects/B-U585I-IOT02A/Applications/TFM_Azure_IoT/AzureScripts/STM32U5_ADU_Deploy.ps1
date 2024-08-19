<#
******************************************************************************
* @file    STM32U5_ADU_Deploy.ps1
* @author  MCD Application Team
* @brief   Deploy a Azure Device Update for the STM32U5 non-secure app
******************************************************************************
 * Copyright (c) 2023 STMicroelectronics.

 * All rights reserved.

 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
******************************************************************************
#>
$config = Get-Content .\Config.json -Raw | ConvertFrom-Json
$version = Get-Content -Path ..\TFM_Appli\NonSecure\VERSION -TotalCount 1

$iotHub = $config.Entered.IotHub
$deviceID = $config.Generated.RegistrationID

if ($config.ADU.Group -eq '')
{
    $aduGroup = $deviceID
}
else 
{
    $aduGroup = $config.ADU.Group
}
if ($config.ADU.Container -eq '')
{
    $containerName = $deviceID.ToLower()
}
else
{
    $containerName = $config.ADU.Container
}

$tag = '{"ADUGroup":"'+$aduGroup+'"}'


$storageAccount = $config.ADU.StorageAccount
$resourceGroup = $config.Entered.ResourceGroup
$aduAccount = $config.ADU.Account
$aduInstance = $config.ADU.Instance

$manifest = 'STMicroelectronics.B-U585I-IOT02A.'+$version+'.importmanifest.json'
$manifestPath = '.\STMicroelectronics.B-U585I-IOT02A.'+$version+'\'+$manifest


# Must be updated with real realease
$nsImage = 'tfm_ns_app_enc_sign.bin'
$nsBuildPath = '..\TFM_Appli\Binary\'+$nsImage
$sImage = 'tfm_s_app_enc_sign.bin'
$sBuildPath = '..\TFM_Appli\Binary\'+$sImage


function Copy_Secure_Binary()
{
    Write-Host 'Copying secure firmware image to working direcory'
    if (Test-Path -Path $sBuildPath) {
        Copy-Item $sBuildPath -Destination . -force
        Write-Host 'Secure binary copied' -ForegroundColor Green
        Write-Host ''
    }
    else 
    {  
        Write-Host 'ERR:   Secure firmware does not exist' -ForegroundColor Red
        Exit 1
    }

    
}
function Copy_NonSecure_Binary()
{
    Write-Host 'Copying non-secure firmware image to working direcory'
    if (Test-Path -Path $nsBuildPath) {
        Copy-Item $nsBuildPath -Destination . -force
        Write-Host 'Non-Secure binary copied' -ForegroundColor Green
        Write-Host ''
    }
    else 
    {  
        Write-Host 'ERR:   Non-Secure firmware does not exist' -ForegroundColor Red
        Exit 1
    }
}

function Create_Update_Package()
{
    Write-Host 'INFO:  Building update package'
    $packRet = & python .\scripts\STM32U5_ADU_CreateUpdatePackage.py -ns="$nsImage" -v="$version"
    
    if ($packRet -contains 'ERROR: Update package directory already exists.')
    {
        Write-Host 'WARN:  Update package already exists' -ForegroundColor Yellow
    } 
    elseif ((Test-Path -Path $manifestPath) -and ($packRet -contains 'Generating update package:'))
    {
        Write-Host 'Update package created' -ForegroundColor Green
    }
    else
    {
        Write-Host 'ERR:   Update package creation failed' -ForegroundColor Red
        $packRet
        Exit 1
    }
    Write-Host ''
}

function Add_Tag($config) 
{
    Write-Host 'INFO:  Adding ADUGroup to device'
    $tag = $tag.replace("`"", "\`"")

    $twin = & az iot hub device-twin update -n $iotHub -d $deviceID --tags $tag | Out-String | ConvertFrom-Json

    if ($twin.tags.ADUGroup -eq $aduGroup)
    {
        Write-Host 'ADUGroup assigned'-ForegroundColor Green 
        Write-Host
    } 
    else 
    {
        Write-Host 'ERR:   ADUGroup not assigned' -ForegroundColor Red
        $twin
        Exit 1
    }
}

function Create_Container()
{
    Write-Host 'INFO:  Creating storage container'

    $created = & az storage container create --name $containerName --account-name $storageAccount --only-show-errors | Out-String | ConvertFrom-Json

    if($created.created)
    {
        Write-Host 'Storage container created '-ForegroundColor Green 
        Write-Host ''
    } 
    else 
    {
        Write-Host 'WARN:  Failed to create storage container. Storage container may already exist' -ForegroundColor Yellow
        Write-Host ''
    }
}


function Stage_Update()
{
    Write-Host 'INFO:  Staging ADU'
    $stage = & az iot du update stage --resource-group $resourceGroup --account $aduAccount --instance $aduInstance --manifest-path $manifestPath --storage-account $storageAccount --storage-container $containerName --only-show-errors | Out-String | ConvertFrom-Json
    
    if ($stage.importCommand)
    {
        Write-Host 'ADU staged' -ForegroundColor Green
        Write-Host ''
    }
    else
    {
        Write-host 'ERR:   ADU not staged' -ForegroundColor Red
        $stage
        Exit 1
    }
    
    Write-Host "INFO:  Importing ADU"
    
    $importCommand = $stage.importCommand+' --only-show-errors'
    $import = Invoke-Expression $importCommand # & az iot du update import -n $aduAccount -i $aduAccount -g $resourceGroup --url cache:// --only-show-errors

    if ($null -eq $import)
    {
        Write-Host 'ADU imported' -ForegroundColor Green
        Write-Host ''
    }
    else 
    {
        Write-host 'ERR:   ADU not imported' -ForegroundColor Red
        $import
        Exit 1
    }
}


function Create_Deployment()
{
    Write-Host 'INFO:  Deploying ADU'

    $curManifest = Get-Content $manifestPath -Raw | ConvertFrom-Json

    $updateName = $curManifest.updateId.name 
    $updateProvider = $curManifest.updateId.provider

    $time = Get-Date -Format "yyyyMMdd-hhmmss"
    $id = $aduGroup.toLower()+'-'+$time
    $deploy = & az iot du device deployment create --account $aduAccount --deployment-id $id --group-id $aduGroup --instance $aduAccount --update-name $updateName --update-provider $updateProvider --update-version $version --only-show-errors | Out-String | ConvertFrom-Json

    if ($deploy.startDateTime)
    {
        Write-Host 'ADU deployed' -ForegroundColor Green
        Write-Host 'Deployment Details:'
        $deploy
    }
    else 
    {
        Write-host 'ERR:   ADU not deployed' -ForegroundColor Red
        $deploy | ConvertTo-Json
        Exit 1
    }
}


#####################################################################################################################
# Main
#####################################################################################################################

Copy_Secure_Binary

Copy_NonSecure_Binary

Create_Update_Package

Add_Tag

Create_Container

Stage_Update

Create_Deployment
