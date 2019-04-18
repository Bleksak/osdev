#pragma once
#ifdef _KERNEL_CPU_32
typedef unsigned int size_t;
#else
typedef unsigned long long size_t;
#endif
