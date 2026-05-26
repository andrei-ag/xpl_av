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
 disasm_nfo            db 'disASM%003'           ; версия дизассемблера
;
 disasm_init:   push   edi
                push   esi
;
                mov    esi,[instr_offset]        ; esi - смещение инструкции
 
 disasm_start:  mov    dword [dregs],0           ; инициализация буфера "dregs"
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
                cmp    al,000h
                jz     @000_disasm
                cmp    al,001h
                jz     @001_disasm
                cmp    al,002h
                jz    _@instr_2ereg              ; " add rega,regb " (8 bit)
                cmp    al,003h
                jz     @003_disasm
 @add_al_byte:  cmp    al,004h
                jz    _@two_bytes                ; " add al, byte "
 @add_eax_dword:cmp    al,005h                   
                jz    _@five_bytes               ; " add eax, dword "
;───────────────
                cmp    al,008h
                jz     @008_disasm
                cmp    al,009h
                jz     @009_disasm
                cmp    al,00Ah
                jz     @00A_disasm
                cmp    al,00Bh
                jz    _@instr_2ereg              ; " or rega,regb " (32 bit)
 @or_eax_dword: cmp    al,00Dh
                jz    _@five_bytes               ; " or eax,dword " (32 bit)
 @push_cs:      cmp    al,00Eh
                jz    _@one_spec                 ; " push cs "
                cmp    al,00Fh
                jz     @00F_disasm
                cmp    al,010h
                jz     @010_disasm
                cmp    al,011h
                jz    _@instr_2ereg              ; " adc rega,regb " (32 bit)
                cmp    al,012h
                jz    _@instr_2ereg              ; " adc rega,regb " (8 bit)
                cmp    al,013h
                jz    _@instr_2ereg              ; " adc rega,regb " (32 bit)
 @adc_eax_dword:cmp    al,015h                  
                jz    _@five_bytes               ; " adc eax, dword "
 @sbb_reg_reg:  cmp    al,018h
                jz    _@instr_2ereg              ; " sbb rega, regb " (8 bit)
                cmp    al,019h
                jz     @019_disasm
                cmp    al,01Ah
                jz    _@instr_2ereg              ; " sbb rega, regb " (8 bit)
                cmp    al,01Bh
                jz     @01B_disasm
 @sbb_al_byte:  cmp    al,01Ch                   
                jz    _@two_bytes                ; " sbb al, byte "
 @sbb_eax_dword:cmp    al,01Dh                   
                jz    _@five_bytes               ; " sbb eax, dword "
 @push_ds:      cmp    al,01Eh
                jz    _@one_spec                 ; " push ds "
;───────────────
                cmp    al,020h
                jz     @020_disasm
                cmp    al,021h
                jz     @021_disasm
                cmp    al,022h
                jz    _@instr_2ereg              ; " and rega, regb " (8 bit)
                cmp    al,023h
                jz    _@instr_2ereg              ; " and rega, regb " (32 bit)
                cmp    al,024h
                jz    _@two_bytes                ; " and al, byte "
 @and_eax_dword:cmp    al,025h                   
                jz    _@five_bytes               ; " and eax, dword "
 @sub_reg_reg:  cmp    al,028h
                jz    _@instr_2ereg              ; " sub rega, regb " (8 bit)
                cmp    al,029h
                jz     @029_disasm
                cmp    al,02Ah
                jz    _@instr_2ereg              ; " sub rega, regb " (8 bit)
                cmp    al,02Bh
                jz     @02B_disasm
 @sub_al_byte:  cmp    al,02Ch                   
                jz    _@two_bytes                ; " sub al, byte "
 @sub_eax_dword:cmp    al,02Dh                   
                jz    _@five_bytes               ; " sub eax, dword "
 @xor_reg_reg:  cmp    al,030h
                jz     @030_disasm
                cmp    al,031h
                jz     @031_disasm              
                cmp    al,032h                    
                jz     @032_disasm              
		cmp    al,033h
                jz     @033_disasm              
 @xor_al_byte:  cmp    al,034h
                jz    _@two_bytes                ; " xor al, byte "
 @xor_eax_dword:cmp    al,035h                   
                jz    _@five_bytes               ; " xor eax, dword "
 @cmp_reg_reg:  cmp    al,038h
                jz    _@instr_2ereg              ; " cmp rega, regb " (8 bit)
                cmp    al,039h
                jz    _@instr_2ereg              ; " cmp rega, regb " (32 bit)
                cmp    al,03Ah
                jz    _@instr_2ereg              ; " cmp rega, regb " (8 bit)
                cmp    al,03Bh
                jz    _@instr_2ereg              ; " cmp rega, regb " (32 bit)
 @cmp_al_byte:  cmp    al,03Ch                   
                jz    _@two_bytes                ; " cmp al, byte "
 @cmp_eax_dword:cmp    al,03Dh                   
                jz    _@five_bytes               ; " cmp eax, dword "
 @ds:           cmp    al,03Eh
                jz    @03E_disasm                ; " ds: "
