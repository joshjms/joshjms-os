#pragma once

#include <stdint.h>

void PIC_sendEOI(uint8_t irq);

void IRQ_set_mask(uint8_t irq_line);

void IRQ_clear_mask(uint8_t irq_line);

void PIC_init();

uint8_t PIC_get_mask_master(void);
uint8_t PIC_get_mask_slave(void);

uint8_t PIC_read_irr_master(void);
uint8_t PIC_read_irr_slave(void);
uint8_t PIC_read_isr_master(void);
uint8_t PIC_read_isr_slave(void);
