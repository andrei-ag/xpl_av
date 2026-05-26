.data
;──────────────────────
;  буфер "dregs", используется для хранения информации об инструкциях,
; разобранных "дополнительно".
;
; dregs+000 - количество регистров использующихся в инструкции
;             для указания смещения
; dregs+1*4 - номер первого регистра использующегося для ...
; dregs+n*4 - номер n-ого регистра ...
;
 dregs                 dd 010 dup (?)            ; см. выше
.code
;──────────────────────────────────────────────────────────────────────────────
; disasm - дизассемблер инструкций (используется эмулятором)
;
;  Определяет параметры инструкции (использующиеся регистры и размер) в том
; случае, если данные о инструкции присутствуют в базе.
;  Инструкции, которые обращаются к смещениям, разбираются дополнительно.
; Данные о количестве регистров, использующихся для указания смещения и
; их номера, складываются в буфер "dregs".
;
; on start : oins - смещение инструкции
; on exit  :  eax = -1 если указанная инструкция неизвестна дизассемблеру
;             ecx - размер инструкции
;             edx - действия необходимые для имитация исполнения инструкции:
;                   0 - достаточно запуска в "карантине"
;                   1 - требует полной имитации исполнения
;
 disasm         proc   oins: dword
;──────────────────────────────────────────────────────────────────────────────
                push   ebx edi esi
                mov    esi,oins                  ; esi - смещение инструкции
 disasm_start:  mov    dword ptr [dregs],0       ; инициализация буфера "dregs"
                sub    eax,eax                   ;┐
                sub    ecx,ecx                   ;│ инициализация регистров
                sub    edx,edx                   ;┘
;
; ca_ip_srch - часть анализатора кода, предназначенная для поиска процедур
;              подсчета delta value
;
                call   ca_ip_srch                ; * (см. выше)
                lodsb                            ; al - первый байт инструкции
;════════════════════════════════════════════════
                cmp    al,001h
                jz    _@instr_2ereg
 @add_al_byte:  cmp    al,004h
                jz    _@two_bytes
 @add_eax_dword:cmp    al,005h                   ;   005HEX, dword
                jz    _@five_bytes               ; " add eax, dword "
;───────────────
                cmp    al,008h
                jz    _@instr_2ereg
                cmp    al,009h
                jz    _@instr_2ereg
                cmp    al,00Fh
                jz     @00F_disasm
 @push_cs:      cmp    al,00Eh
                jz    _@one_byte                 ; " push cs "
 @push_ds:      cmp    al,01Eh
                jz    _@one_byte                 ; " push ds "
                cmp    al,011h
                jz    _@instr_2ereg
;───────────────
 @sub_2ereg:    cmp    al,029h
                jz    _@instr_2ereg
                cmp    al,02Bh
                jz     @02B_disasm
 @sub_al_byte:  cmp    al,02Ch                   ;   02CHEX, byte
                jz    _@two_bytes                ; " sub al, byte "
                cmp    al,030h
                jz    @030_disasm
                cmp    al,031h
                jz    @031_disasm
 @cmp_al_byte:  cmp    al,03Ch
                jz    _@two_bytes
 @ds:           cmp    al,03Eh
                jz    @03E_disasm                ; " ds: "
;───────────────
 @inc_ereg:     cmp    al,040h                   ;
                jc     unknown_instr
                cmp    al,047h
                jbe   _@one_byte                 ; " inc ereg "
;───────────────
 @dec_ereg:     cmp    al,048h                   ;
                jc     unknown_instr
                cmp    al,04Fh
                jbe   _@one_byte                 ; " dec ereg "
;───────────────
 @push_ereg:    cmp    al,050h                   ;
                jc     unknown_instr
                cmp    al,057h
                jbe   _@one_byte                 ; " push ereg "
;───────────────
 @pop_ereg:     cmp    al,058h                   ;   1011reg
                jc     unknown_instr
                cmp    al,05Fh
                jbe   _@one_byte                 ; " pop ereg "
