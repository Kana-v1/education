section .bss
    digitSpace resb 100 
    digitSpacePos resb 8

section .text
    global _start

_start:
    mov rax, 123
    call _printRAX

    mov rax, 60
    mov rdi, 0
    syscall

_printRAX:
    mov rcx, digitSpace
    mov rbx, 10
    mov [rcx], rbx
    inc rcx
    mov [digitSpacePos], rcx

_printRAXLoop:
    mov rbx, 10
    mov rdx, 0
    div rbx
    push rax
    add rdx, 48 ; remainder(3 in this case, 123 % 10 = 3 ) remains in the rdx, so we add 48 to convert the remainder to the charecter

    mov rcx, [digitSpacePos]
    mov [rcx], dl
    inc rcx
    mov [digitSpacePos], rcx

    pop rax
    cmp rax, 0 
    jne _printRAXLoop

_printRAXLoop2: 
    mov rcx, [digitSpacePos]

    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [digitSpacePos]
    dec rcx
    mov [digitSpacePos], rcx

    cmp rcx, digitSpace
    jge _printRAXLoop2

    ret