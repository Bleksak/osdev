#pragma once
#include "disk/ata.h"

struct Partition
{
    unsigned char id;
    struct Drive* drive;
    unsigned int start;
    unsigned int size;
};

void partition_init();
unsigned char partition_read(struct Partition* partition, unsigned int lba, unsigned short size, unsigned char* buffer);
unsigned char partition_write(struct Partition* partition, unsigned int lba, unsigned short size, unsigned char* buffer);