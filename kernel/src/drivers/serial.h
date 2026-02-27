#pragma once

#include <stdint.h>

void serial_init(void);

void serial_putchar(char c);

void serial_puthex(uint64_t num);

void serial_print(const char *str);

void serial_println(const char *str);
