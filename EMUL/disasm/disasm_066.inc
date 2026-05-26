;══════════════════════
;  1a) xxx word ptr [ereg],word              │ 066HEX, 081HEX, 00XXXerg,     word
;  1b) xxx word ptr [esp],word               │ 066HEX, 081HEX, 00XXX100,   024HEX,  word
;  1c) xxx word ptr [erega+eregb],word       │ 066HEX, 081HEX, 00XXX100, 00rgBrgA,  word
;  1d) xxx word ptr [dword],word             │ 066HEX, 081HEX, 00XXX101,    dword,  word
;
;      xxx word ptr [ereg*2],word            │ 066HEX, 081HEX, 00XXX100, 01reg101, 00000000, word
;      xxx word ptr [ereg*2+dword],word      │ 066HEX, 081HEX, 00XXX100, 01reg101,    dword, word
;      xxx word ptr [ereg*4],word            │ 066HEX, 081HEX, 00XXX100, 10reg101, 00000000, word
;      xxx word ptr [ereg*4+dword],word      │ 066HEX, 081HEX, 00XXX100, 10reg101,    dword, word
;      xxx word ptr [ereg*8],word            │ 066HEX, 081HEX, 00XXX100, 11reg101, 00000000, word
;      xxx word ptr [ereg*8+dword],word      │ 066HEX, 081HEX, 00XXX100, 11reg101,    dword, word
;
;      xxx word ptr [erega+eregb*2],word     │ 066HEX, 081HEX, 00XXX100, 01rgBrgA,  word
;      xxx word ptr [erega+eregb*4],word     │ 066HEX, 081HEX, 00XXX100, 10rgBrgA,  word
;      xxx word ptr [erega+eregb*8],word     │ 066HEX, 081HEX, 00XXX100, 11rgBrgA,  word
;
;  1e) xxx word ptr [erega+eregb+byte],word  │ 066HEX, 081HEX, 01XXX101,     byte,  word
;  1f) xxx word ptr [ebp],word               │ 066HEX, 081HEX, 01XXX101,   000HEX,  word
;  1g) xxx word ptr [ebp+byte],word          │ 066HEX, 081HEX, 01XXX101,     byte,  word
;
;  1h) xxx word ptr [ereg+dword],word        │ 066HEX, 081HEX, 10XXXreg,    dword
;  1i) xxx word ptr [esp+dword],word         │ 066HEX, 081HEX, 10XXX100,   024HEX, dword, word
;  1j) xxx word ptr [erega+eregb+dword],word │ 066HEX, 081HEX, 10XXX101,    dword,  word
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB,
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
;  По составу соответствует "xxx byte ptr [...],byte" (080h),
; только на 2 байта больше
;
;   2) add reg,word (16 bit)             │ 066HEX,   081HEX,  11000reg,   word
;      or  reg,word                      │ 066HEX,   081HEX,  11001reg,   word
;      adc reg,word                      │ 066HEX,   081HEX,  11010reg,   word
;      sbb reg,word                      │ 066HEX,   081HEX,  11011reg,   word
;      and reg,word                      │ 066HEX,   081HEX,  11100reg,   word
;      sub reg,word                      │ 066HEX,   081HEX,  11101reg,   word
;      xor reg,word                      │ 066HEX,   081HEX,  11110reg,   word
;      cmp reg,word                      │ 066HEX,   081HEX,  11111reg,   word
;
;  2b) add ax,word  (16 bit)             │ 066HEX,   005HEX,      word 
;      or  ax,word                       │ 066HEX,   00DHEX,      word
;      adc ax,word                       │ 066HEX,   015HEX,      word
;      sbb ax,word                       │ 066HEX,   01DHEX,      word
;      and ax,word                       │ 066HEX,   025HEX,      word
;      sub ax,word                       │ 066HEX,   02DHEX,      word
;      xor ax,word                       │ 066HEX,   035HEX,      word
;      cmp ax,word                       │ 066HEX,   03DHEX,      word
;
;   3) add reg,reg (16 bit)              │ 066HEX,   001HEX,  11rgArgB
;                                        │ 066HEX,   003HEX,  11rgArgB
;      or  reg,reg                       │ 066HEX,   009HEX,  11rgArgB
;                                        │ 066HEX,   00BHEX,  11rgArgB
;      adc reg,reg                       │ 066HEX,   011HEX,  11rgArgB
;                                        │ 066HEX,   013HEX,  11rgArgB
;      sbb reg,reg                       │ 066HEX,   019HEX,  11rgArgB
;                                        │ 066HEX,   01BHEX,  11rgArgB
;      and reg,reg                       │ 066HEX,   021HEX,  11rgArgB
;                                        │ 066HEX,   023HEX,  11rgArgB
;      sub reg,reg                       │ 066HEX,   029HEX,  11rgArgB
;                                        │ 066HEX,   02BHEX,  11rgArgB
;      xor reg,reg                       │ 066HEX,   031HEX,  11rgArgB
;                                        │ 066HEX,   033HEX,  11rgArgB
;      cmp reg,reg                       │ 066HEX,   039HEX,  11rgArgB
;                                        │ 066HEX,   03BHEX,  11rgArgB
;      mov reg,reg                       │ 066HEX,   089HEX,  11rgArgB
;                                        │ 066HEX,   08BHEX,  11rgArgB
;
;      cmpsw                             │ 066HEX,   0A7HEX  
;      stosw                             │ 066HEX,   0ABHEX  
;      lodsw                             │ 066HEX,   0ADHEX
;
;   4) rol reg,1 (16 bit)                │ 066HEX,   0C1HEX,  11000reg, byte
;      ror reg,1                         │ 066HEX,   0C1HEX,  11001reg, byte
;      rcl reg,1                         │ 066HEX,   0C1HEX,  11010reg, byte
;      rcr reg,1                         │ 066HEX,   0C1HEX,  11011reg, byte
;      shl reg,1                         │ 066HEX,   0C1HEX,  11100reg, byte
;      shr reg,1                         │ 066HEX,   0C1HEX,  11101reg, byte
;      sal reg,1                         │ 066HEX,   0C1HEX,  11110reg, byte
;      sar reg,1                         │ 066HEX,   0C1HEX,  11111reg, byte
;
;   5) rol reg,1 (16 bit)                │ 066HEX,   0D1HEX,  11000reg
;      ror reg,1                         │ 066HEX,   0D1HEX,  11001reg
;      rcl reg,1                         │ 066HEX,   0D1HEX,  11010reg
;      rcr reg,1                         │ 066HEX,   0D1HEX,  11011reg
;      shl reg,1                         │ 066HEX,   0D1HEX,  11100reg
;      shr reg,1                         │ 066HEX,   0D1HEX,  11101reg
;      sal reg,1                         │ 066HEX,   0D1HEX,  11110reg
;      sar reg,1                         │ 066HEX,   0D1HEX,  11111reg
;
;   6) inc  reg (16 bit)                 │ 066HEX, 01000reg
;      dec  reg                          │ 066HEX, 01001reg
;      push reg                          │ 066HEX, 01010reg
;      pop  reg                          │ 066HEX, 01011reg
;      pusha                             │ 066HEX, 01100000
;      popa                              │ 066HEX, 01100001
;
;   7) mov reg,word                      │ 066HEX, 10111reg,      word
;
 @066_disasm:   lodsw
                sub    ecx,ecx
