cd ../Software/tpk_key_bsp
nios2-bsp-generate-files.exe --bsp-dir . --settings settings.bsp
make -j6
cd ../tpk_led_bsp
nios2-bsp-generate-files.exe --bsp-dir . --settings settings.bsp
make -j6
# cd ../tpk_key
# make -j
# cd ../tpk_led
# make -j
