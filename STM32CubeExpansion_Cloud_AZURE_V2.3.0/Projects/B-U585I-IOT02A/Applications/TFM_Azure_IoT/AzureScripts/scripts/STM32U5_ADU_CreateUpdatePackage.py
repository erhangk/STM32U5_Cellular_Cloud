#!/bin/env python
###############################################################################
# 
# Copyright (c) 2021 STMicroelectronics.
# All rights reserved.
# 
# This software is licensed under terms that can be found in the ST_LICENSE file
# in the root directory of this software component.
# If no ST_LICENSE file comes with this software, it is provided AS-IS.
# 
###############################################################################

import sys
import os
import json
import argparse
import re
from pathlib import Path
from datetime import datetime
import hashlib
import base64
import shutil
from typing import List,Dict
import struct


class UpdateId:
    provider:str= ''
    name:str= ''
    version:str= ''
    def __init__(self, provider:str='', name:str='', version:str='') -> None:
        self.provider= provider
        self.name=name
        self.version = version

class Device:
    manufacturer:str= ''
    model:str= ''
    def __init__(self, manufacturer:str, model:str) -> None:
        self.manufacturer= manufacturer
        self.model = model

class Step:
    type:str
    description:str
    def __init__(self, type:str, description:str) -> None:
        self.type = type
        self.description= description

class ReferenceStep(Step):
    updateId:UpdateId
    def __init__(self, description:str, updateId:UpdateId) -> None:
        super().__init__('reference',description)
        self.updateId = updateId

class InlineStep(Step):
    handler:str
    files:List[str]
    handlerProperties:Dict[str,str]
    def __init__(self, description:str,files:List[str], installedCriteria:str) -> None:
        super().__init__('inline', description)
        self.handler= 'microsoft/swupdate:1'
        self.files= files
        self.handlerProperties= { 'installedCriteria' : installedCriteria }

class FirmwareFile:
    filename:str
    sizeInBytes:int
    hashes:Dict[str,str]
    def __init__(self, filename:str, sizeInBytes:int, sha256:str) -> None:
        self.filename=filename
        self.sizeInBytes=sizeInBytes
        self.hashes= { "sha256" : sha256 }


class Manifest:
    updateId:UpdateId
    isDeployable:bool
    compatibility:List[Device]
    instructions:Dict[str,object]
    files:List[FirmwareFile]
    createdDateTime:str
    manifestVersion:str
    def __init__(self, 
        updateId:UpdateId, 
        isDeployable:bool,
        compatibility:List[Device],
        steps:List[Step], 
        files:List[FirmwareFile],
        createdDateTime:str
        ) -> None:
        self.updateId= updateId
        self.isDeployable= isDeployable
        self.compatibility=compatibility
        self.instructions=dict(steps=steps)
        if files is not None:
            self.files= files
        self.createdDateTime=createdDateTime
        self.manifestVersion='4.0'


def dumper(obj):
    try:
        return obj.toJSON()
    except:
        return obj.__dict__

def sha256(filename):
    sha256_hash = hashlib.sha256()
    with open(filename,"rb") as f:
        # Read and update hash string value in blocks of 4K
        for byte_block in iter(lambda: f.read(4096),b""):
            sha256_hash.update(byte_block)
        return base64.b64encode(sha256_hash.digest()).decode('utf-8')

def extract_version_from_filename(filename):
    # format: x.y.z+build
    # ignore build number, only return x.y.z
    m = re.search('_(\\d+\\.\\d+\\.\\d+)(\\+\\d+)?(\\.\\w*)?$' , filename)
    if m is not None :
        return m.group(1)
    return ''

def extract_version_from_firmware(firmware):
    IMAGE_MAGIC = 0x96f3b83d
    ##
    # Retrieve version from binary
    # code from imgtool.py (image.py)
    ##
    with open(firmware, "rb") as f:
            b = f.read()
    # Magic    uint32
    # LoadAddr uint32
    # HdrSz    uint16
    # PTLVSz   uint16
    # ImgSz    uint32
    # Flags    uint32
    # Vers :    
    # iv_major uint8;
    # iv_minor uint8;
    # iv_revision uint16;
    # iv_build_num uint32;
    
    #magic, Load_addr, header_size, ptlvs, img_size, flags = struct.unpack('IIHHII', b[:20])
    magic, = struct.unpack('I', b[:4])
    iv_major, iv_minor, iv_revision, iv_build_num = struct.unpack('BBHI', b[20:28])
    # ignore build number
    # only return major.minor.revision
    return '' if magic != IMAGE_MAGIC else '{}.{}.{}'.format(iv_major, iv_minor, iv_revision)


parser = argparse.ArgumentParser(
                    prog='CreateSTM32U585Update',
                    description='Generate ADU assets for STM32U5',
                    )

parser.add_argument('-v'   , '--version', metavar='x.y.z', default='', help='Update package version: Ux.Uy.Uz')
parser.add_argument('-m'   , '--manufacturer', metavar='name', default='STMicroelectronics', help='Name of the device manufacturer')
parser.add_argument('-mo'  , '--model', metavar='model', dest='model', default='B-U585I-IOT02A', help='Model name of the device')

