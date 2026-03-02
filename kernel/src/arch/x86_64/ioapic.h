#pragma once

#include <stdint.h>

void IOAPIC_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector);

void IOAPIC_mask(uint8_t irq);

void IOAPIC_unmask(uint8_t irq);

void IOAPIC_init(uintptr_t apic_base_addr);
