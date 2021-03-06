#pragma once

struct Partition;
typedef struct Partition Partition;

#include "ata.h"
#include <stdint.h>
#include <stddef.h>

struct Partition {
    unsigned char id;
    Drive* drive;
    unsigned int start;
    unsigned int size;

    

};

size_t partition_init_drive(const Drive* drive);

unsigned char partition_read(struct Partition* partition, unsigned int lba, unsigned short size, unsigned char* buffer);
unsigned char partition_write(struct Partition* partition, unsigned int lba, unsigned short size, unsigned char* buffer);
