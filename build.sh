#!/bin/bash

# Stop if an error occurs
set -e

# Variables
ISO_DIR=iso
ELF_NAME=witheros.elf
ISO_NAME=witheros.iso
DISK_IMAGE=ide_disk.img

# Build the OS
make clean && make

# Create the ISO directory
rm -rf $ISO_DIR
mkdir -p $ISO_DIR/boot/grub

# Copy files into ISO directory
cp build/$ELF_NAME $ISO_DIR/boot/
cp grub.cfg $ISO_DIR/boot/grub/

# Create the ISO
grub-mkrescue -o "$ISO_NAME" "$ISO_DIR"

# Create an empty 1MB disk image
rm -rf $DISK_IMAGE
dd if=/dev/zero of=$DISK_IMAGE bs=512 count=204800