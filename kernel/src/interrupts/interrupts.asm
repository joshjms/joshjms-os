%macro exception_no_err_stub 1
isr_stub_%+%1:
    push qword 0
    push qword %1
    jmp exception_common
%endmacro

%macro exception_err_stub 1
isr_stub_%+%1:
    push qword %1
    jmp exception_common
%endmacro

%macro irq_stub 1
isr_stub_%+%1:
    push qword 0
    push qword %1
    jmp interrupt_common
%endmacro

exception_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call exception_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16
    iretq

interrupt_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call interrupt_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16
    iretq

extern exception_handler
extern interrupt_handler

exception_no_err_stub 0
exception_no_err_stub 1
exception_no_err_stub 2
exception_no_err_stub 3
exception_no_err_stub 4
exception_no_err_stub 5
exception_no_err_stub 6
exception_no_err_stub 7
exception_err_stub    8
exception_no_err_stub 9
exception_err_stub    10
exception_err_stub    11
exception_err_stub    12
exception_err_stub    13
exception_err_stub    14
exception_no_err_stub 15
exception_no_err_stub 16
exception_err_stub    17
exception_no_err_stub 18
exception_no_err_stub 19
exception_no_err_stub 20
exception_no_err_stub 21
exception_no_err_stub 22
exception_no_err_stub 23
exception_no_err_stub 24
exception_no_err_stub 25
exception_no_err_stub 26
exception_no_err_stub 27
exception_no_err_stub 28
exception_no_err_stub 29
exception_err_stub    30
exception_no_err_stub 31

irq_stub 32
irq_stub 33
irq_stub 34
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47

global isr_stub_255
irq_stub 255

global isr_stub_table
isr_stub_table:
%assign i 0
%rep 48
    dq isr_stub_%+i
%assign i i+1
%endrep
