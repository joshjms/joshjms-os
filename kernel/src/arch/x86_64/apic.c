#include <arch/x86_64/apic.h>
#include <arch/x86_64/cpuid.h>

#include <cpuid.h>
#include <stdbool.h>
#include <stdint.h>

#define IA32_APIC_BASE_MSR        0x1B
#define IA32_APIC_BASE_MSR_ENABLE  (1 << 11)
#define IA32_APIC_BASE_MSR_X2APIC (1 << 10)

static inline uint64_t apic_rdmsr(uint32_t msr) {
    uint32_t lo, hi;
    __asm__ volatile ("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
    return ((uint64_t)hi << 32) | lo;
}

static inline void apic_wrmsr(uint32_t msr, uint64_t val) {
    __asm__ volatile ("wrmsr" : : "c"(msr), "a"((uint32_t)val), "d"((uint32_t)(val >> 32)));
}

bool APIC_check(void) {
    uint32_t eax, ebx, ecx, edx;
    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx))
        return false;
    return (edx & CPUID_FEAT_EDX_APIC) && (ecx & CPUID_FEAT_ECX_X2APIC);
}

uintptr_t APIC_get_base(void) {
    return (uintptr_t)(apic_rdmsr(IA32_APIC_BASE_MSR) & 0xFFFFF000);
}

void APIC_set_base(uintptr_t apic) {
    uint64_t val = apic_rdmsr(IA32_APIC_BASE_MSR);
    val = (val & ~(uint64_t)0xFFFFF000) | (apic & 0xFFFFF000);
    apic_wrmsr(IA32_APIC_BASE_MSR, val);
}

void APIC_enable(void) {
    /* Enable xAPIC then x2APIC (must be done in two steps per Intel SDM) */
    uint64_t base = apic_rdmsr(IA32_APIC_BASE_MSR);
    apic_wrmsr(IA32_APIC_BASE_MSR, base | IA32_APIC_BASE_MSR_ENABLE);
    apic_wrmsr(IA32_APIC_BASE_MSR, base | IA32_APIC_BASE_MSR_ENABLE | IA32_APIC_BASE_MSR_X2APIC);

    apic_wrmsr(APIC_MSR_TPR,      0);
    apic_wrmsr(APIC_MSR_SPURIOUS, APIC_SPURIOUS_ENABLE | APIC_SPURIOUS_VECTOR);
    apic_wrmsr(APIC_MSR_LVT_LINT0, APIC_LVT_EXTINT);   /* route PIC through LINT0 */
    apic_wrmsr(APIC_MSR_LVT_LINT1, APIC_LVT_MASKED);   /* mask LINT1 */
}

void APIC_send_eoi(void) {
    apic_wrmsr(APIC_MSR_EOI, 0);
}

void APIC_send_ipi(uint32_t dest, uint8_t vector) {
    /* x2APIC ICR is a single 64-bit write; destination in bits 63:32 */
    apic_wrmsr(APIC_MSR_ICR, ((uint64_t)dest << 32) | vector);
}
