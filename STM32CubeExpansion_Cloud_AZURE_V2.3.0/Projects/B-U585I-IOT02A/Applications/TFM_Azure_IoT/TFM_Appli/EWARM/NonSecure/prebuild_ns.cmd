@echo OFF
echo Processsing linker file %1\stm32u585xx_flash_ns.icf
iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2 -DTFM_PSA_API -DTFM_PARTITION_PLATFORM  -I%1\..\..\..\Linker  %1\stm32u585xx_flash_ns.icf  --silent --preprocess=ns %1\stm32u585xx_flash_ns.icf.i > %1\output.txt 2>&1