;───────────────
 @pushad:       cmp    al,060h
                jz    _@one_byte                 ; " pushad "
 @gs:           cmp    al,065h
                jz    @03E_disasm                ; " gs: "
                cmp    al,066h
                jz    @066_disasm
;───────────────
 @push_dword:   cmp    al,068h
                jnz   _@06A_disasm
                mov    cl,4
                jmp   _@one_byte                 ; " push dword "
;
_@06A_disasm:   cmp    al,06Ah
                jz    @06A_disasm
;───────────────
; инструкции:
; jo, jno, jb, jnb, je, jne, jbe, ja, js, jns, jp, jnp, jl, jnl, jle, jg
;
 @jo_byte:      cmp    al,070h
                jc     unknown_instr
 @jg_byte:      cmp    al,07fh
                jbe   _@two_spec
;───────────────
                cmp    al,080h
                jz    @080_disasm
                cmp    al,081h
                jz    @0813_disasm
                cmp    al,083h
                jz    @0813_disasm
;───────────────
                cmp    al,086h
                jz    _@instr_2ereg
                cmp    al,088h                   ; "mov byte ptr [ereg], reg"
                jz    @08A_disasm                ; разбирается в "@08A"
                cmp    al,089h
                jz    @089_disasm
                cmp    al,08Ah
                jz    @08A_disasm
                cmp    al,08Bh
                jz    @08B_disasm
                cmp    al,08Dh
                jz    @08D_disasm
                cmp    al,08Fh
                jz    @08F_disasm
 @nop:          cmp    al,090h
                jz    @03E_disasm                ; " nop "
;──────────────
; xchg ecx/edx/ebx/esp/ebp/esi/edi,ecx 
;
 @xchg_ecx_eax: cmp    al,091h
                jc     unknown_instr             ; " xchg ecx, eax "
 @xchg_edi_eax: cmp    al,097h
                jbe   _@one_byte                 ; " xchg edi, eax "
;
 @cwde:         cmp    al,098h
                jz    _@one_byte                 ; " cwde "
 @cdq:          cmp    al,099h
                jz    _@one_byte                 ; " cdq "
 @pushfd:       cmp    al,09Ch
                jz    _@one_byte                 ; " pushfd "
 @popfd:        cmp    al,09Dh
                jz    _@one_byte                 ; " popfd "
 @stosb:        cmp    al,0AAh
                jz    _@one_spec                 ; " stosb "
 @lodsb:        cmp    al,0ACh
                jz    _@one_spec                 ; " lodsb "
 @scasd:        cmp    al,0AFh
                jz    _@one_byte                 ; " scasd "
;───────────────
 @mov_ereg_byte:cmp    al,0B0h                   ;   10110reg, byte
                jc     unknown_instr
                cmp    al,0B7h
                jbe   _@two_bytes                ; " mov ereg, byte "
;───────────────
 @mov_ereg_dd:  cmp    al,0B8h                   ;   10111reg, dword
                jc     unknown_instr
                cmp    al,0BFh
                jbe   _@five_bytes               ; " mov ereg,dword "
;───────────────
                cmp    al,0C1h
                jz    @0C1_disasm
 @ret_byte:     cmp    al,0C3h
                jz    _@one_spec                 ; " ret "
 @loop_byte:    cmp    al,0E2h
                jz    _@loop_byte
 @call_dword:   cmp    al,0E8h                   ; ─
                jz    _@cj_dword                 ; инструкции call и jmp
 @jmp_dword:    cmp    al,0E9h                   ; требуют одинакового разбора
                jz    _@cj_dword                 ; ─
 @jmp_byte:     cmp    al,0EBh
                jz    _@two_spec
 @cmc:          cmp    al,0F5h
                jz    _@one_byte                 ; " cmc "
 @clc:          cmp    al,0F8h
                jz    _@one_byte                 ; " clc "
 @stc:          cmp    al,0F9h
                jz    _@one_byte                 ; " stc "
 @cli:          cmp    al,0FAh
                jz    _@one_byte                 ; " cli "
 @sti:          cmp    al,0FBh
                jz    _@one_byte                 ; " sti "
 @cld:          cmp    al,0FCh
                jz    _@one_byte                 ; " cld "
 @std:          cmp    al,0FDh
                jz    _@one_byte                 ; " std "
                cmp    al,0FEh
                jz    @0FE_disasm
                cmp    al,0FFh
                jz    @0FF_disasm
