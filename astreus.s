; TODO: 'DELETE ' is valid

global _start

%include "utils.inc"
%include "parse.inc"

; constants
BUFFER_MAX_LEN equ 8192
KEY_MAX_LEN equ 32
VALUE_MAX_LEN equ 32
ACTION_MAX_LEN equ 8

; ascii
NULL_CHAR equ 0
LINE_FEED equ 10
SPACE     equ 32

; valid action
CREATE db "CREATE", NULL_CHAR
READ   db "READ", NULL_CHAR
UPDATE db "UPDATE", NULL_CHAR
DELETE db "DELETE", NULL_CHAR

section .text
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

  ; parse
  lea   rdi, [buffer]
  mov   rsi, rax
  call  parse_query

  cmp   rax, 0
  jl    error

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

  key     resb KEY_MAX_LEN
  value   resb VALUE_MAX_LEN
  action  resb ACTION_MAX_LEN

section .data