parser.add_argument('-ns' , '--non-secure', metavar='<filename>', dest='ns_firmware', default='', help='The filename of the non-secure firmware. The firmware version can be retrieved from the filename, <filename_x.y.z.bin>.')
parser.add_argument('-nsv', '--non-secure-version', metavar='x.y.z', dest='ns_version', default='', help='Version of the Non Secure firmware. Will override the version retrieved from the filename. Use with caution')
parser.add_argument('-nsd', '--non-secure-desc', metavar='"description"', dest='ns_desc', help='Description of the non-secure firmware.')

parser.add_argument('-s'  , '--secure', metavar='<filename>', dest='s_firmware', default='', help='The filename of the secure firmware. The firmware version can be retrieved from the filename, <filename_x.y.z.bin>.')
parser.add_argument('-sv' , '--secure-version', dest='s_version', metavar='"x.y.z"', default='', help='Version of the Secure firmware. Will override the version retrieved from the firmware. Use with caution')
parser.add_argument('-sd' , '--secure-desc', dest='s_desc', metavar='"description"', help='Description of the secure firmware')

parser.add_argument('-f'  , '--force', action='store_true', help='force the creation of the update package even if the target directory already exixts')
parser.add_argument('-i'  , '--interactive', action='store_true', help='interactive mode')
parser.add_argument('directory', nargs='?', help='The parent directory where the update package directory will be created')

args = parser.parse_args()

interactive = args.interactive


create_date_time = datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%S.%fZ')
manifest_version = "4.0"


##
# Process Update package version
##
update_version = args.version
if interactive:
    while True:
        val = input('Update package version[{}]: '.format(update_version))
        val = val.strip()
        val = val if val != '' else update_version
        if re.match("^(\\d+)\\.(\\d+)\\.(\\d+)$", val) :
            update_version = val 
            break
        print('Invalid update package version: \'{}\''.format(val))

if update_version == '':
    print('Update package version is mandatory')
    parser.print_usage()
    sys.exit(1)

if not re.match("^(\\d+)\\.(\\d+)\\.(\\d+)$", update_version) :
    print("Update package version is invalid: "+update_version)
    parser.print_usage()
    sys.exit(1)

##
# process Manufacturer / model 
##
manufacturer = args.manufacturer
model= args.model

if interactive:
    val = input('Manufacturer [{}]: '.format(manufacturer))
    val = val.strip()
    if val != '' :
        manufacturer = val


if interactive:
    val = input('model [{}]: '.format(model))
    val = val.strip()
    if val != '' :
        model = val

##
# Process Non Secure firmware
##
ns_firmware= args.ns_firmware
ns_version = args.ns_version
ns_desc= args.ns_desc

if interactive:
    while True:
        val = input('Non-secure firmware[{}]: '.format(ns_firmware))
        val = val.strip()
        if val != '':
            ns_firmware = val
        if Path(ns_firmware).exists():
            break
        print('ERROR: Unbale to find non-secure firmware: {}'.format(ns_firmware))
        

if ns_firmware != '':
    ns_firmware = Path(ns_firmware)

    if not ns_firmware.exists():
        print('ERROR: Unbale to find non-secure firmware: {}'.format(ns_firmware))
        parser.print_usage()
        sys.exit(1)

    fw_version = extract_version_from_firmware(ns_firmware)
    ns_version = fw_version if ns_version == '' else ns_version
    # TODO: Add warning in case of mismatched !! Should we abort packaging ?

    if interactive:
        while True:
            val = input('Non-secure fw version[{}]: '.format(ns_version))
            val = val.strip()
            val = val if val != '' else ns_version
            if re.match("^(\\d+)\\.(\\d+)\\.(\\d+)(\\+\\d+)?$", val) :
                ns_version = val
                break

    if not re.match("^(\\d+)\\.(\\d+)\\.(\\d+)(\\+\\d+)?$", ns_version) :
        print("Invalid non-secure firmware version: "+ns_version)
        parser.print_usage()
        sys.exit(1)

    ns_desc = ns_desc if ns_desc is not None else 'Non-secure firmware '+ns_version


    if interactive:
        val = input('Non-secure fw description[{}]: '.format(ns_desc))
        val = val.strip()
        if val != '' :
            ns_desc = val
else:
    ns_firmware = None

##
# Process Secure firmware
##
s_firmware= args.s_firmware
s_version = args.s_version
s_desc= args.s_desc


if interactive:
    while True:
        val = input('Secure firmware[{}]: '.format(s_firmware))
        val = val.strip()
        if val != '':
            s_firmware = val
        if Path(s_firmware).exists():
            break
        print('ERROR: Unbale to find secure firmware: {}'.format(s_firmware))