;
 unknown_instr: sub    eax,eax                   ; инструкции нет в базе
                dec    eax                       ; eax = -1
;
 disasm_ret:    pop    esi edi ebx
                ret
;══════════════════════
; 1) add  [eregA],[eregB]                  │ 001HEX, 11rgArgB
; 2) or   [regA] ,[regB]                   │ 008HEX, 11rgArgB
; 3) or   [eregA],[eregB]                  │ 009HEX, 11rgArgB
; 4) adc  [eregA],[eregB]                  │ 011HEX, 11rgArgB
; 5) sub  [eregA],[eregB]                  │ 029HEX, 11rgBrgA
; 6) xchg [eregA],[eregB]                  │ 086HEX, 11rgBrgA
;
_@instr_2ereg:  lodsb
                cmp    al,11000000b
                jc     unknown_instr
                jmp   _@two_bytes
;══════════════════════
; 1) jne jo, jno, jb, jnb, je, jne,        │ 00FHEX, 080 ... 08FHEX, dword
;    jbe, ja, js, jns, jp, jnp, jl,
;    jnl, jle, jg dword
;
; 2) movzx [ereg],[REG]                    │ 00FHEX, 0B7HEX, 11ergREG
; 3) movsx [ereg],[REG]                    │ 00FHEX, 0BFHEX, 11ergREG
;
 @00F_disasm:   lodsw
                inc    ecx
                cmp    al,080h                   ; "1"
                jc     unknown_instr
                cmp    al,08Fh
                jna   _@cj_dword
;
 @00F_disasm_2: cmp    al,0B7h                   ; "2"
                jz     @00F_disasm_3b
 @00F_disasm_3: cmp    al,0BFh                   ; "3"
                jnz    unknown_instr
 @00F_disasm_3b:cmp    ah,11000000b
                jc     unknown_instr
                jmp   _@two_bytes
;══════════════════════
; 1) sub [eregA],[eregB]                   │ 02BHEX, 11rgArgB
;
 @02B_disasm:   cmp    al,11000000b
                jc     unknown_instr
                jmp   _@two_bytes
;══════════════════════
; 1a) xor byte ptr [eregA],[reg]           │ 030HEX, 00regrgA
; 1b) xor byte ptr [esp],[reg]             │ 030HEX, 00REGesp, 00espESP
;  2) xor [regA],[regB]                    │ 030HEX, 11rgArgB
;  3) xor [eregA+eregB],[reg]              │ 030HEX, 00reg100, 00rgArgB
;
 @030_disasm:  ;jmp    @031_disasm
;══════════════════════
; 1a) xor dword ptr [eregA],[eregB]        │ 031HEX, 00rgBrgA
; 1b) xor dword ptr [esp],[ereg]           │ 031HEX, 00REGesp, 00espESP
;  2) xor [eregA],[eregB]                  │ 031HEX, 11rgArgB
;  3) xor [eregA+eregB],[eregC]            │ 031HEX, 00rgC100, 00rgArgB
;
 @031_disasm:   lodsb
                cmp    al,11000000b              ; если al > 11eaxEAX (11000000)
                jnc   _@two_bytes                ; перед нами инструкция 2 типа
