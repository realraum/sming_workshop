1) rebuild Sming with
   - cd $SMING_HOME/SMIG 
   - make clean
   - ENABLE_GDB=1 make

2) flash app
   - make flash
 
3) debug with: 
    $ESP_HOME/xtensa-lx106-elf/bin/xtensa-lx106-elf-gdb -x gdbcmds -b 115200