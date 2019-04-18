#include "acpi.h"
#include "console.h"
#include "cpu/apic.h"
#include <stddef.h>
#include "AML.h"


struct RSDP_2
{
	uint32_t length;
	uint64_t xsdt_address;
	uint8_t ext_checksum;
	uint8_t reserved[3];
} __attribute__((packed));

struct RSDP
{
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt_address;
	
	struct RSDP_2 xsdt;
} __attribute__((packed)) * rsdp = 0;

struct RSDT_Header
{
	struct RSDT rsdt;
	uint32_t sdt_pointer[];
} * rsdt;

enum entries_string
{
	MADT = 0x43495041, //"APIC" represented as uint 
	DSDT = 0x54445344,
	FADT = 0x50434146,
};

struct GenericAddressStructure
{
	uint8_t address_space;
	uint8_t bit_width;
	uint8_t bit_offset;
	uint8_t access_size;
	uint64_t address;
} __attribute__((packed));

struct FADT
{
	struct RSDT sdt;
	uint32_t firmware_ctl;
	uint32_t dsdt;

	uint8_t reserved;

	uint8_t preferred_profile;

	uint16_t sci_interrupt;
	uint32_t smi_command_port;
	uint8_t acpi_enable;
	uint8_t acpi_disable;
	uint8_t s4bios_req;
	uint8_t pstate_control;

	uint32_t pm1a_event_block;
	uint32_t pm1b_event_block;
	uint32_t pm1a_control_block;
	uint32_t pm1b_control_block;
	uint32_t pm2_control_block;
	uint32_t pm_timer_block;
	
	uint32_t gpe0_block;
	uint32_t gpe1_block;

	uint8_t pm1_event_length;
	uint8_t pm1_control_length;
	uint8_t pm2_control_length;
	uint8_t pm_timer_length;
	
	uint8_t gpe0_length;
	uint8_t gpe1_length;
	uint8_t gpe1_base;
	uint8_t c_state_control;

	uint16_t worst_c2_latency;
	uint16_t worst_c3_latency;

	uint16_t flush_size;
	uint16_t flush_stride;

	uint8_t duty_offset;
	uint8_t duty_width;
	uint8_t day_alarm;
	uint8_t month_alarm;
	uint8_t century;

	uint16_t boot_architecture_flags;

	uint8_t reserved2;

	uint32_t flags;
	
	struct GenericAddressStructure reset_register;

	uint8_t reset_value;
	uint8_t reserved3[3];

	uint64_t x_firmware_control;
	uint64_t x_dsdt;
	
	struct GenericAddressStructure x_pm1a_event_block;
	struct GenericAddressStructure x_pm1b_event_block;
	struct GenericAddressStructure x_pm1a_control_block;
	struct GenericAddressStructure x_pm1b_control_block;
	struct GenericAddressStructure x_pm2_control_block;
	struct GenericAddressStructure x_pm_timer_block;
	struct GenericAddressStructure x_gpe0_block;
	struct GenericAddressStructure x_gpe1_block;

} __attribute__((packed)) * fadt = 0;

extern struct MADT* madt;

struct RSDT* dsdt = 0;

static uint32_t sdt_entry_count = 0;

__attribute__((const))
static bool checksum(uint32_t addr, uint32_t size)
{
	uint8_t sum = 0;

	for(uint32_t i = 0; i<size; ++i)
	{
		sum += ((uint8_t*)addr)[i];
	}

	return sum == 0;
}

__attribute__((pure))
void* find_sdt_table(uint32_t signature)
{
	if(!sdt_entry_count)
		return 0;

	for(uint32_t i = 0; i<sdt_entry_count; ++i)
	{
		struct RSDT* m = (void*) rsdt->sdt_pointer[i];
		if(*(uint32_t*)m->signature == signature)
		{
			return (void*) m;
		}
	}
	return 0;
}

bool init_acpi()
{
	//find rsdp
	//0xE0000 -> 0xFFFFF

	for(uint32_t addr = 0xE0000; addr < 0xFFFFF; addr+=16)
	{
		if(*(uint64_t*)(addr) == 0x2052545020445352)
		{
			if(checksum(addr, sizeof(struct RSDP)))
			{
				rsdp = (void*)addr;
				break;
			}
			if(checksum(addr, sizeof(struct RSDP) - sizeof(struct RSDP_2)))
			{
				rsdp = (void*)addr;
				break;
			}
		}
	}

	if(!rsdp)
	{
		uint32_t ebda = (uint32_t)(*(uint16_t*)0x40E) * 0x10;
		for(uint32_t addr = ebda; addr < (ebda + 1024); addr += 16)
		{
			if(*(uint64_t*)(addr) == 0x2052545020445352)
			{
				if(checksum(addr, sizeof(struct RSDP)))
				{
					rsdp = (void*) addr;
					break;
				}
				if(checksum(addr, sizeof(struct RSDP) - sizeof(struct RSDP_2)))
				{
					rsdp = (void*) addr;
					break;
				}
			}
		}

	}

	if(!rsdp) return false;

	rsdt = (rsdp->revision == 0) ? ((void*)(size_t)rsdp->rsdt_address) : ((void*)(size_t)rsdp->xsdt.xsdt_address);
	
	if(!checksum((uint32_t)rsdt, rsdt->rsdt.length)) return false;

	sdt_entry_count = (rsdt->rsdt.length - sizeof(struct RSDT)) / sizeof(void*);
	
	fadt = find_sdt_table(FADT);
	
	if(fadt)
	{
		dsdt = (void*)fadt->dsdt;
		
		printf("%s\n", AML_parse(dsdt));
	}
	
	madt = find_sdt_table(MADT);
	if(madt)
	{
		apic_init();
	}
	
	return true;

}
