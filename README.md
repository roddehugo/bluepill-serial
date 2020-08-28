# Bluepill USB serial device

Boilerplate for using libopencm3 USB CDC on STM32F1 Bluepill board.

    $ make
    $ arm-none-eabi-gdb src/program.elf
    (gdb) target extended-remote <port>
    (gdb) monitor swdp_scan
    (gdb) attach 1
    (gdb) load
    (gdb) run