;───────────────
                call   two_regs                  ; eax - "rgA" / edx - "rgB"
                cmp    dl,00000100b
                jnz    @031_disasm_1             ; инструкция "1а типа"
                inc    ecx        ; инструкции остальных типов на 1 байт больше
                lodsb
                cmp    al,00100100b         ; инструкция типа "1b", на 1 байт
                jz     @031_disasm_1        ; больше, но разбирается как и "1a"
 @031_disasm_3: call   two_regs
                inc    dword ptr [dregs]
                mov    dword ptr [dregs+008],eax
 @031_disasm_1: inc    dword ptr [dregs]
                mov    dword ptr [dregs+004],edx
                inc    ecx
                inc    ecx                       ; ecx - размер инструкции
                sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                jmp    disasm_ret
;══════════════════════
; 1) ds:                                   │ 03EHEX
; 2) gs:                                   │ 065HEX
; 3) nop                                   │ 090HEX
;
;    инструкции "пустышки", пропускаются дизассемблером
;
 @03E_disasm:   pop    eax                       ; вспомним значение esi в eax
                push   esi                       ; установим новое значение esi
                jmp    disasm_start              ; разберем следующую инструкцию
;══════════════════════
; 1) xor word ptr [ereg],word          │ 066HEX,   081HEX,          , word
; 2) xor word ptr  [ebp],word          │ 066HEX,   081HEX,    075HEX, 000HEX, WORD
; 3) mov [regA],[regB]                 │ 066HEX,   08AHEX, 111rgArgB
; 4) mov [reg],word                    │ 066HEX, 10111reg,      word
; 5) shr reg (word),byte               │ 066HEX,   0C1HEX,  11101reg, byte
;
 @066_disasm:   lodsw
                cmp    al,081h
                jz     @066_disasm_12
;───────────────
                cmp    al,08Bh
                jz     @066_disasm_3
;
                cmp    al,10111000b
                jc     unknown_instr
                cmp    al,10111111b
                ja     @066_test_5
 @066_disasm_4: mov    cl,4                      ; тип "4"
                jmp    disasm_ret
;
 @066_test_5:   cmp    al,0C1h
                jz     @066_disasm_5
                jmp    unknown_instr
;───────────────
 @066_disasm_3: cmp    ah,11000000b
                jc     unknown_instr
                inc    ecx
                jmp   _@two_bytes 
;
 @066_disasm_5: cmp    ah,11101000b
                jc     unknown_instr
                cmp    ah,11101111b
                ja     unknown_instr
                mov    cl,4
                jmp    disasm_ret
;───────────────
 @066_disasm_12:cmp    ah,030h
                jc     unknown_instr
                mov    cl,6
                cmp    ah,075h
                jz     @066_disasm_2
                cmp    ah,037h
                ja     unknown_instr
                cmp    ah,034h
                jz     unknown_instr
                cmp    ah,035h
                jz     unknown_instr
                dec    ecx
                xor    ah,030h
                mov    al,000h
                xchg   ah,al
                jmp   @066_disasm_1
;───────────────
 @066_disasm_2: mov    al,005h
 @066_disasm_1: call   set_dreg_eax
                jmp    disasm_ret
;══════════════════════
; 1) push 00000000                         │ 06AHEX, 00000000
;
  @06A_disasm:  lodsb
                cmp    al,0
                jz    _@two_bytes
                jmp    unknown_instr
