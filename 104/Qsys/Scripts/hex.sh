cd ../Software/tpk_key
make mem_init_generate
cd ../tpk_led
make mem_init_generate
cd ../../Scripts
python3 ./MergeHex.py ../Software/tpk_key/mem_init/Software.hex ../Software/tpk_led/mem_init/Software.hex 1000
