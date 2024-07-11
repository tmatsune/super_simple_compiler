section .data
    hello db 'Hello, World!', 0

section .text
    global _start

_start:
    ; Write "Hello, World!" to stdout
    mov x0, 1                ; file descriptor: stdout
    ldr x1, =hello           ; address of hello
    mov x2, 13               ; buffer length
    mov x8, 64               ; syscall number for write
    svc 0                    ; make syscall

    ; Exit the program
    mov x8, 93               ; syscall number for exit
    mov x0, 0                ; status: 0
    svc 0                    ; make syscall
