#pragma once

#include <stdbool.h>
#include <stdint.h>

/* x2APIC MSR addresses */
#define APIC_MSR_ID            0x802
#define APIC_MSR_VERSION       0x803
#define APIC_MSR_TPR           0x808   /* Task Priority */
#define APIC_MSR_EOI           0x80B   /* End of Interrupt */
#define APIC_MSR_SPURIOUS      0x80F
#define APIC_MSR_ESR           0x828   /* Error Status */
#define APIC_MSR_ICR           0x830   /* Interrupt Command (64-bit) */
#define APIC_MSR_LVT_TIMER     0x832
#define APIC_MSR_LVT_LINT0     0x835
#define APIC_MSR_LVT_LINT1     0x836
#define APIC_MSR_LVT_ERROR     0x837
#define APIC_MSR_TIMER_INITCNT 0x838
#define APIC_MSR_TIMER_CURRCNT 0x839
#define APIC_MSR_TIMER_DIV     0x83E

/* Spurious vector register bits */
#define APIC_SPURIOUS_VECTOR   0xFF
#define APIC_SPURIOUS_ENABLE   (1 << 8)

/* LVT flags */
#define APIC_LVT_MASKED        (1 << 16)
#define APIC_TIMER_PERIODIC    (1 << 17)
#define APIC_TIMER_DIV_16      0x3

/* LVT delivery modes */
#define APIC_LVT_EXTINT        (7 << 8)
#define APIC_LVT_NMI           (4 << 8)

/* ICR flags */
#define APIC_ICR_FIXED         0x00000000
#define APIC_ICR_ASSERT        (1 << 14)
#define APIC_ICR_EDGE          0x00000000

bool     APIC_check(void);
uintptr_t APIC_get_base(void);
void     APIC_set_base(uintptr_t apic);
void     APIC_enable(void);
void     APIC_send_eoi(void);
void     APIC_send_ipi(uint32_t dest, uint8_t vector);
