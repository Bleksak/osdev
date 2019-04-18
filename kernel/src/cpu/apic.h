#pragma once

#include <stdint.h>
#include "acpi.h"

struct MP_Entry
{
	uint8_t type, record_length;
};

struct Local_APIC
{
	uint8_t acpi_cpu_id;
	uint8_t apic_id;
	uint32_t flags; // bit 1 == cpu_enabled
} __attribute__((packed)); // entry type 0


struct IO_APIC
{
	uint8_t ioapic_id;
	uint8_t reserved;
	uint32_t ioapic_addr;
	uint32_t global_system_interrupt_base;
} __attribute__((packed)); //entry type 1


struct InterruptSourceOverride
{
	uint8_t bus_source;
	uint8_t irq_source;
	uint32_t global_system_interrupt;
	uint16_t flags; // same as NMI ::
	/*

		 bit 1 => active when low
		bit 4 => level triggered

	*/
} __attribute__((packed)); //entry type 2


struct NMI
{
	uint8_t acpi_cpu_id; // 0xff means all cpus
	uint16_t flags;
	uint8_t LINT; // 0 or 1
} __attribute__((packed)); //entry type 4


struct LAPIC_Override
{
	uint16_t reserved;
	uint64_t local_apic_addr; // if this is defined, use it instead of the 32bit version in
	// the header
} __attribute__((packed)); // entry type 5

struct MADT
{
	struct RSDT sdt;

	uint32_t local_apic_addr;
	uint32_t flags; // bit 1 = Dual 8259 Legacy PICs installed

	// variable length stuff, find through sdt.length

	struct MP_Entry entries;

} __attribute__((packed));


void apic_init();

