#pragma once

#include <stdint.h>

void PIC_sendEOI(uint8_t irq);

void IRQ_set_mask(uint8_t irq_line);

void IRQ_clear_mask(uint8_t irq_line);

void PIC_init();
