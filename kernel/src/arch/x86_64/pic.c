#include <arch/x86_64/pic.h>

#include <stdint.h>

#include <arch/x86_64/io.h>

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_irq 2

#define MASTER_OFFSET 0x20
#define SLAVE_OFFSET 0x28

static void PIC_route_to_apic(void) {
    /*
     * If the IMCR is present, route legacy PIC interrupts to the APIC
     * (virtual-wire mode). This is required on most modern chipsets.
     */
    outb(0x22, 0x70);
    io_wait();
    uint8_t value = inb(0x23);
    outb(0x23, value | 0x01);
    io_wait();
}

void PIC_sendEOI(uint8_t irq) {
    if(irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

static void PIC_remap(int offset1, int offset2) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();
    outb(PIC1_DATA, (1 << CASCADE_irq));
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
}

void IRQ_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;
    
    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void IRQ_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;
    
    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

void PIC_init() {
    PIC_route_to_apic();

    outb(PIC1_DATA, 0xFF);
    io_wait();
    outb(PIC2_DATA, 0xFF);
    io_wait();
    PIC_remap(MASTER_OFFSET, SLAVE_OFFSET);
    outb(PIC1_DATA, 0xFF);
    io_wait();
    outb(PIC2_DATA, 0xFF);
    io_wait();
}

uint8_t PIC_get_mask_master(void) {
    return inb(PIC1_DATA);
}

uint8_t PIC_get_mask_slave(void) {
    return inb(PIC2_DATA);
}

static uint8_t PIC_read_irr(uint16_t command_port) {
    outb(command_port, 0x0A);
    io_wait();
    return inb(command_port);
}

static uint8_t PIC_read_isr(uint16_t command_port) {
    outb(command_port, 0x0B);
    io_wait();
    return inb(command_port);
}

uint8_t PIC_read_irr_master(void) {
    return PIC_read_irr(PIC1_COMMAND);
}

uint8_t PIC_read_irr_slave(void) {
    return PIC_read_irr(PIC2_COMMAND);
}

uint8_t PIC_read_isr_master(void) {
    return PIC_read_isr(PIC1_COMMAND);
}

uint8_t PIC_read_isr_slave(void) {
    return PIC_read_isr(PIC2_COMMAND);
}
