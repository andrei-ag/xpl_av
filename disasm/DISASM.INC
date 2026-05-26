;──────────────────────────────────────────────────────────────────────────────
; disasm - дизассемблер инструкций (используется эмулятором)
;
;  Определяет параметры инструкции (использующиеся регистры и размер) в том
; случае, если данные о инструкции присутствуют в базе.
;  Инструкции, которые обращаются к смещениям, разбираются дополнительно.
; Данные о количестве регистров, использующихся для указания смещения и
; их номера, складываются в буфер "dregs".
;
; on start : instr_offset - смещение инструкции
; on exit  :  eax = -1 если указанная инструкция неизвестна дизассемблеру
;             ecx - размер инструкции
;             edx - действия необходимые для имитация исполнения инструкции:
;                   0 - достаточно запуска в "карантине"
;                   1 - требует полной имитации исполнения
;             ebx - состояние строчного представления инструкции
;                   ( 0 если подготовлено и находится в "disasm_str" ) 
;
 proc disasm           instr_offset
;──────────────────────────────────────────────────────────────────────────────
                jmp    disasm_init
;
 disasm_nfo            db 'disASM%005'           ; версия дизассемблера
;
 disasm_init:   
                sub    eax,eax                   ;┐
                sub    ecx,ecx                   ;│ инициализация регистров
                sub    edx,edx                   ;┘
;
                push   edi
                push   esi
;
                lea    edi,[segments]            ; очистим индикаторы сегментов
                stosd
                stosw
;
                mov    esi,[instr_offset]        ; esi - смещение инструкции
 
 disasm_start:  mov    dword [dregs],0           ; инициализация буфера "dregs"
;
; ca_ip_srch - часть анализатора кода, предназначенная для поиска процедур
;              подсчета delta value
;
; *** отключено с версии 12 версии программы
;
;               call   ca_ip_srch                ; * (см. выше)
;
                lodsb
                call   mul_4
               
                mov    eax,dword [disasm_table_of_procs+eax]
                or     eax,eax                 
                jnz    run_disasm_proc           ; -> Нет процедуры

 unknown_instr: sub    eax,eax                   ; инструкции нет в базе
                dec    eax                       ; Error code = -1
;
 disasm_ret:    pop    esi
                pop    edi
                ret
;
 run_disasm_proc:
                push    eax
                sub     eax,eax
                        db 0c3h ; ( ret ) -> RUN PROC
;══════════════════════
; Однотипные инструкций не требующие имитации и имеющие одинаковый размер
;
;  1) add  regA,regB   (8 bit)             │ 000HEX, 11rgArgB
;                      (32 bit)            │ 001HEX, 11rgArgB
;                      (8 bit)             │ 002HEX, 11rgBrgA 
;                      (32 bit)            │ 003HEX, 11rgBrgA 
;  2) or   regA,regB   (8 bit)             │ 008HEX, 11rgArgB
;                      (32 bit)            │ 009HEX, 11rgArgB
;                      (8 bit)             │ 00AHEX, 11rgArgB
;                      (32 bit)            │ 00BHEX, 11rgArgB
;  3) adc  regA,regB   (8 bit)             │ 010HEX, 11rgBrgA
;                      (32 bit)            │ 011HEX, 11rgArgB
;                      (8 bit)             │ 012HEX, 11rgBrgA
;                      (32 bit)            │ 013HEX, 11rbArbB
;  4) sbb  regA,regB   (8 bit)             │ 018HEX, 11rgArgB
;                      (32 bit)            │ 019HEX, 11rgArgB
;                      (8 bit)             │ 01AHEX, 11rgArgB
;                      (32 bit)            │ 01BHEX, 11rgBrgA
;  5) and  regA,regB   (8 bit)             │ 020HEX, 11rgBrgA
;                      (32 bit)            │ 021HEX, 11rgBrgA
;                      (8 bit)             │ 022HEX, 11rgBrgA
;                      (32 bit)            │ 023HEX, 11rgBrgA
;  6) sub  regA,regB   (8 bit)             │ 028HEX, 11rgBrgA
;                      (32 bit)            │ 029HEX, 11rgBrgA
;                      (8 bit)             │ 02AHEX, 11rgBrgA
;                      (32 bit)            │ 02BHEX, 11rgBrgA
;  7) xor  regA,regB   (8 bit)             │ 030HEX, 11rgBrgA
;                      (32 bit)            │ 031HEX, 11rgBrgA
;                      (8 bit)             │ 032HEX, 11rgBrgA
;                      (32 bit)            │ 033HEX, 11rgBrgA
;  8) cmp  regA,regB   (8 bit)             │ 038HEX, 11rgBrgA
;                      (32 bit)            │ 039HEX, 11rgBrgA
;                      (8 bit)             │ 03AHEX, 11rgBrgA
;                      (32 bit)            │ 03BHEX, 11rgBrgA
;  9) test regA,regB   (8 bit)             │ 084HEX, 11rgBrgA
;                      (32 bit)            │ 085HEX, 11rgBrgA
; 10) xchg regA,regB   (8 bit)             │ 086HEX, 11rgBrgA
;                      (32 bit)            │ 087HEX, 11rgBrgA
;
_@instr_2ereg:  lodsb
                cmp    al,11000000b
                jc     unknown_instr

