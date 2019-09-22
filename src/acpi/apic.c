#include "apic.h"
#include "../console.h"
#include "../paging.h"
#include "../mheap.h"
// #include "../cpu/msr.h"
#include "../isr.h"
#include "../panic.h"
#include "../io.h"

#include "../cpu/cr.h"
#include "ioapic.h"

enum APIC_REGISTERS {
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

enum APIC_VALUES {
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

static uint32_t read_reg(uintptr_t offset) {
    return MmioRead32((void*) (apic.lapic_addr + offset));
}

static void write_reg(uintptr_t offset, uint32_t value) {
    MmioWrite32((void*) (apic.lapic_addr + offset), value);
}

void spurious_irq(struct registers* regs) {
    printf("spurious triggered\n");
    write_reg(APIC_EOI_REGISTER, 0);
    (void) regs;
}

void dummy_tmr(struct registers* regs) {
    printf("dummy?");
    write_reg(APIC_EOI_REGISTER, 0);
    (void) regs;
}

static uint32_t lapic_get_id(void) {
    return read_reg(APIC_APICID) >> 24;
}

extern void enable_apic(void);

static void LocalApicSendInit(uint8_t apic_id) {
    write_reg(APIC_INTERRUPT_COMMAND_HIGH_REGISTER, apic_id << 24);
    write_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER, ICR_INIT | ICR_PHYSICAL
        | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (read_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER) & ICR_SEND_PENDING);
}

static void lapic_startup(uint32_t apic_id, uint32_t vector) {
    write_reg(APIC_INTERRUPT_COMMAND_HIGH_REGISTER, apic_id << 24);
    write_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER, vector | ICR_STARTUP
        | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (read_reg(APIC_INTERRUPT_COMMAND_LOW_REGISTER) & ICR_SEND_PENDING);
}

void apic_init(struct MADT_SDT* madt) {
    // Enumerate all entries
    struct MADT_Entry* current_entry = &madt->entries;

    apic.lapic_addr = madt->lapic_addr;
    apic.flags = madt->flags;

    while((uintptr_t)current_entry < ((uintptr_t)madt + madt->sdt.length))
    {
        switch(current_entry->type)
        {
            case ProcessorLocalAPIC:
            {
                apic.cpu_apics[apic.cpu_count++] = &current_entry->lapic;
            } break;

            case IOAPIC:
            {
                apic.io_apics[apic.ioapic_count++] = &current_entry->io_apic;
            } break;

            case InterruptSourceOverride:
            {
                apic.interrupt_source_override[apic.interrupt_source_override_count++] = &current_entry->interrupt_source_override;
            } break;

            case NonMaskableInterrupts:
            {
                apic.non_maskable_interrupts[apic.non_maskable_interrupts_count++] = &current_entry->non_maskable_interrupt;
            } break;

            case LocalAPICAddressOverride:
            {
                apic.lapic_addr = current_entry->lapic_addr_override.address;
            } break;
        }
        
        current_entry = (struct MADT_Entry*)((uint32_t)current_entry + current_entry->length);
    }

    const uintptr_t virt_offset = mem_offset_get();
    const ptrdiff_t offset = map_page(apic.lapic_addr, virt_offset, Present | ReadWrite);

    apic.lapic_addr = virt_offset + offset;

    mem_offset_set(offset + 0x1000);
    
    __asm__ volatile("cli");
    pic_disable();
    
    
    wrmsr(MSR_APIC, (rdmsr(MSR_APIC) & 0xfffff000) | 0x800);
    
    write_reg(0xF0, read_reg(0xF0) | 0x1FF);

    

    __asm__ volatile("sti");
    
    ioapic_init(&apic);


}
