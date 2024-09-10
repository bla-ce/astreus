global _start

; constants
BUFFER_MAX_LEN equ 8192
KEY_MAX_LEN equ 32
VALUE_MAX_LEN equ 32
ACTION_MAX_LEN equ 8

section .text
find_number_of_word:
  ; rdi -> buffer
  ; rsi -> buffer len
  ; return value: rax -> count 

  ; prepare stack
  sub   rsp, 0x10
  mov   qword [rsp], 0 ; count

find_number_word_loop:
  mov   al, byte [rdi]
  cmp   al, 32 ; compare char with space
  je    add_word

  mov   al, byte [rdi]
  cmp   al, 0
  je    end_string

  inc   rdi
  jmp   find_number_word_loop

add_word:
  inc   rdi
  inc   qword [rsp]

  jmp   find_number_word_loop

end_string:
  inc   qword [rsp]

return:
  mov   rax, qword [rsp]
  ; clean stack
  add   rsp, 0x10
  ret

find_next_char:
  ; rdi -> buffer
  ; rsi -> char
  ; return value: rax -> char pos (-1 if doesnt exist)
  sub   rsp, 0x8

  mov   qword [rsp], 0 ; index of the char

  mov   rbx, rsi
  lea   rsi, [rdi]

find_next_char_loop:
  cld
  lodsb 

  cmp   al, bl
  je    return_find_next_char

  cmp   al, 0
  je    char_not_found

  inc   qword [rsp]
  jmp   find_next_char_loop

char_not_found:
  mov   qword [rsp], -1

return_find_next_char:
  mov   rax, qword [rsp]
  add   rsp, 0x8
  ret

parse_query:
  ; rdi -> query
  ; rsi -> number of words
  sub   rsp, 0x10
  mov   qword [rsp+0x8], rsi
  
  push  rdi

  ; get action
  mov   rsi, 32 ; space
  call  find_next_char 

  cmp   rax, 0
  jl    error_parse_query

  cmp   rax, ACTION_MAX_LEN
  jg    error_parse_query

  mov   rcx, rax
  pop   rsi
  lea   rdi, [action]
  rep   movsb

  inc   rsi
  
  ; get key
  push  rsi
  lea   rdi, [rsi]  
  mov   rsi, 32

  cmp   qword [rsp+0x8], 2
  mov   rdx, 10
  cmove rsi, rdx ; if only two word, we expect the end of the line

  call  find_next_char

  cmp   rax, 0
  jl    error_parse_query

  cmp   rax, KEY_MAX_LEN
  jg    error_parse_query

  mov   rcx, rax
  pop   rsi
  lea   rdi, [key]
  rep   movsb
  
  cmp   qword [rsp+0x8], 3
  jl    return_parse_query

  inc   rsi

  ; get value if rsi == 3
  push  rsi
  lea   rdi, [rsi]  
  mov   rsi, 10 ; we are expecting new line at the end of the string
  call  find_next_char

  cmp   rax, 0
  jl    error_parse_query

  cmp   rax, VALUE_MAX_LEN
  jg    error_parse_query

  mov   rcx, rax
  pop   rsi
  lea   rdi, [value]
  rep   movsb

return_parse_query:
  add   rsp, 0x10
  mov   rax, 0
  ret

error_parse_query:
  add   rsp, 0x10
  mov   rax, -1
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

  ; parse
  lea   rdi, [buffer]
  mov   rsi, rax
  call  parse_query

  cmp   rax, 0
  jl    error

  b1:

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