;
; Промежуток " 40h - 5Fh "
;
; LEGEND: from 40h - INC  EAX till 47h
;         from 48h - DEC  EAX till 4Fh
;         from 50h - PUSH EAX till 57h
;              58h - POP  EAX till 5Fh
;              60h - pushad
;              61h - popad  
;
 @inc_ereg:     cmp    al,040h                   
                jb     unknown_instr             ; < inc EAX
                cmp    al,04Fh
                jbe    _@one_byte                ; <= dec EDI
                cmp    al,061h
                jbe    _@one_spec                ; < pushad
 @fs:           cmp    al,064h
                jz     @03E_disasm                ; " fs: "
 @gs:           cmp    al,065h
                jz     @03E_disasm                ; " gs: "
                cmp    al,066h
                jz     @066_disasm
;───────────────
 @push_dword:   cmp    al,068h
                jz     @068_disasm
;
 @push_byte:    cmp    al,06Ah
                jz     _@two_spec                 ; " push byte "
                jb     unknown_instr
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
                jz    @081_disasm
                cmp    al,083h
                jz    @083_disasm
;───────────────
                cmp    al,084h
                jz    _@instr_2ereg              ; " test rega,regb " (8 bit) 
                cmp    al,085h
                jz    _@instr_2ereg              ; " test rega,regb " (32 bit) 
                cmp    al,086h
                jz    _@instr_2ereg              ; " xchg rega,regb " (8 bit)
                cmp    al,087h
                jz    _@instr_2ereg              ; " xchg rega,regb " (32 bit)
;
                cmp    al,088h                   
                jz    @088_disasm                
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
 @wait:         cmp    al,09Bh
                jz    _@one_byte                 ; " wait "
 @pushfd:       cmp    al,09Ch
                jz    _@one_byte                 ; " pushfd "
 @popfd:        cmp    al,09Dh
                jz    _@one_byte                 ; " popfd "
 @sahf:         cmp    al,09Eh
                jz    _@one_byte                 ; " sahf "
 @lahf:         cmp    al,09Fh
                jz    _@one_byte                 ; " lahf "
 @cmpsb:        cmp    al,0A6h
                jz    _@one_spec                 ; " cmpsb "
 @cmpsd:        cmp    al,0A7h
                jz    _@one_spec                 ; " cmpsd "
 @stosb:        cmp    al,0AAh
                jz    _@one_spec                 ; " stosb "
 @stosd:        cmp    al,0ABh
                jz    _@one_spec                 ; " stosd "
 @lodsb:        cmp    al,0ACh
                jz    _@one_spec                 ; " lodsb "
 @lodsd:        cmp    al,0ADh
                jz    _@one_spec                 ; " lodsd "
; @scasb:        cmp    al,0AEh
;                jz    _@one_spec                 ; " scasb "
; @scasd:        cmp    al,0AFh
;                jz    _@one_spec                 ; " scasd "
;───────────────
 @mov_ereg_byte:
                cmp    al,0B0h                   ;   10110reg, byte
                jc     unknown_instr
                cmp    al,0B7h
                jbe   _@two_bytes                ; " mov ereg, byte "
;───────────────
 @mov_ereg_dword:
                cmp    al,0B8h                   ;   10111reg, dword
                jc     unknown_instr             ; < mov eax, dword
                cmp    al,0BFh
                jbe   _@five_bytes               ; " mov ereg,dword "
