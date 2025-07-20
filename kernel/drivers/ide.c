#include "drivers/include/ide.h"
#include "cpu/include/io.h"
#include <stdint.h>

static void ide_wait_busy()
{
    while (inb(IDE_STATUS) & IDE_BSY);
}

static void ide_wait_drq()
{
    while (!(inb(IDE_STATUS) & IDE_DRQ));
}

int ide_identify(uint16_t *buffer)
{
    outb(IDE_DRIVE_HEAD, 0xA0);
    outb(IDE_SECTOR_COUNT, 0);
    outb(IDE_LBA_LOW, 0);
    outb(IDE_LBA_MID, 0);
    outb(IDE_LBA_HIGH, 0);
    outb(IDE_COM, IDE_CMD_IDENTIFY);


    if (inb(IDE_STATUS) == 0)
        return 0;

    if (inb(IDE_LBA_MID) != 0 || inb(IDE_LBA_HIGH) != 0)
        return 0;

    ide_wait_drq();
    insw(IDE_DATA, buffer, 256);

    return 1;
}

int ide_read_sector(uint32_t lba, uint16_t *buffer)
{
    ide_wait_busy();

    outb(IDE_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    outb(IDE_SECTOR_COUNT, 1);
    outb(IDE_LBA_LOW, (uint8_t)(lba & 0xFF));
    outb(IDE_LBA_MID, (uint8_t)((lba >> 8) & 0xFF));
    outb(IDE_LBA_HIGH, (uint8_t)((lba >> 16) & 0xFF));
    outb(IDE_COM, IDE_CMD_READ_SECTORS);

    ide_wait_busy();
    ide_wait_drq();

    insw(IDE_DATA, buffer, 256);

    return 1;
}

int ide_write_sector(uint32_t lba, uint16_t *buffer)
{
    ide_wait_busy();


    outb(IDE_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    outb(IDE_SECTOR_COUNT, 1);
    outb(IDE_LBA_LOW,  lba & 0xFF);
    outb(IDE_LBA_MID,  (lba >> 8) & 0xFF);
    outb(IDE_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(IDE_COM, IDE_CMD_WRITE_SECTORS);

    ide_wait_busy();
    ide_wait_drq();

    outsw(IDE_DATA, buffer, 256);

    ide_wait_busy();

    return 1;
}