;
; Проверим на инструкции из раздела №6
;
                cmp    al,01000000b              
                jc     @066_disasm_test_1        ; < inc eax
                cmp    al,01100010b
                jc    _@two_bytes                ; < (popa) + 1 byte
;
; Проверим на инструкции из раздела №2b               
;
 @066_disasm_test_1:
                cmp    al,005h
                jz    _@four_bytes               ; add ax,word
                cmp    al,00Dh
                jz    _@four_bytes               ; or ax,word
                cmp    al,015h                
                jz    _@four_bytes               ; adc ax,word
                cmp    al,01Dh                
                jz    _@four_bytes               ; sbb ax,word
                cmp    al,025h                
                jz    _@four_bytes               ; and ax,word
                cmp    al,02Dh
                jz    _@four_bytes               ; sub ax,word
                cmp    al,035h                
                jz    _@four_bytes               ; xor ax,word
                cmp    al,03Dh
                jz    _@four_bytes               ; cmp ax,word
;
; Проверим на инструкции из раздела №3               
;
                cmp    al,001h
                jz     @066_disasm_3_5           ; add rega,regb (вариант 1)
                cmp    al,003h
                jz     @066_disasm_3_5           ; add rega,regb (вариант 2)
;
                cmp    al,00Ah
                jz     @066_disasm_3_5           ; or rega,regb (вариант 1)
                cmp    al,00Bh
                jz     @066_disasm_3_5           ; or rega,regb (вариант 2)
