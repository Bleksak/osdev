#pragma once

#include <disk/partition.h>

bool fat16_identify(Partition* partition);
FSFILE* fat16_open(Partition* partition, const char* fname);
void fat16_close(FSFILE* fsfile);
