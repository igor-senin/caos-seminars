#!/usr/bin/env bash

gcc -S $@ -O0 -pthread -fno-asynchronous-unwind-tables -fno-dwarf2-cfi-asm -masm=intel
