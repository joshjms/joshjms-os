#include <interrupts/handlers.h>

#include <interrupts/interrupts.h>
#include <interrupts/pit.h>

void register_interrupt_handlers() {
    IRQ_register_handler(0, pit_isr);
}