;_@instr_2ereg_2:
; IFDEF DEBUG
;                push   instr_offset
;                call   make_str_various
; ENDIF
                jmp    _@two_bytes
;
;══════════════════════
; ds:                                      │ 03EHEX
;
 @03E_disasm:   inc    byte [segments+3]
                jmp    @one_byte_junk
;══════════════════════
; fs:                                      │ 064HEX
;
 @064_disasm:   inc    byte [segments+4]
                jmp    @one_byte_junk
;══════════════════════
; gs:                                      │ 065HEX
;
 @065_disasm:   inc    byte [segments+5]
                jmp    @one_byte_junk
;══════════════════════
; push dword                               │ 068HEX, dword
;
 @068_disasm:   sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                jmp   _@five_bytes
;══════════════════════
; 1) nop                                   │ 090HEX
; 2) sti                                   │ 0FBHEX
;
;    инструкции "пустышки", пропускаются дизассемблером
;
 @one_byte_junk:
                pop    eax                       ; вспомним значение esi в eax
                sub    eax,eax
                push   esi                       ; установим новое значение esi
                jmp    disasm_start              ; разберем следующую инструкцию
;══════════════════════
; 1) rol reg,byte (8 bit)                  │ 0C0HEX, 11000reg, byte
; 2) ror reg,byte (8 bit)                  │ 0C0HEX, 11001reg, byte
; 3) rcr reg,byte (8 bit)                  │ 0C0HEX, 11011reg, byte
; 4) shr reg,byte (8 bit)                  │ 0C0HEX, 11101reg, byte
; 5) sal reg,byte (8 bit)                  │ 0C0HEX, 11110reg, byte
; 6) shl reg,byte (8 bit)                  │ 0C0HEX, 11100reg, byte
; 7) sar reg,byte (8 bit)                  │ 0C0HEX, 11111reg, byte
; 8) rcl reg,byte (8 bit)                  │ 0C0HEX, 11010reg, byte
;
 @0C0_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr
                mov    cl,3
                jmp    disasm_ret
;══════════════════════
; 1) rol ereg,byte                         │ 0C1HEX, 11000reg, byte
; 2) ror ereg,byte                         │ 0C1HEX, 11001reg, byte
; 3) rcr ereg,byte                         │ 0C1HEX, 11011reg, byte
; 4) shr ereg,byte                         │ 0C1HEX, 11101reg, byte
; 5) sal ereg,byte                         │ 0C1HEX, 11110reg, byte
; 6) shl ereg,byte                         │ 0C1HEX, 11100reg, byte
; 7) sar ereg,byte                         │ 0C1HEX, 11111reg, byte
; 8) rcl ereg,byte                         │ 0C1HEX, 11010reg, byte
;
 @0C1_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr
                mov    cl,3
                jmp    disasm_ret
;══════════════════════
; 1) rol ereg,1                            │ 0D1HEX, 11000reg
; 2) ror ereg,1                            │ 0D1HEX, 11001reg
; 3) rcr ereg,1                            │ 0D1HEX, 11011reg
; 4) shr ereg,1                            │ 0D1HEX, 11101reg
; 5) sal ereg,1                            │ 0D1HEX, 11110reg
; 6) shl ereg,1                            │ 0D1HEX, 11100reg
; 7) sar ereg,1                            │ 0D1HEX, 11111reg
; 8) rcl ereg,1                            │ 0D1HEX, 11010reg
;
 @0D1_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr
                jmp   _@two_bytes
;══════════════════════
; call dword                               │ 0E8HEX, dword
; jmp  dword                               │ 0E9HEX, dword
;
_@cj_dword:     sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                jmp   _@five_bytes
;══════════════════════
; 1) test ereg,dword                       │ 0F7HEX, 11000reg, dword  
; 2) not  ereg                             │ 0F7HEX, 11010reg
; 3) neg  ereg                             │ 0F7HEX, 11011reg
; 4) mul  ereg                             │ 0F7HEX, 11100reg
; 5) imul ereg                             │ 0F7HEX, 11101reg
; 6) div  ereg не эмулируется              │ 0F7HEX, 11110reg
; 7) idiv ereg не эмулируется              │ 0F7HEX, 11111reg
;
 @0F7_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr       ; < test eax,dword
                cmp    al,11000111b        
                jbe    @0F7_disasm_test    ; <= test edi,dword
;
                cmp    al,11110000b
                jb     _@two_bytes         ; < imul edi
                jmp    unknown_instr