;───────────────
                cmp    al,0C1h
                jz    @0C1_disasm
 @ret_byte:     cmp    al,0C3h
                jz    _@one_spec                 ; " ret "      
                cmp    al,0C7h
                jz    @0C7_disasm
		cmp    al,0D1h
	        jz    @0D1_disasm
                cmp    al,0D8h
                jz    @0D8_disasm
                cmp    al,0D9h
                jz    @0D9_disasm
                cmp    al,0DAh
                jz    @0DA_disasm
                cmp    al,0DBh
                jz    @0DB_disasm
                cmp    al,0DDh
                jz    @0DD_disasm
                cmp    al,0DEh
                jz    @0DE_disasm
 @loop_byte:    cmp    al,0E2h
                jz     _@two_spec
 @call_dword:   cmp    al,0E8h                   ; ─
                jz    _@cj_dword                 ; инструкции call и jmp
 @jmp_dword:    cmp    al,0E9h                   ; требуют одинакового разбора
                jz    _@cj_dword                 ; ─
 @jmp_byte:     cmp    al,0EBh
                jz    _@two_spec
 @cmc:          cmp    al,0F5h
                jz    _@one_byte                 ; " cmc "
                cmp    al,0F6h
                jz    @0F6_disasm
                cmp    al,0F7h
                jz    @0F7_disasm
 @clc:          cmp    al,0F8h
                jz    _@one_byte                 ; " clc "
 @stc:          cmp    al,0F9h
                jz    _@one_byte                 ; " stc "
 @cli:          cmp    al,0FAh
                jz    _@one_byte                 ; " cli "
 @sti:          cmp    al,0FBh
;                jz    _@one_byte                 ; " sti "
; ПОКА ПРОПУСКАЕТСЯ КАК "МУСОР"
                jz    @03E_disasm                ; " nop "
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
 disasm_ret:    pop    esi
                pop    edi
                ret
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
; 1) ds:                                   │ 03EHEX
; 2) gs:                                   │ 065HEX
; 3) nop                                   │ 090HEX
; 4) fs:                                     064HEX
;
;    инструкции "пустышки", пропускаются дизассемблером
;
 @03E_disasm:   pop    eax                       ; вспомним значение esi в eax
                push   esi                       ; установим новое значение esi
                jmp    disasm_start              ; разберем следующую инструкцию
;══════════════════════
; push dword                               │ 068HEX, dword
;
 @068_disasm:   sub    edx,edx
                inc    edx                       ; edx = 1 - полная эмуляция
                jmp   _@five_bytes
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
; 6) div  ereg                             │ 0F7HEX, 11110reg
; 7) idiv ereg                             │ 0F7HEX, 11111reg
;
 @0F7_disasm:   lodsb
                cmp    al,11010000b        
                jae    _@two_bytes         ; >= not eax
;
                cmp    al,11000000b
                jb     unknown_instr       ; < test eax,dword
                cmp    al,11000111b
                ja     unknown_instr       ; > test edi,dword
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
 include               "emul/disasm/disasm_000.inc"               
 include               "emul/disasm/disasm_001.inc"               
 include               "emul/disasm/disasm_003.inc"               
 include               "emul/disasm/disasm_008.inc"               
 include               "emul/disasm/disasm_009.inc"               
 include               "emul/disasm/disasm_00A.inc"               
 include               "emul/disasm/disasm_00F.inc"               
 include               "emul/disasm/disasm_010.inc"               
 include               "emul/disasm/disasm_019.inc"               
 include               "emul/disasm/disasm_01B.inc"               
 include               "emul/disasm/disasm_020.inc"               
 include               "emul/disasm/disasm_021.inc"               
 include               "emul/disasm/disasm_029.inc"               
 include               "emul/disasm/disasm_02B.inc"               
 include               "emul/disasm/disasm_030.inc"
 include               "emul/disasm/disasm_031.inc"
 include               "emul/disasm/disasm_032.inc"
 include               "emul/disasm/disasm_033.inc"
 include               "emul/disasm/disasm_066.inc"               
 include               "emul/disasm/disasm_080.inc"
 include               "emul/disasm/disasm_081.inc"
 include               "emul/disasm/disasm_083.inc"
 include               "emul/disasm/disasm_088.inc"
 include               "emul/disasm/disasm_089.inc"
 include               "emul/disasm/disasm_08a.inc"
 include               "emul/disasm/disasm_08b.inc"
 include               "emul/disasm/disasm_08d.inc"
 include               "emul/disasm/disasm_08f.inc"
 include               "emul/disasm/disasm_0c7.inc"
 include               "emul\disasm/disasm_0f6.inc"
 include               "emul/disasm/disasm_0fe.inc"
 include               "emul/disasm/disasm_0ff.inc"
;
 include               "emul/disasm_coproc.inc"
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

;IFDEF DEBUG
; include               emul\disasm_str.inc
; include               emul\disasm_str_lib.inc
;ENDIF                      