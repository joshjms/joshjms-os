#pragma once

#include <stdint.h>

void LAPIC_init(uintptr_t lapic_base);

void LAPIC_timer_init(uint8_t vector, uint32_t ms);

void LAPIC_eoi(void);

uint32_t LAPIC_id(void);
