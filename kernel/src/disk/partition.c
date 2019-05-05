#include "disk/partition.h"
#include "console.h"
#include "memory.h"

struct mbr_partition
{
    unsigned char boot_indicator;
    unsigned char start_head;
    unsigned short start_sector: 6;
    unsigned short start_cylinder: 10;
    unsigned char system_id;
    unsigned char end_head;
    unsigned short end_sector: 6;
    unsigned short end_cylinder: 10;
    unsigned int partition_start;
    unsigned int partition_size;
} PACKED;


