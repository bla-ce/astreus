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

  lea   rdi, [action]
  lea   rsi, [CREATE]
  call  str_equal

  cmp   rax, 1
  je    create

  lea   rdi, [action]
  lea   rsi, [READ]
  call  str_equal

  cmp   rax, 1
  je    read

  lea   rdi, [action]
  lea   rsi, [UPDATE]
  call  str_equal

  cmp   rax, 1
  je    update

  lea   rdi, [action]
  lea   rsi, [DELETE]
  call  str_equal

  cmp   rax, 1
  je    delete

  jmp   next_query

create:
  mov   rax, 1
  mov   rdi, 1
  lea   rsi, [CREATE]
  mov   rdx, 6
  syscall

  jmp   next_query

read:
  mov   rax, 1
  mov   rdi, 1
  lea   rsi, [READ]
  mov   rdx, 4
  syscall

  jmp   next_query
  
update:
  mov   rax, 1
  mov   rdi, 1
  lea   rsi, [UPDATE]
  mov   rdx, 7
  syscall

  jmp   next_query

delete:
  mov   rax, 1
  mov   rdi, 1
  lea   rsi, [DELETE]
  mov   rdx, 7
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