;══════════════════════
;  1) cmp byte ptr [ereg],byte             │ 080HEX, 00111reg, byte
; 2a) xor byte ptr [ereg+dword],byte       │ 080HEX, 10110reg,  dword, byte
; 2b) xor byte ptr [esp+dword],byte        │ 080HEX, 10110esp, 024HEX, dword, byte
;  3) add [reg],byte                       │ 080HEX, 11000reg, byte
;  4) or  [reg],byte                       │ 080HEX, 11001reg, byte
;  5) adc [reg],byte                       │ 080HEX, 11010reg, byte
;  6) sbb [reg],byte                       │ 080HEX, 11011reg, byte
;  7) and [reg],byte                       │ 080HEX, 11100reg, byte
;  8) sub [reg],byte                       │ 080HEX, 11101reg, byte
;  9) xor [reg],byte                       │ 080HEX, 11110reg, byte
; 10) cmp [reg],byte                       │ 080HEX, 11111reg, byte
;
 @080_disasm:   lodsb
                mov    cl,3
                cmp    al,00111000b
                jc     unknown_instr
                cmp    al,00111111b
                jbe   @080_disasm_1              ; инструкция "типа 1"
;
                cmp    al,10110000b             ; "2a/b"
                jc     unknown_instr
                cmp    al,10110111b
                jbe   @080_disasm_2
;
                cmp    al,11000000b              ; "3 ... 10"
                jc     unknown_instr
                jmp    disasm_ret
                jmp    unknown_instr
;───────────────
 @080_disasm_1: xor    al,00111000b
 @080_disasm_1a:call   set_dreg_eax
                jmp    disasm_ret
;───────────────
 @080_disasm_2: add    cl,4                      ; разбор инструкции типа "2a/b"
                xor    al,10110000b
                cmp    al,100b
                jnz    @080_disasm_1a
                inc    ecx
                jmp    @080_disasm_1a
;══════════════════════
; 1a) xor dword ptr [ereg],dword                 │ 081HEX, 00110reg, dword
; 1b) xor dword ptr [esp],dword                  │ 081HEX, 00110100, 024HEX, dword
; 7a) add dword ptr [ereg+dword],dword           │ 081HEX, 10000reg, dword, dword
; 7b) add dword ptr [esp+dword],dword            │ 081HEX, 10000100, 024HEX, dword
; 8a) xor dword ptr [ereg+dword],dword           │ 081HEX, 10110reg, dword, dword
; 8b) xor dword ptr [esp+dword],dword            │ 081HEX, 10110100, 024HEX, dword
; 2) add [ereg],dword                            │ 081HEX, 11000reg, dword
; 3)  or [ereg],dword                            │ 081HEX, 11001reg, dword
; 4) adc [ereg],dword                            │ 081HEX, 11010reg, dword
; 5) sub [ereg],dword                            │ 081HEX, 11101reg, dword
; 6) cmp [ereg],dword                            │ 081HEX, 11111reg, dword
;                                                │
; 1a) xor byte ptr [ereg],byte                   │ 083HEX, 00110reg, byte
; 1b) xor byte ptr [esp],byte                    │ 083HEX, 00110100, 024HEX, byte
; 7a) add byte ptr [ereg+byte],byte              │ 083HEX, 10000reg, byte, byte
; 7b) add byte ptr [esp+byte],byte               │ 083HEX, 10000100, 024HEX, byte
; 8a) xor byte ptr [ereg+byte],byte              │ 083HEX, 10110reg, dword, dword
; 8b) xor byte ptr [esp+byte],byte               │ 083HEX, 10110100, 024HEX, dword
; 2) add [ereg],byte                             │ 083HEX, 11000reg, byte
; 3)  or [ereg],byte                             │ 083HEX, 11001reg, byte
; 4) adc [ereg],byte                             │ 083HEX, 11010reg, byte
; 5) sub [ereg],byte                             │ 083HEX, 11101reg, byte
; 6) cmp [ereg],byte                             │ 083HEX, 11111reg, byte
;
 @0813_disasm:  mov    cl,3
                cmp    al,83h
                jz     @0813_disasm_1
                add    cl,3
 @0813_disasm_1:lodsb
                mov    ah,00110000b
                cmp    al,ah                     ; "1a/b"
                jc     unknown_instr
                cmp    al,00110111b
                jna    @0813_7ab_2

                mov    ah,10000000b
                cmp    al,ah                     ; "7a/b"
                jc     unknown_instr
                cmp    al,10000111b
                jna    @0813_7ab	
                mov    ah,10110000b
                cmp    al,ah                     ; "8a/b"
                jc     unknown_instr
                cmp    al,10110111b
                jna    @0813_7ab	

                cmp    al,11000000b
                jc     unknown_instr
                jmp    disasm_ret                ; add, or, adc, sub, cmp
