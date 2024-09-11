%ifndef PARSE_INC
%define PARSE_INC

parse_query:
  ; rdi -> query
  ; rsi -> number of words
  sub   rsp, 0x10

  mov   [rsp], rdi
  mov   qword [rsp+0x8], rsi
  
  ; get action
  mov   rsi, SPACE ; space
  call  find_next_char 

  cmp   rax, 0
  jl    error_parse_query

  cmp   rax, ACTION_MAX_LEN
  jg    error_parse_query

  mov   rcx, rax
  lea   rax, [rsp]
  mov   rsi, [rax]
  lea   rdi, [action]
  rep   movsb

  inc   rsi
  mov   [rsp], rsi
  
  ; get key
  lea   rdi, [rsi]  
  mov   rsi, SPACE

  cmp   qword [rsp+0x8], 2
  mov   rdx, LINE_FEED
  cmove rsi, rdx ; if only two word, we expect the end of the line

  call  find_next_char

  lea   rdi, [ERR_INVALID_KEY]

  cmp   rax, 0
  jle   error_parse_query

  cmp   rax, KEY_MAX_LEN
  jg    error_parse_query

  mov   rcx, rax
  lea   rax, [rsp]
  mov   rsi, [rax]
  lea   rdi, [key]
  rep   movsb
  
  cmp   qword [rsp+0x8], 3
  jl    return_parse_query

  inc   rsi
  mov   [rsp], rsi

  ; get value if rsi == 3
  lea   rdi, [rsi]  
  mov   rsi, LINE_FEED ; we are expecting new line at the end of the string
  call  find_next_char

  lea   rdi, [ERR_INVALID_VALUE]

  cmp   rax, 0
  jle   error_parse_query

  cmp   rax, VALUE_MAX_LEN
  jg    error_parse_query

  mov   rcx, rax
  lea   rax, [rsp]
  mov   rsi, [rax]
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

%endif
