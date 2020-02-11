#include "fs.h"
#include "fat16/fat16.h"

static const FsDriver fsdrivers[] = {
    {
        .name = "FAT",
        .identify = fat16_identify,
        .open = fat16_open,
        .close = fat16_close,
    }
};

FsDriver* fs_find_driver(Partition* partition) {
    
}
