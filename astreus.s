global _start

; constants
BUFFER_MAX_LEN equ 8192

section .text
find_number_of_word:
  ; rdi -> buffer
  ; rsi -> buffer len
  ; return value: rax -> count 

  ; prepare stack
  sub   rsp, 0x10
  mov   qword [rsp], 0 ; count

loop:
  mov   al, byte [rdi]
  cmp   al, 32 ; compare char with space
  je    add_word

  mov   al, byte [rdi]
  cmp   al, 0
  je    end_string

  inc   rdi
  jmp   loop

add_word:
  inc   rdi
  inc   qword [rsp]

  jmp   loop

end_string:
  inc   qword [rsp]

return:
  mov   rax, qword [rsp]
  ; clean stack
  add   rsp, 0x10
  ret

_start:
next_query:
  ; read user input
  mov   rax, 0
  mov   rdi, 0 ; stdin
  lea   rsi, [buffer]
  mov   rdx, BUFFER_MAX_LEN
  syscall

  cmp   rax, 0
  jl    error

  mov   qword [buffer_len], rax

  ; split
  lea   rdi, [buffer]
  mov   rsi, qword [buffer_len]
  call  find_number_of_word

  cmp   rax, 3
  jg    next_query

  cmp   rax, 1
  jle   next_query 

  mov   rax, 1
  mov   rdi, 1
  lea   rsi, [buffer]
  mov   rdx, qword [buffer_len]
  syscall

  jmp   next_query

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