;───────────────
 @0813_7ab:     inc    ecx
                cmp    cl,4
                jz     @0813_7ab_2
                add    cl,3
 @0813_7ab_2:   xor    al,ah
                mov    ah,0
                call   set_dreg_eax
                cmp    al,100b                   ; инструкция типа "b" ?
                jnz    disasm_ret                ; нет, закончим разбор
                inc    ecx                       ; инструкция тип "b" имеет
                                                 ; размер на 1 байт больше
                jmp    disasm_ret                ;
;══════════════════════
; 1) mov dword ptr [ergA],[ergB]           │ 089HEX, 00rgBrgA
; 2) mov [ergA],[ergB]                     │ 089HEX, 11rgBrgA
;
 @089_disasm:   lodsb
;
; инструкция "типа 1" разбирается так же, как и
; "mov [reg],byte ptr [EReG]" (см. ниже)
;
                cmp    al,111111b
                jbe   @08A_disasm_2
;
; инструкцию "mov eregA, eregB" не разбираем, возвращаем только ее размер
;
                cmp    al,11000000b
                jc     unknown_instr
                mov    cl,2
                jmp    disasm_ret
;══════════════════════
; 1a) mov [reg],byte ptr [EReG]            │ 08AHEX, 00regERG
; 1b) mov [reg],byte ptr [esp]             │ 08AHEX, 00regESP, 00espESP
; 1c) mov [reg],byte ptr [ebp]             │ 08AHEX, 01regEBP, 00000000
; 2a) mov [reg],byte ptr [EReG+byte]       │ 08AHEX, 01regERG, byte
; 2b) mov [reg],byte ptr [esp+byte]        │ 08AHEX, 01regESP, 00espESP, byte
;
 @08A_disasm:   lodsb
                cmp    al,00111111b
                ja     @08A_disasm_1
;
                inc    ecx
                call   two_regs
                cmp    dl,00000100b
                jz     @08A_disasm_3
                cmp    dl,00000101b
                jz     @08A_disasm_3
                dec    ecx
                jmp    @08A_disasm_3
;
 @08A_disasm_1: cmp    al,01111111b
                ja     unknown_instr
                inc    ecx
                cmp    byte ptr [esi],00100100b
                jnz    @08A_disasm_2
                inc    ecx
;
 @08A_disasm_2: call   two_regs                  ; eax - "reg" / edx - "ERG"
 @08A_disasm_3: inc    dword ptr [dregs]
                mov    dword ptr [dregs+004],edx
                sub    edx,edx
                inc    edx                       ; edx = 1 - полная имитация
                inc    ecx                       ; ecx - размер инструкции
                inc    ecx                       ;       минимум 2 байта
                jmp    disasm_ret
;══════════════════════
; 1a) mov [eregA],dword ptr [eregB]        │ 08BHEX, 00rgArgB
; 1b) mov [eregA],dword ptr [esp]          │ 08BHEX, 00rgAESP, 00espESP
; 1c) mov [eregA],dword ptr [ebp]          │ 08BHEX, 01rgAEBP, 00000000
; 2a) mov [eregA],dword ptr [eregB+byte]   │ 08BHEX, 01rgArgB, byte
; 2b) mov [eregA],dword ptr [esp+byte]     │ 08BHEX, 01rgAESP, 00espESP, byte
; 3 ) mov [eregA],[eregB]                  │ 08BHEX, 11rgArgB
;
;  Инструкции по строению и соотвественно способам разбора полностью
; соотвествуют тем, что разбираются в "@08A_disasm". Исключением является тип
; "3", который разбирается ниже
;
 @08B_disasm:   cmp    byte ptr [esi],11000000b
                jc    @08A_disasm
                inc    ecx
                inc    ecx
                jmp    disasm_ret
