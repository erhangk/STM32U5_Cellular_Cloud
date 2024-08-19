#!/bin/bash

# ARGS: 
#   $1 build directory
#   $2 CPU

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
scriptdir=`dirname $SCRIPT`

projectdir=$(readlink -f $1) 
cpu=$2

if [ ! -d $projectdir ]; then
	mkdir -p $projectdir
fi

Third_Party=$scriptdir"/../../../../../../../Middlewares/Third_Party"
TFM_TEST_PATH=$Third_Party"/trustedfirmware/test"
TFM_PATH=$Third_Party"/trustedfirmware"
TOOLS=$Third_Party"/trustedfirmware/tools"
# Recopy config for generation
CONFIG=$scriptdir"/../../Secure/Src"
PARTITION=$Third_Party"/trustedfirmware/secure_fw/partitions"

# Config from application
cp $CONFIG"/tfm_manifest_list.yaml" $projectdir
cp $CONFIG"/tfm_generated_file_list.yaml" $projectdir

# Config platform customizeable
cp $CONFIG"/tfm_app_rot.yaml" $projectdir
cp $CONFIG"/tfm_platform.yaml" $projectdir

# Config from middleware standard services
$(cp $PARTITION"/crypto/tfm_crypto.yaml" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/protected_storage/tfm_protected_storage.yaml" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/internal_trusted_storage/tfm_internal_trusted_storage.yaml" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/initial_attestation/tfm_initial_attestation.yaml" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/firmware_update/tfm_firmware_update.yaml" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/tfm_ffm11_partition/tfm_ffm11_partition.yaml" $projectdir 2>/dev/null | true)
# Copy template file from middleware
$(cp $PARTITION"/manifestfilename.template" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/partition_intermedia.template" $projectdir 2>/dev/null | true)
$(cp $PARTITION"/tfm_service_list.inc.template" $projectdir 2>/dev/null | true)
$(cp $TFM_PATH"/interface/include/tfm_veneers.h.template" $projectdir 2>/dev/null | true)
$(cp $TFM_PATH"/secure_fw/spm/cmsis_psa/tfm_secure_irq_handlers_ipc.inc.template" $projectdir 2>/dev/null | true)
$(cp $TFM_PATH"/secure_fw/spm/cmsis_psa/tfm_spm_db_ipc.inc.template" $projectdir 2>/dev/null | true)
$(cp $TFM_PATH"/interface/include/psa_manifest/sid.h.template" $projectdir 2>/dev/null | true)
$(cp $TFM_PATH"/interface/include/psa_manifest/pid.h.template" $projectdir 2>/dev/null | true)
# Copy config for test
$(cp $TFM_TEST_PATH"/test_services/tfm_core_test/tfm_ss_core_test.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_core_test_2/tfm_ss_core_test_2.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_secure_client_service/tfm_secure_client_service.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_ipc_service/tfm_ipc_service_test.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_secure_client_service/tfm_secure_client_service.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_ipc_client/tfm_ipc_client_test.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_ps_test_service/tfm_ps_test_service.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_ipc_service/tfm_ipc_service_test.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_ipc_client/tfm_ipc_client_test.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_irq_test_service_1/tfm_irq_test_service_1.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_ps_test_service/tfm_ps_test_service.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_secure_client_2/tfm_secure_client_2.yaml" $projectdir 2>/dev/null | true)
$(cp $TFM_TEST_PATH"/test_services/tfm_core_test/tfm_ss_core_test.yaml" $projectdir 2>/dev/null | true)
# Config platform customizeable
$(cp $CONFIG"/tfm_app_rot.yaml" $projectdir 2>/dev/null | true)
$(cp $CONFIG"/tfm_platform.yaml" $projectdir 2>/dev/null | true)

export projectdir

tfm_parse_manifest_list=$TOOLS"/dist/tfm_parse_manifest_list/tfm_parse_manifest_list.exe"
uname | grep -i -e windows -e mingw > /dev/null
if [ $? == 0 ] && [ -e "$tfm_parse_manifest_list" ]; then
command=$tfm_parse_manifest_list" -o "$projectdir"/../g -m "$projectdir"/tfm_manifest_list.yaml -f "$projectdir"/tfm_generated_file_list.yaml"
else
command=python" "$TOOLS"/dist/pyscript/tfm_parse_manifest_list.py -o "$scriptdir"/g -m "$projectdir"/tfm_manifest_list.yaml -f "$projectdir"/tfm_generated_file_list.yaml"
fi
$command
if [ $? -eq 0 ] ; then 
echo Updating linker file
command="arm-none-eabi-gcc -E -P -xc -DBL2 -D"$cpu" -DTFM_PSA_API  -I$scriptdir/../../../Linker -o $projectdir/output.ld  $scriptdir/tfm_common_s.ld"
$command 
fi