#!env sh
 arm-none-eabi-objdump --disassemble=secure main
 arm-none-eabi-objdump --disassemble=__gnu_cmse_nonsecure_call main
