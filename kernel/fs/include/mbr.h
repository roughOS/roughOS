#pragma once

#include <stdint.h>

#define MAX_PARTITIONS                  4

#define MBR_SIGNATURE                   0xaa55

typedef struct
{
    uint8_t bootable;
    uint8_t chs_start[3];
    uint8_t type;
    uint8_t chs_end[3];
    uint32_t lba_start;
    uint32_t sectors;
} __attribute__((packed)) MBRPartition;

typedef struct
{
    uint8_t boot_code[446];
    MBRPartition partitions[MAX_PARTITIONS];
    uint16_t signature;
} __attribute__ ((packed)) MBR;

int mbr_read_partitions(MBRPartition *out_partitions);