#!/bin/bash
nasm -f bin -o mbr.bin lol.asm
mv mbr.bin ../mbr.bin