;
                cmp    al,011h
                jz     @066_disasm_3_5           ; adc rega,regb (вариант 1)
                cmp    al,013h
                jz     @066_disasm_3_5           ; adc rega,regb (вариант 2)
;
                cmp    al,019h
                jz     @066_disasm_3_5           ; sbb rega,regb (вариант 1)
                cmp    al,01Bh
                jz     @066_disasm_3_5           ; sbb rega,regb (вариант 2)
;
                cmp    al,021h
                jz     @066_disasm_3_5           ; and rega,regb (вариант 1)
                cmp    al,023h
                jz     @066_disasm_3_5           ; and rega,regb (вариант 2)
;
                cmp    al,029h
                jz     @066_disasm_3_5           ; sub rega,regb (вариант 1)
                cmp    al,02Bh
                jz     @066_disasm_3_5           ; sub rega,regb (вариант 2)
;
                cmp    al,031h
                jz     @066_disasm_3_5           ; xor rega,regb (вариант 1)
                cmp    al,033h
                jz     @066_disasm_3_5           ; xor rega,regb (вариант 2)
;
                cmp    al,039h
                jz     @066_disasm_3_5           ; cmp rega,regb (вариант 1)
                cmp    al,03Bh
                jz     @066_disasm_3_5           ; cmp rega,regb (вариант 2)
;
                cmp    al,089h
                jz     @066_disasm_3_5           ; mov rega,regb (вариант 1)
                cmp    al,08Bh
                jz     @066_disasm_3_5           ; mov rega,regb (вариант 2)
;
; Проверим на инструкции остальных разделов (№: 1, 2, 4, 5)               
;
                cmp    al,081h
                jz     @066_disasm_1_2
                cmp    al,0a7h
                jz    _@two_spec                 ; cmpsw
                cmp    al,0abh
                jz    _@two_spec                 ; stosw
                cmp    al,0adh
                jz    _@two_spec                 ; lodsw
                cmp    al,0C1h
                jz     @066_disasm_4
                cmp    al,0D1h
                jz     @066_disasm_3_5
;
; Проверим на инструкции раздела 7
;
                cmp    al,10111000b
                jc     @066_disasm_test_2        ; < mov ax, word
                cmp    al,11000000b
                jc    _@four_bytes               ; <= mov di, word
;
 @066_disasm_test_2:
                jmp    unknown_instr
;───────────────
; Инструкции первого или второго разделов
;
 @066_disasm_1_2:
                cmp    ah,11000000b
                jae    _@five_bytes      
;───────────────
; Все инструкции раздела №1
; 
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB,
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
;  По составу соответствует "xxx byte ptr [...],byte" (080h),
; только на 2 байта больше. Разбираются аналогично "080h"
;
 @066_disasm_1: inc    ecx
                inc    ecx
                sub    eax,eax
                dec    esi
                lodsb
                jmp    @080_disasm_test
;───────────────
; Инструкции разделов №3 & 5 
;
 @066_disasm_3_5:
                cmp    ah,11000000b
                jc     unknown_instr
                inc    ecx
                jmp   _@two_bytes 
;───────────────
; Инструкции раздела №4
;
 @066_disasm_4: cmp    ah,11000000b
                jc     unknown_instr
                jmp   _@four_bytes