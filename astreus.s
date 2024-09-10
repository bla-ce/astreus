global _start

; constants
BUFFER_MAX_LEN equ 8192

section .text
_start:
  ; read user input
  mov   rax, 0
  mov   rdi, 0 ; stdin
  lea   rsi, [buffer]
  mov   rdx, BUFFER_MAX_LEN
  syscall

  cmp   rax, 0
  jl    error

  mov   qword [buffer_len], rax

  ; print it back
  mov   rax, 1
  mov   rdi, 1 ; stdout
  lea   rsi, [buffer]
  mov   rdx, qword [buffer_len]
  syscall

  ; exit program
  mov   rax, 60
  mov   rdi, 1
  syscall

error:
  mov   rax, 60
  mov   rdi, -1
  syscall

section .bss
  buffer resb BUFFER_MAX_LEN

  buffer_len resq 1

section .data

