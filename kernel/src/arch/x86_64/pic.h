#pragma once

#include <stdint.h>

void PIC_eoi(uint8_t irq);

void IRQ_mask(uint8_t irq_line);

void IRQ_unmask(uint8_t irq_line);

void PIC_init();
