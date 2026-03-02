#include <arch/x86_64/ioapic.h>

#include <stdint.h>

#define IOAPICID        0x00
#define IOAPICVER       0x01
#define IOAPICARB       0x02
#define IOAPICREDTBL(n) (0x10 + 2 * n)

static uintptr_t apic_base;

static void IOAPIC_write(const uint8_t offset, const uint32_t val) {
    // Tell IOREGSEL where we want to write to
    *(volatile uint32_t *)(apic_base) = offset;
    // Write the value to IOREGWIN
    *(volatile uint32_t *)(apic_base + 0x10) = val;
}

static uint32_t IOAPIC_read(const uint8_t offset) {
    *(volatile uint32_t *)(apic_base) = offset;
    return *(volatile uint32_t *)(apic_base + 0x10);
}

void IOAPIC_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector) {
    uint32_t low  = vector;             // interrupt vector (0x20–0xFF)
    uint32_t high = apic_id << 24;      // destination Local APIC ID

    IOAPIC_write(IOAPICREDTBL(irq), low);
    IOAPIC_write(IOAPICREDTBL(irq) + 1, high);
}

void IOAPIC_mask(uint8_t irq) {
    uint32_t low = IOAPIC_read(IOAPICREDTBL(irq));

    IOAPIC_write(IOAPICREDTBL(irq), low | (1 << 16));
}

void IOAPIC_unmask(uint8_t irq) {
    uint32_t low = IOAPIC_read(IOAPICREDTBL(irq));

    IOAPIC_write(IOAPICREDTBL(irq), low & ~(1 << 16));
}

void IOAPIC_init(uintptr_t apic_base_addr) {
    apic_base = apic_base_addr;

    uint32_t max_irq = (IOAPIC_read(IOAPICVER) >> 16) & 0xFF;

    for(uint8_t i = 0; i <= max_irq; i++) {
        IOAPIC_mask(i);
    }
}
