#include "fs/include/mbr.h"
#include "drivers/include/ide.h"
#include <stdint.h>

int mbr_read_partitions(MBRPartition *out_partitions)
{
    uint16_t sector[256];
    if (!ide_read_sector(0, sector))
        return -1;

    uint8_t *mbr = (uint8_t *) sector;
    if (!mbr) return -3;

    if (mbr[510] != 0x55 || mbr[511] != 0xaa)
        return -2;

    for (int i = 0; i < MAX_PARTITIONS; i++)
    {
        MBRPartition *entry = (MBRPartition *)(mbr + 446 + i * 16);
        if (!entry) return -4;

        out_partitions[i] = *entry;
    }

    return 0;
}