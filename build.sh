source ~/Tools/esp/esp-idf/export.sh


idf.py set-target esp32s2

idf.py menuconfig

idf.py build

# idf.py -p /dev/ttyUSB0 flash

# idf.py monitor

# idf.py fullclean
# idf.py build
