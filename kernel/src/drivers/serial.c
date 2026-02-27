#include <drivers/serial.h>

#include <arch/x86_64/io.h>

#define COM1 0x3F8

void serial_init(void) {
    outb(COM1 + 1, 0x00); // disable interrupts
    outb(COM1 + 3, 0x80); // enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03); // baud rate lo: 38400 baud
    outb(COM1 + 1, 0x00); // baud rate hi
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static int serial_is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20;
}

void serial_putchar(char c) {
    while (serial_is_transmit_empty() == 0);
    outb(COM1, c);
}

void serial_puthex(uint64_t num) {
    char buffer[17];
    for (int i = 0; i < 16; i++) {
        uint8_t digit = (num >> ((15 - i) * 4)) & 0xF;
        buffer[i] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
    }
    buffer[16] = '\0';
    serial_print(buffer);
}

void serial_print(const char *str) {
    while (*str) {
        serial_putchar(*str++);
    }
}

void serial_println(const char *str) {
    serial_print(str);
    serial_putchar('\n');
}
