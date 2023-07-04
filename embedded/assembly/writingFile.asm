%include "macros.inc"

section .data
    fileName db "someFile.txt"
    text db "text"

section .text
    global _start

_start: 
    call _createFile
    call _writeFile
    call _closeFile

    exit

_createFile:
    mov rax, SYS_OPEN
    mov rdi, fileName
    mov rsi, O_CREAT+O_WRONLY
    mov rdx, 0644o
    syscall
    ret

_writeFile: 
    push rax
    mov rdi, rax
    mov rax, SYS_WRITE
    mov rsi, text
    mov rdx, 4
    syscall
    ret

_closeFile:
    mov rax, SYS_CLOSE
    pop rdi
    syscall
    ret