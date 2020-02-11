#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <disk/partition.h>

struct FSFILE {
    const char* name;
    size_t size;
    void* data;
};

typedef FSFILE FSFILE;

typedef FSFILE* (*fsopen)(Partition* partition, const char* name);
typedef void (*fsclose)(Partition* partition, FSFILE* f);
typedef bool (*fsidentify)(Partition* partition);

struct FsDriver {
    const char* name;
    fsidentify identify;
    fsopen open;
    fsclose close;
};

typedef FsDriver FsDriver;

FsDriver* fs_find_driver(Partition* partition);
