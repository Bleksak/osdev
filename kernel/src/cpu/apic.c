#include "cpu/apic.h"
#include "isr.h"
#include "cpu/cpu.h"
#include "cpu/msr.h"
#include "acpi.h"
#include "console.h"

#include "pit.h"
#include "idt.h"

struct MADT* madt = 0;

static struct Local_APIC* lapic[32];
static uint16_t lapic_entries = 0;

static struct IO_APIC* io_apic[32];
static uint16_t io_apic_entries = 0;

static struct InterruptSourceOverride* interrupt_source_override[32];
static uint16_t interrupt_source_override_entries = 0;

static struct NMI* nmi[32];
static uint16_t nmi_entries;

static struct LAPIC_Override* lapic_override = 0;

static uint32_t local_apic_address = 0;

bool enable_cpu(uint8_t id)
{
	if(lapic[id]->flags & 1)
		return true;
	
	

	return true;
}

void apic_init()
{
	pic_disable();
	struct MP_Entry* current_entry = &madt->entries;
	
	bool lapic_override_duplicate = false;

	while(current_entry->record_length)
	{
		switch(current_entry->type)
		{
			case 0:
			{
				if(current_entry->record_length != sizeof(struct Local_APIC) + sizeof(struct MP_Entry))
					goto done;
				
				lapic[lapic_entries++] = (void*)current_entry;

			} break;
			case 1:
			{
				if(current_entry->record_length != sizeof(struct IO_APIC) + sizeof(struct MP_Entry))
					goto done;
				
				io_apic[io_apic_entries++] = (void*) current_entry;

			} break;
			case 2:
			{
				if(current_entry->record_length != sizeof(struct InterruptSourceOverride) 
					+ sizeof(struct MP_Entry))
					goto done;

				interrupt_source_override[interrupt_source_override_entries++] = (void*) current_entry;

			} break;
			case 4:
			{
				if(current_entry->record_length != sizeof(struct NMI) + sizeof(struct MP_Entry))
					goto done;
				
				nmi[nmi_entries++] = (void*) current_entry;
				
			} break;
			case 5:
			{
				if(current_entry->record_length != sizeof(struct LAPIC_Override) + sizeof(struct MP_Entry))
					goto done;
				
				if(lapic_override_duplicate)
					goto done;

				lapic_override_duplicate = true;
				lapic_override = (void*) current_entry;
			} break;

			default: goto done;
		}
		current_entry = ((void*) (uint32_t)current_entry + current_entry->record_length);
	}

	done:
	
	local_apic_address = (lapic_override) ? lapic_override->local_apic_addr : madt->local_apic_addr;
	
	if(!local_apic_address)
	{
		uint32_t eax, edx;
		cpu_get_msr(MSR_APIC_BASE_REGISTER, &eax, &edx);

		local_apic_address = eax & 0xfffff000;
	}

	// if PAE is enabled do something..... // osdev wiki
	
	cpu_set_msr(MSR_APIC_BASE_REGISTER, (local_apic_address & 0xfffff000) | MSR_APIC_ENABLE , 0);
	

	enable_cpu(0);
	enable_cpu(1);
	enable_cpu(2);
	enable_cpu(3);

	printf("End");

}
