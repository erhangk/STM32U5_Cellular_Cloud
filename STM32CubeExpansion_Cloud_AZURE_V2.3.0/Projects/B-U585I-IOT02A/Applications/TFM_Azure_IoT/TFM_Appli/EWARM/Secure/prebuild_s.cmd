@echo OFF
echo Processing linker file %1\tfm_common_s.icf
iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2 -DTFM_PSA_API -DTFM_PARTITION_PLATFORM  -I%1\..\..\..\Linker  %1\tfm_common_s.icf  --silent --preprocess=ns %1\tfm_common_s.icf.i >> %1\output.txt 2>&1
