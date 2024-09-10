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

  ; clean stack
  pop   rsi
  pop   rdi
  add   rsp, 0x10

  mov   rax, -1
  ret


read_key:
  ; rdi -> key
  sub   rsp, 0x10

  mov   qword [rsp+0x8], 0

  push  rdi

  ; check if key exists
  lea   rdi, [data]

next_key:
  pop   rsi
  push  rsi
  mov   rcx, KEY_MAX_LEN
  rep   cmpsb

  jne   key_not_equal

  mov   rax, 1
  lea   rsi, [rdi]
  mov   rdx, VALUE_MAX_LEN
  mov   rdi, 1
  syscall

  pop   rsi

  add   rsp, 0x10
  ret

key_not_equal:
  inc   qword [rsp+0x8]
  
  mov   rbx, qword [record_count]
  cmp   qword [rsp+0x8], rbx
  je    key_does_not_exist

  ; move to the next key
  add   rdi, rcx
  sub   rdi, KEY_MAX_LEN
  add   rdi, VALUE_MAX_LEN

  jmp   next_key

key_does_not_exist:
  mov   rax, -1
  pop   rsi
  add   rsp, 0x10
  ret

update_key:
  ; rdi -> key
  ; rsi -> value
  sub   rsp, 0x18

  mov   qword [rsp+0x10], 0 ; count

  push  rsi
  push  rdi

  ; check if key exists
  lea   rdi, [data]

  update_next_key:
  pop   rsi
  push  rsi
  mov   rcx, KEY_MAX_LEN
  rep   cmpsb

  jne   update_key_not_equal

  pop   rsi ; pop the key
  pop   rsi ; pop the value
  mov   rcx, VALUE_MAX_LEN
  rep   movsb

  add   rsp, 0x18
  ret

  update_key_not_equal:
  inc   qword [rsp+0x10]

  mov   rbx, qword [record_count]
  cmp   qword [rsp+0x10], rbx
  je    update_key_does_not_exist

  ; move to the next key
  add   rdi, rcx
  sub   rdi, KEY_MAX_LEN
  add   rdi, VALUE_MAX_LEN

  jmp   update_next_key

  update_key_does_not_exist:
  mov   rax, -1
  pop   rsi
  add   rsp, 0x18
  ret

delete_key:
  ; rdi -> key
  sub   rsp, 0x10

  mov   qword [rsp+0x8], 0

  push  rdi

  ; check if key exists
  lea   rdi, [data]

delete_next_key:
  pop   rsi
  push  rsi
  mov   rcx, KEY_MAX_LEN
  rep   cmpsb

  jne   delete_key_not_equal

  add   rdi, rcx
  sub   rdi, KEY_MAX_LEN
  mov   al, 0
  mov   rcx, KEY_MAX_LEN
  rep   stosb

  mov   rcx, VALUE_MAX_LEN
  rep   stosb

  pop   rsi

  dec   qword [record_count]

  add   rsp, 0x10
  ret

delete_key_not_equal:
  inc   qword [rsp+0x8]
  
  mov   rbx, qword [record_count]
  cmp   qword [rsp+0x8], rbx
  je    delete_key_does_not_exist

  ; move to the next key
  add   rdi, rcx
  sub   rdi, KEY_MAX_LEN
  add   rdi, VALUE_MAX_LEN

  jmp   delete_next_key

delete_key_does_not_exist:
  mov   rax, -1
  pop   rsi
  add   rsp, 0x10
  ret

%endif
