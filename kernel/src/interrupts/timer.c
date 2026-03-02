#include <interrupts/timer.h>

#include <arch/x86_64/apic.h>

void apic_timer_isr(struct registers *regs) {
    (void)regs;
    
    LAPIC_eoi();
}
