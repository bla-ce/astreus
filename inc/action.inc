%ifndef ACTION_INC
%define ACTION_INC

create_key:
  ; rdi -> key
  ; rsi -> value
  sub   rsp, 0x10

  push  rsi
  push  rdi

  ; go to next record
  mov   rax, DATA_STRUCT_LEN
  mov   rbx, qword [record_count]
  mul   rbx

  ; increase record count
  inc   qword [record_count]

  cmp   qword [record_count], DATA_MAX_COUNT
  jg    no_space

  ; load key
  lea   rdi, [data]
  add   rdi, rax
  pop   rsi
  mov   rcx, KEY_MAX_LEN
  rep   movsb

  ; load value
  pop   rsi
  mov   rcx, VALUE_MAX_LEN
  rep   movsb

  add   rsp, 0x10
  mov   rax, 0
  ret

no_space:
  dec   qword [record_count]
  pop   rsi
  pop   rdi
  add   rsp, 0x10
  mov   rax, -1
  ret

read_key:
  ret

update_key:
  ret

delete_key:
  ret

%endif