;══════════════════════
;  1) lea ergA,[ergB+ergC]                 │ 08DHEX, 00010rgA, 00rgCrgB
; 2a) lea ergA,[ergB+dword]                │ 08DHEX, 10rgArgB, dword
; 2b) lea ereg,[ esp+dword]                │ 08DHEX, 10reg100, 00ESPesp, dword
;  3) lea ergA,[ergB+ergC+dword]           │ 08DHEX, 10rgA100, 00rgCrgB, dword
;
;  Инструкции типов 1-3 не требуют специальной имитации исполнения и по этому
; дизассемблер определяет только размер разбираемой инструкции
;
 @08D_disasm:   lodsb
                cmp    al,10000000b              ; если значение al
                jnc   _@08D_disasm_2             ; от 10000000b то тип 2 и выше
;
; если значение регистра al, выходит из промежутка: 10000 - 10111 (10eax-10edi)
; то - перед нами неизвестная инструкция иначе, инструкция "1" типа (3 байта)
;
                cmp    al,10000b
                jc     unknown_instr
                cmp    al,11111b
                ja     unknown_instr
                mov    cl,3
                xor    al,10000b             ; уберем мусор
                push   eax                   ; запомним значение "eregA"
                lodsb                        ; проверим регистры eregB и eregC
                call   ca_check_regs         ; на наличие ip и указания на стек
                pop    eax
                mov    byte ptr [eax+_regs],dl
                jmp    disasm_ret
;
; если значение регистра al, находится в промежутке:
; 10eax100 - 10edi100 (10000100 - 10111100)
; то это инструкция типа "2b" или "3", ее размер равен 7 байт
; иначе это инструкция типа "2a" (размер 6 байт)
;
_@08D_disasm_2: cmp    al,10111111b
                ja     unknown_instr
                xor    al,10000000b
                call   two_regs                  ; eax - "rgA" / edx - "rgB"
                xchg   eax,edx
                sub    edx,edx
                mov    cl,7
                cmp    al,00000100b
                jz     disasm_ret
;
; lea ergA,[ergB+dword]                   │ 08DHEX, 10rgArgB, dword
; ~~
                mov    al,byte ptr [esi-1]
                xor    al,10000000b
                call   two_regs
                mov    dl,byte ptr [edx+_regs]
                mov    byte ptr [eax+_regs],dl
                dec    ecx
                sub    edx,edx
                jmp    disasm_ret
;══════════════════════
; 1) pop dword ptr [eregA+eregB]           │ 08FHEX, 00000100, 00rgBrgA
;
;  Инструкция разбирается аналогично "push dword ptr [eregA+eregB]",
; единственное различие в первых двух байтах - идентификаторах типа инструкции.
;
 @08F_disasm:   lodsb
                cmp    al,00000100b
                jnz    unknown_instr
                jmp    @0FF_disasm_1
;══════════════════════
; 1) rol [ereg],byte                       │ 0C1HEX, 11000reg, byte
; 2) shl [ereg],byte                       │ 0C1HEX, 11100reg, byte
;
 @0C1_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr
                cmp    al,11000111b
                jbe    @0C1_disasm_12
;
                cmp    al,11100000b
                jc     unknown_instr
                cmp    al,11100111b
                ja     unknown_instr
 @0C1_disasm_12:mov    cl,3
                jmp    disasm_ret
;══════════════════════
; loop byte                                │ 0E2HEX, FE-byte = num
;                                                    jmp to current IP + num
;
_@loop_byte:    mov    al,0feh                   ; al = 0FE
                sub    al,byte ptr [esi]         ; al = 0FE-byte
                jmp   _@two_spec
