#pragma once

#include "../../std.h"

typedef unsigned long long elf64_addr_t;
typedef unsigned long long elf64_offset_t;
typedef unsigned short elf64_half_t;
typedef unsigned int elf64_word_t;
typedef signed int elf64_sword_t;
typedef unsigned long long elf64_xword_t;
typedef signed long long elf64_xsword_t;

enum elf_bits {
    BITS32 = 1,
    BITS64 = 2,
};

enum elf_program_types {
    relocate = 1,
    execute = 2,
    shared = 3,
    core = 4,
};

enum elf_segment_type {
    executable = 1,
    writable = 2,
    readable = 4,
};

enum elf_instruction_set {
    unknown = 0,
    sparc = 2,
    x86 = 3,
    mips = 8,
    powerpc = 0x14,
    ARM = 0x28,
    superh = 0x2A,
    IA_64 = 0x32,
    x86_64 = 0x3E,
    AArch64 = 0xb7,
};

struct elf_header32 {
    uint32_t magic;
    uint8_t op_size;
    uint8_t endianity;
    uint8_t _elf_version;
    uint8_t os_abi;
    uint64_t padding;

    uint16_t type;
    uint16_t instruction_set;
    uint32_t elf_version;
    uint32_t program_entry;
    uint32_t program_header_table_offset;
    uint32_t section_header_table_offset;
    uint32_t flags;
    uint16_t header_size;
    
    uint16_t program_header_entry_size;
    uint16_t program_header_entry_count;

    uint16_t section_header_table_size;
    uint16_t section_header_table_count;
    uint16_t section_names_index;
} __attribute__((packed));

struct elf_program_header {
    uint32_t segment_type;
    uint32_t data_file_offset; //p_offset
    uint32_t virtual_memory_start;
    uint32_t undefined;
    uint32_t file_size;
    uint32_t memory_size;
    uint32_t flags;
    uint32_t alignment;    
} __attribute__((packed));

struct elf_section_header {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} __attribute__((packed));
