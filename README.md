# RoughOS
*A small, free and open-source 32-bit operating system for x86.*

## Why should I use RoughOS?
Let's be honest: you shouldn't.

The OS is at an early development stage. Using it as your main OS would be a very bad and stupid idea. But you can read the source code, contribute to help its improvement, or even use it as a base for your operating system.

## Features
- VFS
- MBR support
- Heap
- Physical memory management
- Kernel libc
- ATA driver
- Keyboard driver
- PS/2 driver
- PIC driver
- VGA driver
- Interrupts support
- GDT
- Booting with GRUB2

## Building
1. Clone the repository
```bash
git clone https://github.com/roughOS/roughOS
```

2. Run the build script
```bash
cd roughOS
chmod +x build.sh
./build.sh
```

A file named `roughos.iso` and another one named `ide_disk.img` should have been created!
You can now run RoughOS using the following command:
```bash
qemu-system-i386 -cdrom roughos.iso -drive format=raw,file=ide_disk.img -boot d
```

## Run a program
RoughOS cannot run binaries yet, but you can make programs for it without editing the source code.

Open [`template.c`](kernel/template.c). You will see a basic function displaying "Hello world!" using the klibc `kprintf()` function. This function is automatically executed by the kernel when you run the operating system. Just recompile it when you edit the function code.

You can also interact with the VFS (Virtual FileSystem) using [the VFS header](kernel/fs/include/vfs.h).

## Contributing
Everyone is free to contribute to WitherOS! Read detailed contribution rules [here](CONTRIBUTING.md).

## Changelog
See a list of every updates [here](CHANGELOG.md).