#include <interrupts/handlers.h>

#include <interrupts/interrupts.h>
#include <interrupts/timer.h>

void register_interrupt_handlers() {
    IRQ_register_handler(0, apic_timer_isr);
}
