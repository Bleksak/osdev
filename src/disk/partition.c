#include "partition.h"
#include "../console.h"
#include "../memory.h"
#include "../mheap.h"
#include "../os.h"
#include "../paging.h"


extern OS os;

#define MBR_OFFSET 446

struct MBRPartition {
    uint8_t boot_indicator;
    uint8_t start_head;
    uint16_t start_sector: 6;
    uint16_t start_cylinder: 10;
    uint8_t system_id;
    uint8_t end_head;
    uint16_t end_sector: 6;
    uint16_t end_cylinder: 10;
    uint32_t partition_start;
    uint32_t partition_size;
} __attribute__((packed));

static uint64_t checksum(const struct MBRPartition* partition) {

    uint64_t checksum = 0;

    for(size_t i = 0; i < sizeof(struct MBRPartition); ++i) {
        checksum += ((uint8_t*)partition)[i];
    }

    return checksum;
}

size_t partition_init_drive(const Drive* drive) { // returns the partition count
    Result res = ata_read(drive, 0, 1);

    if(!res.ok) {
        return 0;
    }

    const struct MBRPartition* mbr = (void*) ((uintptr_t)res.result + MBR_OFFSET);
    size_t partition_count = 0;

    for(size_t i = 0; i < 4; ++i) {
        if(checksum(mbr) == 0) {
            continue;
        }

        if(!mbr[i].partition_size) {
            continue;
        }

        if(!mbr[i].start_sector || !mbr[i].end_sector) {
            continue;
        }

        if(!mbr[i].system_id) {
            continue;
        }
        /*
            null ptr
            => mem_get_offset()

        */
        // *partitions = realloc(*partitions, sizeof(Partition) * (partition_count + 1));
        // (*partitions)[partition_count].id = i;
        // (*partitions)[partition_count].drive = &os.drives[drive];
        // (*partitions)[partition_count].start = mbr[i].partition_start;
        // (*partitions)[partition_count].size  = mbr[i].partition_size;

        partition_count++;
    }
    
    return partition_count;
}
