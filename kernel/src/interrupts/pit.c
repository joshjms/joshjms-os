#include <interrupts/pit.h>

#include <arch/x86_64/io.h>
#include <arch/x86_64/pit.h>
#include <interrupts/interrupts.h>
#include <lib/log.h>

volatile uint64_t pitInteruptsTriggered = 0;

void pit_isr(struct registers *regs) {
    (void) regs;
    pitInteruptsTriggered++;
}
