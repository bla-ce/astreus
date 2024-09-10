%ifndef UTILS_INC
%define UTILS_INC

find_number_of_word:
  ; rdi -> buffer
  ; rsi -> buffer len
  ; return value: rax -> count 

  ; prepare stack
  sub   rsp, 0x10
  mov   qword [rsp], 0 ; count

find_number_word_loop:
  mov   al, byte [rdi]
  cmp   al, SPACE ; compare char with space
  je    add_word

  mov   al, byte [rdi]
  cmp   al, NULL_CHAR
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

  cmp   al, NULL_CHAR
  je    char_not_found

  inc   qword [rsp]
  jmp   find_next_char_loop

char_not_found:
  mov   qword [rsp], -1

return_find_next_char:
  mov   rax, qword [rsp]
  add   rsp, 0x8
  ret

get_length:
  ; rdi -> string
  ; return value: rax -> length
  mov   rcx, 0

get_length_loop:
  mov   al, byte [rdi]
  cmp   al, 0
  je    return_length

  inc   rdi
  inc   rcx
  jmp   get_length_loop

return_length:
  mov   rax, rcx
  ret

str_equal:
  ; rdi -> string 1
  ; rsi -> string 2
  sub   rsp, 0x8

  push  rdi
  call  get_length

  mov   rcx, rax
  pop   rdi

  rep   cmpsb
  jne   str_not_equal

  mov   rax, 1
  add   rsp, 0x8
  ret

str_not_equal:
  mov   rax, -1
  add   rsp, 0x8
  ret

contains:
  ; rdi -> string
  ; rsi -> substring
  sub   rsp, 0x18
  mov   qword [rsp+0x8], 0 ; string index
  mov   qword [rsp+0x10], 0 ; sub pos

  push  rdi
  lea   rdi, [rsi]
  call  get_length
  mov   rcx, rax

  pop   rdi

cmp_loop:
  rep   cmpsb

  jne   not_equal

  jmp   found

not_equal:
  sub   rsi, rax
  add   rsi, rcx

  add   qword [rsp+0x8], rax
  sub   qword [rsp+0x8], rcx

  mov   rbx, qword [rsp+0x8]
  mov   rdx, qword [rsp+0x10]
  cmp   rbx, rdx
  jge   not_found

  jmp   cmp_loop

not_found:
  mov   rax, -1
  add   rsp, 0x18
  ret

found:
  mov   rax, qword [rsp+0x10]
  add   rsp, 0x18
  ret

%endif

