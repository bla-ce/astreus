%ifndef ACTION_INC
%define ACTION_INC

create_key:
  ; rdi -> key
  ; rsi -> value
  sub   rsp, 0x10

  mov   [rsp], rdi
  mov   [rsp+0x8], rsi

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
  lea   rax, [rsp]
  mov   rsi, [rax]
  mov   rcx, KEY_MAX_LEN
  rep   movsb

  ; load value
  lea   rax, [rsp+0x8]
  mov   rsi, [rax]
  mov   rcx, VALUE_MAX_LEN
  rep   movsb

  add   rsp, 0x10
  mov   rax, 0
  ret

no_space:
  dec   qword [record_count]

  lea   rdi, [ERR_DATA_FULL]

  ; clean stack
  add   rsp, 0x10

  mov   rax, -1
  ret

read_key:
  ; rdi -> key
  sub   rsp, 0x10 ; key and counter

  mov   [rsp], rdi
  mov   qword [rsp+0x8], 0 ; counter

  ; check if key exists
  lea   rdi, [data]

next_key:
  lea   rax, [rsp]
  mov   rsi, [rax]
  mov   rcx, KEY_MAX_LEN
  rep   cmpsb

  jne   key_not_equal

  mov   rax, 1
  lea   rsi, [rdi]
  mov   rdx, VALUE_MAX_LEN
  mov   rdi, 1
  syscall

  mov   rax, 1
  lea   rsi, [STR_NEW_LINE]
  mov   rdi, 1
  mov   rdx, 1
  syscall

  mov   rax, 0

  add   rsp, 0x10
  ret

key_not_equal:
  inc   qword [rsp+0x8]
  
  mov   rax, qword [rsp+0x8]
  cmp   rax, qword [record_count]
  jg    key_does_not_exist

  ; move to the next key
  add   rdi, rcx
  add   rdi, VALUE_MAX_LEN

  jmp   next_key

key_does_not_exist:
  lea   rdi, [ERR_KEY_NOT_EXIST]

  mov   rax, -1
  add   rsp, 0x10
  ret

update_key:
  ; rdi -> key
  ; rsi -> value
  sub   rsp, 0x18 ; key, value and counter

  mov   [rsp], rdi
  mov   [rsp+0x8], rsi
  mov   qword [rsp+0x10], 0

  ; check if key exists
  lea   rdi, [data]

update_next_key:
  lea   rax, [rsp]
  mov   rsi, [rax]
  mov   rcx, KEY_MAX_LEN
  rep   cmpsb

  jne   update_key_not_equal

  lea   rax, [rsp+0x8]
  mov   rsi, [rax]
  mov   rcx, VALUE_MAX_LEN
  rep   movsb

  mov   rax, 0

  add   rsp, 0x18
  ret

update_key_not_equal:
  inc   qword [rsp+0x10]

  mov   rax, qword [rsp+0x10]
  cmp   rax, qword [record_count]
  jg    update_key_does_not_exist

  ; move to the next key
  add   rdi, rcx
  add   rdi, VALUE_MAX_LEN

  jmp   update_next_key

update_key_does_not_exist:
  lea   rdi, [ERR_KEY_NOT_EXIST]

  mov   rax, -1
  add   rsp, 0x18
  ret

delete_key:
  ; rdi -> key
  sub   rsp, 0x10 ; key, counter

  mov   [rsp], rdi
  mov   qword [rsp+0x8], 0

  ; check if key exists
  lea   rdi, [data]

delete_next_key:
  lea   rax, [rsp]
  mov   rsi, [rax]
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

  dec   qword [record_count]

  mov   rax, 0

  add   rsp, 0x10
  ret

delete_key_not_equal:
  inc   qword [rsp+0x8]

  mov   rax, qword [rsp+0x8]
  cmp   rax, qword [record_count]
  jg    delete_key_does_not_exist

  ; move to the next key
  add   rdi, rcx
  add   rdi, VALUE_MAX_LEN

  jmp   delete_next_key

delete_key_does_not_exist:
  lea   rdi, [ERR_KEY_NOT_EXIST]

  mov   rax, -1
  add   rsp, 0x10
  ret

%endif
