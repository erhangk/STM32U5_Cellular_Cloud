@echo OFF
echo Processing linker file %1\stm32u585xx_flash_s.icf
iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2 -I%1\..\..\..\Linker %1\stm32u585xx_flash_s.icf --silent --preprocess=ns %1\flash_s.icf.i >> %1\output.txt 2>&1
