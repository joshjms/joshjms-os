#include <interrupts/interrupts.h>

#include <arch/x86_64/pic.h>
#include <drivers/serial.h>

static const char *exception_names[] = {
    "Division by Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point",
};

__attribute__((noreturn))
void exception_handler(struct registers *regs) {
    serial_print("Exception: ");
    if (regs->vector < sizeof(exception_names) / sizeof(exception_names[0])) {
        serial_print(exception_names[regs->vector]);
    } else {
        serial_puthex(regs->vector);
    }
    serial_print(" at rip=");
    serial_puthex(regs->rip);
    serial_print(" error=");
    serial_puthex(regs->error_code);
    serial_putchar('\n');
    
    __asm__ volatile("cli; hlt");
}

static void (*irq_handlers[256])(struct registers *);

void IRQ_register_handler(int irq, void (*handler)(struct registers *)) {
    irq_handlers[irq] = handler;
    IRQ_clear_mask(irq);
}

void irq_dispatch(struct registers *regs) {
    int irq = regs->vector - 32;
    if (irq_handlers[irq])
        irq_handlers[irq](regs);
    else {
        serial_print("Unhandled IRQ: ");
        serial_puthex(irq);
        serial_putchar('\n');

        exception_handler(regs);
    }
    PIC_sendEOI(irq);
}
