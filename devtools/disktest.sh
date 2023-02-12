#!/bin/bash
nasm -f bin -o mbr.bin disktest.asm
mv ../hdd.img ../hdd.img.bak
mv mbr.bin ../hdd.img