#pragma once

#include <stdint.h>

void IDT_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

void IDT_init();