;
 @0F7_disasm_test:
                add    cl,6
                jmp    disasm_ret
;──────────────────────────────────────────────────────────────────────────────
; _@five_bytes - инструкция размером 5 байт, требующая запуска в "карантине"
;
 _@five_bytes:  add    cl,5
                jmp    disasm_ret
;──────────────────────────────────────────────────────────────────────────────
; _@four_bytes - инструкция размером 4 байта, требующая запуска в "карантине"
;
 _@four_bytes:  add    cl,4
                jmp    disasm_ret
;───────────────
; _@two_spec  - инструкция размером 2 байта, требующая полной имитации исполнения
; _@two_bytes - инструкция размером 2 байта, требующая запуска в "карантине"
;
 _@two_bytes:   inc    ecx
                jmp   _@one_byte
 _@two_spec:    inc    ecx
;───────────────
; разбор простейших однобайтовых инструкций
; _@one_spec - инструкция размером 1 байт, требующая полной имитации исполнения
; _@one_byte - инструкция размером 1 байт, требующая запуска в "карантине"
;
 _@one_spec:    sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
;───────────────
 _@one_byte:    inc    ecx
                jmp    disasm_ret
;
;══════════════════════
 include               "disasm/disasm/disasm_000.inc"               
 include               "disasm/disasm/disasm_001.inc"               
 include               "disasm/disasm/disasm_002.inc"               
 include               "disasm/disasm/disasm_003.inc"               
 include               "disasm/disasm/disasm_008.inc"               
 include               "disasm/disasm/disasm_009.inc"               
 include               "disasm/disasm/disasm_00A.inc"               
 include               "disasm/disasm/disasm_00B.inc"               
 include               "disasm/disasm/disasm_00F.inc"               
 include               "disasm/disasm/disasm_010.inc"               
 include               "disasm/disasm/disasm_011.inc"               
 include               "disasm/disasm/disasm_012.inc"               
 include               "disasm/disasm/disasm_013.inc"               
 include               "disasm/disasm/disasm_018.inc"               
 include               "disasm/disasm/disasm_019.inc"               
 include               "disasm/disasm/disasm_01A.inc"               
 include               "disasm/disasm/disasm_01B.inc"               
 include               "disasm/disasm/disasm_020.inc"               
 include               "disasm/disasm/disasm_021.inc"               
 include               "disasm/disasm/disasm_022.inc"               
 include               "disasm/disasm/disasm_023.inc"               
 include               "disasm/disasm/disasm_028.inc"               
 include               "disasm/disasm/disasm_029.inc"               
 include               "disasm/disasm/disasm_02a.inc"               
 include               "disasm/disasm/disasm_02B.inc"               
 include               "disasm/disasm/disasm_030.inc"
 include               "disasm/disasm/disasm_031.inc"
 include               "disasm/disasm/disasm_032.inc"
 include               "disasm/disasm/disasm_033.inc"
 include               "disasm/disasm/disasm_066.inc"               
 include               "disasm/disasm/disasm_067.inc"               
 include               "disasm/disasm/disasm_080.inc"
 include               "disasm/disasm/disasm_081.inc"
 include               "disasm/disasm/disasm_083.inc"
 include               "disasm/disasm/disasm_088.inc"
 include               "disasm/disasm/disasm_089.inc"
 include               "disasm/disasm/disasm_08a.inc"
 include               "disasm/disasm/disasm_08b.inc"
 include               "disasm/disasm/disasm_08d.inc"
 include               "disasm/disasm/disasm_08f.inc"
 include               "disasm/disasm/disasm_0a0-0a3.inc"
 include               "disasm/disasm/disasm_0c6.inc"
 include               "disasm/disasm/disasm_0c7.inc"
 include               "disasm/disasm/disasm_0d3.inc"
 include               "disasm/disasm/disasm_0d8.inc"
 include               "disasm/disasm/disasm_0da.inc"
 include               "disasm/disasm/disasm_0dc.inc"
 include               "disasm/disasm/disasm_0de.inc"
 include               "disasm/disasm/disasm_0f6.inc"
 include               "disasm/disasm/disasm_0fe.inc"
 include               "disasm/disasm/disasm_0ff.inc"
;
 include               "disasm/disasm_coproc.inc"
;══════════════════════
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc set_dreg_eax
;
                inc    dword [dregs]
                mov    dword [dregs+004],eax
                sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; если регистр al содержит данные 00rgArgB (где rgA, rgB два регистра),
; то на выходе edx - rgB
;              eax - rgA
;
 proc two_regs
;
                push   eax
                shr    al,3                      ; - al = regA
                pop    edx
                push   eax
                rol    al,3
                xor    dl,al                     ; - dl = regB
                pop    eax
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
 include                "disasm/disasm/_instr_table.inc"
;IFDEF DEBUG
; include               "disasm/disasm_str.inc"
; include               "disasm/disasm_str_lib.inc"
;ENDIF                      