#pragma once

#include <stdint.h>

#define IDE_DATA                    0x1f0
#define IDE_ERROR                   0x1f1
#define IDE_SECTOR_COUNT            0x1f2
#define IDE_LBA_LOW                 0x1f3
#define IDE_LBA_MID                 0x1f4
#define IDE_LBA_HIGH                0x1f5
#define IDE_DRIVE_HEAD              0x1f6
#define IDE_COM                     0x1f7
#define IDE_STATUS                  0x1f7
#define IDE_CONTROL                 0x3f6

#define IDE_BSY                     0x80
#define IDE_DRDY                    0x40
#define IDE_DF                      0x20
#define IDE_DSC                     0x10
#define IDE_DRQ                     0x08
#define IDE_CORR                    0x04
#define IDE_IDX                     0x02
#define IDE_ERR                     0x01

#define IDE_CMD_IDENTIFY            0xec
#define IDE_CMD_READ_SECTORS        0x20
#define IDE_CMD_WRITE_SECTORS       0x30

/*
    Identify the ATA/IDE drive

    Params:
        buffer - where to store the data

    Return:
        0 - No device present
        1 - Success
*/
int ide_identify(uint16_t *buffer);

/*
    Read a sector from the disk

    Params:
        lba - start LBA
        buffer - where to store the data

    Return:
        1 - Success
*/
int ide_read_sector(uint32_t lba, uint16_t *buffer);

/*
    Write a sector on the disk

    Params:
        lba - start LBA
        buffer - data to write

    Return:
        1 - success
*/
int ide_write_sector(uint32_t lba, uint16_t *buffer);