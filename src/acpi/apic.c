#include "apic.h"
#include "../console.h"
#include "../paging.h"
#include "../mheap.h"
// #include "../cpu/msr.h"
#include "../isr.h"
#include "../panic.h"
#include "../io.h"

enum APIC_REGISTERS
{
    APIC_APICID = 0x20,
    APIC_APICVER = 0x30,
    APIC_TASKPRIOR = 0x80,
    APIC_EOI_REGISTER = 0xB0,
    APIC_LDR = 0xD0,
    APIC_DFR = 0xE0,
    APIC_SPURIOUS_REGISTER = 0xF0,
    APIC_ESR = 0x280,
    APIC_INTERRUPT_COMMAND_LOW_REGISTER = 0x300,
    APIC_INTERRUPT_COMMAND_HIGH_REGISTER = 0x310,
    APIC_TIMER_REGISTER = 0x320,
    APIC_PERF = 0x340,
    APIC_LINT0_REGISTER = 0x350,
    APIC_LINT1_REGISTER = 0X360,
    APIC_LVT_ERROR = 0x370,
    APIC_TIMER_INIT_CNT = 0x380,
    APIC_TIMER_CURRENT_CNT = 0x390,
    APIC_TIMER_DIV = 0x3E0,
};

enum APIC_VALUES
{
    APIC_DISABLE = 0x10000,
    APIC_SW_ENABLE = 0x100,
    APIC_LAST = 0x38F,
    APIC_NMI = 4 << 8,
    APIC_CPU_FOCUS = 0x200,
    APIC_TIMER_PERIODIC = 0x20000,
    APIC_TIMER_BASE_DIV = 1 << 20,
};

#define ICR_FIXED                       0x00000000
#define ICR_LOWEST                      0x00000100
#define ICR_SMI                         0x00000200
#define ICR_NMI                         0x00000400
#define ICR_INIT                        0x00000500
#define ICR_STARTUP                     0x00000600

// Destination Mode
#define ICR_PHYSICAL                    0x00000000
#define ICR_LOGICAL                     0x00000800

// Delivery Status
#define ICR_IDLE                        0x00000000
#define ICR_SEND_PENDING                0x00001000

// Level
#define ICR_DEASSERT                    0x00000000
#define ICR_ASSERT                      0x00004000

// Trigger Mode
#define ICR_EDGE                        0x00000000
#define ICR_LEVEL                       0x00008000

// Destination Shorthand
#define ICR_NO_SHORTHAND                0x00000000
#define ICR_SELF                        0x00040000
#define ICR_ALL_INCLUDING_SELF          0x00080000
#define ICR_ALL_EXCLUDING_SELF          0x000c0000

struct APIC apic;

static uint32_t read_reg(uintptr_t offset)
{
    return MmioRead32((void*) (apic.lapic_addr + offset));
}

static void write_reg(uintptr_t offset, uint32_t value)
{
    MmioWrite32((void*) (apic.lapic_addr + offset), value);
}

void spurious_irq(struct registers* regs)
{
    printf("spurious triggered\n");
    write_reg(APIC_EOI_REGISTER, 0);
    (void) regs;
}

void dummy_tmr(struct registers* regs)
{
    printf("dummy?");
    write_reg(APIC_EOI_REGISTER, 0);
    (void) regs;
}

static uint32_t lapic_get_id(void)
{
    return read_reg(APIC_APICID) >> 24;
}

extern void enable_apic(void);

static void LocalApicSendInit(uint8_t apic_id)
{
    write_reg(APIC_INTERRUPT_COMMAND_HIGH_REGISTER, apic_id << 24);
    write_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER, ICR_INIT | ICR_PHYSICAL
        | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (read_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER) & ICR_SEND_PENDING)
        ;
}

static void LocalApicSendStartup(uint32_t apic_id, uint32_t vector)
{
    write_reg(APIC_INTERRUPT_COMMAND_HIGH_REGISTER, apic_id << 24);
    write_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER, vector | ICR_STARTUP
        | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (read_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER) & ICR_SEND_PENDING)
        ;
}

void initAPIC(struct MADT_SDT* madt)
{
    // Enumerate all entries
    struct MADT_Entry* current_entry = &madt->entries;

    apic.lapic_addr = madt->LocalApicAddress;
    apic.flags = madt->Flags;

    while((uintptr_t)current_entry < ((uintptr_t)madt + madt->SDT.Length))
    {
        switch(current_entry->Type)
        {
            case ProcessorLocalAPIC:
            {
                apic.cpu_apics[apic.cpu_count++] = &current_entry->ProcessorLocalAPIC;
            } break;

            case IOAPIC:
            {
                apic.ioapics[apic.ioapic_count++] = &current_entry->IOAPIC;
            } break;

            case InterruptSourceOverride:
            {
                apic.interrupt_source_override[apic.interrupt_source_override_count++] = &current_entry->InterruptSourceOverride;
            } break;

            case NonMaskableInterrupts:
            {
                apic.non_maskable_interrupts[apic.non_maskable_interrupts_count++] = &current_entry->NonMaskableInterrupts;
            } break;

            case LocalAPICAddressOverride:
            {
                apic.lapic_addr = current_entry->LocalAPICAddressOverride.address;
            } break;
        }
        
        current_entry = (struct MADT_Entry*)((uint32_t)current_entry + current_entry->Length);
    }

    const uintptr_t virt_offset = getCurrentVirtualMemoryOffset();
    const ptrdiff_t offset = map_page(apic.lapic_addr, virt_offset, Present | ReadWrite);

    apic.lapic_addr = virt_offset + offset;

    setCurrentVirtualMemoryOffset(offset + 0x1000);
    
    irq_install_handler(0, dummy_tmr);
    irq_install_handler(7, spurious_irq);

    write_reg(APIC_TASKPRIOR, 0);
    write_reg(APIC_DFR, 0xFFFFFFFF);
    write_reg(APIC_LDR, (read_reg(APIC_LDR) & 0xFFFFFF) | 1);
    
    write_reg(APIC_TIMER_REGISTER, APIC_DISABLE);
    write_reg(APIC_PERF, APIC_NMI);
    write_reg(APIC_LINT0_REGISTER, APIC_DISABLE);
    write_reg(APIC_LINT1_REGISTER, APIC_DISABLE);

    enable_apic();

    write_reg(APIC_SPURIOUS_REGISTER, 39 + APIC_SW_ENABLE);
    write_reg(APIC_TIMER_REGISTER, 32);
    write_reg(APIC_TIMER_DIV, 3);

    for(uint8_t i = 0; i < apic.cpu_count; ++i)
    {
        LocalApicSendStartup(apic.cpu_apics[i]->APICID, 12);
        //LocalApicSendInit(apic.cpu_apics[i]->APICID);
    }

}