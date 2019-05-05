#pragma once

struct cpuid
{
    unsigned int maximum_cpuid_input_value;
    char cpu_brand_string[12];
    unsigned int cpu_signature;
    unsigned char brand_index;
    unsigned char clflush_line_size; // *8 clflsh feature flag
    unsigned char logical_cpu_count; // Hyper-Threading flag
    unsigned char local_apic_id;
    struct
    {
        unsigned char fpu : 1;
        unsigned char vme : 1;
        unsigned char de  : 1;
        unsigned char pse : 1;
        unsigned char tsc : 1;
        unsigned char msr : 1;
        unsigned char pae : 1;
        unsigned char mce : 1;
        unsigned char cx8 : 1;
        unsigned char apic: 1;
        unsigned char res : 1;
        unsigned char sep : 1;
        unsigned char mtrr: 1;
        unsigned char pge : 1;
        unsigned char mca : 1;
        unsigned char cmov: 1;
        unsigned char pat : 1;
        unsigned char pse36:1;
        unsigned char psn : 1;
        unsigned char clfsh :1;
        unsigned char res1 : 1;
        unsigned char ds : 1;
        unsigned char acpi : 1;
        unsigned char mmx : 1;
        unsigned char fxsr :1;
        unsigned char sse : 1;
        unsigned char sse2 : 1;
        unsigned char ss : 1;
        unsigned char htt : 1;
        unsigned char tm : 1;
        unsigned char res2 : 1;
        unsigned char res3 : 1;

        unsigned char sse3 : 1;
        unsigned char pclmulqdq : 1;
        unsigned char dtes64 : 1;
        unsigned char monitor : 1;
        unsigned char dscpl : 1;
        unsigned char vmx : 1;
        unsigned char smx : 1;
        unsigned char eist : 1;
        unsigned char tm2 : 1;
        unsigned char ssse3 : 1;
        unsigned char cntxid : 1;
        unsigned char sdbg : 1;
        unsigned char fma : 1;
        unsigned char cmpxchg16b :1;
        unsigned char xtpruc : 1;
        unsigned char pdcm : 1;
        unsigned char res4 : 1;
        unsigned char pcid : 1;
        unsigned char dca : 1;
        unsigned char sse41 : 1;
        unsigned char sse42 : 1;
        unsigned char x2apic : 1;
        unsigned char movbe : 1;
        unsigned char popcnt : 1;
        unsigned char tscdeadline : 1;
        unsigned char aesnii :1;
        unsigned char xsave : 1;
        unsigned char osxsave : 1;
        unsigned char avx : 1;
        unsigned char f16c : 1;
        unsigned char rdrand :1;
        unsigned char res5;
    } PACKED features;

    unsigned int maximum_cpuid_input_value_ext;
    unsigned char cpu_brand_string_ext[48];
} PACKED;

void cpu_init();

struct cpuid* get_cpu_info();