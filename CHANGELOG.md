# RoughOS Changelog

**Version 0.0.0**:
- First release

**Version 0.0.1**:
- Added ATA driver
- Added MBR support

**Version 0.0.2**:
- More robust code
- Code cleanup

**Version 0.0.3**:
- Added basic VFS
- Added tmpfs:
    - Create directories and files
    - List directories and files
    - Read files
    - Edit files
- Added functions to kstring.h:
    - kstrlen()
    - kstrcpy()
    - kstrncpy()
    - kstrchr()
    - kstrtok()

**Version 0.0.4**:
- Improved VFS:
    - tmpfs backend
    - FS drivers management
    - Can remove files and directories