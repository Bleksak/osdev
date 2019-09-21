#pragma once
#include "ata.h"
#include "../std.h"

struct Partition {
    unsigned char id;
    struct Drive* drive;
    unsigned int start;
    unsigned int size;
};

typedef struct Partition Partition;

size_t partition_init_drive(uint32_t drive, Partition** partitions);

unsigned char partition_read(struct Partition* partition, unsigned int lba, unsigned short size, unsigned char* buffer);
unsigned char partition_write(struct Partition* partition, unsigned int lba, unsigned short size, unsigned char* buffer);