;══════════════════════
; call dword                               │ 0E8HEX, dword
; jmp  dword                               │ 0E9HEX, dword
;
_@cj_dword:     sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                jmp   _@five_bytes
;══════════════════════
; 1a) dec  byte ptr [ereg+dword]           │ 0FEHEX, 10001reg, dword
; 1b) dec  byte ptr [esp+dword]            │ 0FEHEX, 10001100,  024HEX, dword
;
 @0FE_disasm:   lodsb
                cmp    al,10001000b
                jc     unknown_instr
                cmp    al,10001111b
                ja     unknown_instr
                jmp    @0FF_disasm_2b
;══════════════════════
;  1) push dword ptr [eregA+eregB]         │ 0FFHEX, 00110100, 00rgBrgA
; 2a) dec  dword ptr [ereg+dword]          │ 0FFHEX, 10001reg, dword
; 2b) dec  dword ptr [esp+dword]           │ 0FFHEX, 10001100,  024HEX, dword
;  3) dec  dword ptr [eregA+eregB+dword]   │ 0FFHEX, 10001100, 00rgArgB, dword
;  4) call [ereg+dword]                    │ 0FFHEX, 10010reg, dword
;
 @0FF_disasm:   lodsb
                cmp    al,00110100b
                jz     @0FF_disasm_1
;═══════════════
 @0FF_disasm_2: cmp    al,10001000b
                jc     unknown_instr
                cmp    al,10001100b
                jnz    @0FF_disasm_2a
                inc    ecx                       ; *
                cmp    byte ptr [esi],024h
                jnz    @0FF_disasm_3
 @0FF_disasm_2a:cmp    al,10001111b
                ja     @0FF_disasm_4
;
 @0FF_disasm_2b:xor    al,10001000b
                jmp    @0FF_disasm_4b
;═══════════════
 @0FF_disasm_3: add    cl,3
                jmp    @0FF_disasm_1
;═══════════════
 @0FF_disasm_4: cmp    al,10010000b
                jc     unknown_instr
                cmp    al,10010111b
                ja     unknown_instr
;
                sub    ecx,ecx                   ; *
                xor    al,10010000b
 @0FF_disasm_4b:call   set_dreg_eax
                add    cl,6                      ; ecx - размер инструкции
                jmp    disasm_ret
;═══════════════
 @0FF_disasm_1: lodsb
                call   two_regs
                inc    dword ptr [dregs]
                mov    dword ptr [dregs+008],edx
                call   set_dreg_eax
                add    cl,3
                jmp    disasm_ret
;──────────────────────────────────────────────────────────────────────────────
; _@five_bytes - инструкция размером 5 байт, требующая запуска в "карантине"
;
_@five_bytes:   add    cl,5
                jmp    disasm_ret
;───────────────
; _@two_spec - инструкция размером 2 байта, требующая полной имитации исполнения
; _@two_spec - инструкция размером 2 байт, требующая запуска в "карантине"
;
_@two_bytes:    inc    ecx
                jmp   _@one_byte
_@two_spec:     inc    ecx
;───────────────
; разбор простейших однобайтовых инструкций
; _@one_spec - инструкция размером 1 байт, требующая полной имитации исполнения
; _@one_byte - инструкция размером 1 байт, требующая запуска в "карантине"
;
_@one_spec:     sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
;───────────────
_@one_byte:     inc    ecx
                jmp    disasm_ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 set_dreg_eax   proc
                inc    dword ptr [dregs]
                mov    dword ptr [dregs+004],eax
                sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; если регистр al содержит данные 00rgArgB (где rgA, rgB два регистра),
; то на выходе edx - rgB
;              eax - rgA
;
 two_regs       proc
                push   eax
                shr    al,3                      ; - al = regA
                pop    edx
                push   eax
                rol    al,3
                xor    dl,al                     ; - dl = regB
                pop    eax
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────