if s_firmware != '' :
    s_firmware = Path(s_firmware)

    if not s_firmware.exists():
        print('ERROR: Unbale to find secure firmware: {}'.format(s_firmware))
        parser.print_usage()
        sys.exit(1)

    fw_version = extract_version_from_firmware(s_firmware)
    s_version = fw_version if s_version == '' else s_version
    # TODO: Add warning in case of mismatched !! Should we abort packaging ?

    if interactive:
        while True:
            val = input('Secure fw version[{}]: '.format(s_version))
            val = val.strip()
            val = val if val != '' else s_version
            if re.match("^(\\d+)\\.(\\d+)\\.(\\d+)$", val) :
                s_version = val
                break

    if not re.match("^(\\d+)\\.(\\d+)\\.(\\d+)$", s_version) :
        print("Invalid secure firmware version: "+s_version)
        parser.print_usage()
        sys.exit(1)

    s_desc = s_desc if s_desc is not None else 'Secure firmware '+s_version

    if interactive:
        val = input('Secure fw description[{}]: '.format(s_desc))
        val = val.strip()
        if val != '' :
            s_desc = val
else:
    s_firmware = None

##
# Sanity check
##
if s_firmware is None and ns_firmware is None :
    print("At least one firmware must be provided")
    sys.exit(1)


##
# Package directory
##
update_package = manufacturer+'.'+model+'.'+update_version

package_dir = args.directory
package_dir = package_dir if package_dir is not None else update_package

if interactive:
    val = input('Target directory [{}]: '.format(package_dir))
    val = val.strip()
    if val != '':
        package_dir = val

if package_dir is None:
    print("ERROR: Package directory is not defined")
    parser.print_usage()
    sys.exit(1)

package_dir = Path(package_dir)


##
# Packaging
##

s_model = model+"-S"

if s_firmware is not None :
    s_firmware = Path(s_firmware)
    s_size = s_firmware.stat().st_size
    s_sha256 = sha256(s_firmware)

print('Generating update package:\n{}'.format(package_dir.resolve()))
if package_dir.exists():
    force = args.force
    if interactive:
        print("WARNING: Update package directory already exists !!!")
        while True:
            val = input('Proceed anyway (Y/N) [{}]: '.format('Y' if force else 'N'))
            val.strip()
            if val == '':
                break
            if val == 'Y' or val == 'y':
                force = True
                break
            if val == 'N' or val == 'n':
                force = False
                break
            
    if not force:
        if interactive:
            print("ERROR: Packaging aborted !!!")
        else:
            print("ERROR: Update package directory already exists.")
            print("ERROR: Use --force option to force package creation.")
        sys.exit(1)
    print('WARNING: Using existing update package directory')

os.makedirs(package_dir, exist_ok=True)

if s_firmware is not None:
    dest = package_dir / '{}.{}.{}_s_app_{}.bin'.format(manufacturer, s_model, update_version, s_version)
    print('Packaging secure firmware: {}'.format(dest.name))
    if dest.exists():
        os.remove(dest)
    shutil.copy(s_firmware, dest)
    s_firmware = dest
    s_size = s_firmware.stat().st_size
    s_sha256 = sha256(s_firmware)


if ns_firmware is not None:
    dest = package_dir / '{}.{}.{}_ns_app_{}.bin'.format(manufacturer, model, update_version, ns_version)
    print('Packaging non-secure firmware: {}'.format(dest.name))
    if dest.exists():
        os.remove(dest)
    shutil.copy(ns_firmware, dest)
    ns_firmware = dest
    ns_size = ns_firmware.stat().st_size
    ns_sha256 = sha256(ns_firmware)


if s_firmware is not None:
    # Leaf
    s_manifest= Manifest(
        UpdateId(manufacturer,s_model,update_version),
        False,
        [Device(manufacturer,s_model)],
        [InlineStep(s_desc, [s_firmware.name], s_version)],
        [FirmwareFile(s_firmware.name, s_size, s_sha256) ],
        create_date_time
    )

    s_manifest_file = package_dir / str(manufacturer+'.'+s_model + '.' + update_version+'.importmanifest.json')
    print('Generating Secure update manifest: ' + s_manifest_file.name)
    with open(s_manifest_file, "w") as outfile:
        json.dump(s_manifest, outfile, indent=2, default=dumper )


steps=[]
files= None
if s_firmware is not None:
    steps.append(ReferenceStep(s_desc, UpdateId(manufacturer,s_model,update_version)))
if ns_firmware is not None:
    steps.append(InlineStep(ns_desc, [ns_firmware.name], ns_version))
    files = [FirmwareFile(ns_firmware.name, ns_size, ns_sha256) ]

ns_manifest= Manifest(
    UpdateId(manufacturer,model,update_version),
    True,
    [Device(manufacturer,model)],
    steps,
    files,
    create_date_time
)

ns_manifest_file = package_dir / str(manufacturer+'.'+model+ '.' + update_version+'.importmanifest.json')
print('Generating Non-Secure update manifest: ' + ns_manifest_file.name)
with open(ns_manifest_file, "w") as outfile:
    json.dump(ns_manifest, outfile, indent=2, default=dumper )


sys.exit(0)

