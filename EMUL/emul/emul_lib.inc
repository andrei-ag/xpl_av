;─────────────────────────────────────────────────────────────────────────────
; Эмуляция инструкций структуры:
; 
; xxx byte ptr [EREG],reg              │ XXX, 00regERG
; xxx byte ptr [esp],reg               │ XXX, 00reg100,   024HEX
; xxx byte ptr [erega+eregb],reg       │ XXX, 00reg100, 00rgBrgA 
; xxx byte ptr [dword],reg             │ XXX, 00reg101,    dword
;
; xxx byte ptr [ereg*2],reg            │ XXX, 00reg100, 01reg101, 00000000
; xxx byte ptr [ereg*2+dword],reg      │ XXX, 00reg100, 01reg101,    dword
; xxx byte ptr [ereg*4],reg            │ XXX, 00reg100, 10reg101, 00000000
; xxx byte ptr [ereg*4+dword],reg      │ XXX, 00reg100, 10reg101,    dword
; xxx byte ptr [ereg*8],reg            │ XXX, 00reg100, 11reg101, 00000000
; xxx byte ptr [ereg*8+dword],reg      │ XXX, 00reg100, 11reg101,    dword
;
; xxx byte ptr [erega+eregb*2],reg     │ XXX, 00reg100, 01rgBrgA
; xxx byte ptr [erega+eregb*4],reg     │ XXX, 00reg100, 10rgBrgA
; xxx byte ptr [erega+eregb*8],reg     │ XXX, 00reg100, 11rgBrgA
;
; xxx byte ptr [ebp],reg               │ XXX, 01reg101, 00000000 
; xxx byte ptr [esp+byte],reg          │ XXX, 01reg100,   024HEX, byte
; xxx byte ptr [EREG+byte],reg         │ XXX, 01regERG,     byte
; xxx byte ptr [erega+eregb+byte],reg  │ XXX, 01reg100, 00rgBrgA, byte 
;
; xxx byte ptr [EREG+dword],reg        │ XXX, 10regERG,    dword
; xxx byte ptr [ESP+dword],reg         │ XXX, 10reg100,   024HEX, dword
; xxx byte ptr [erega+eregb+dword],reg │ XXX, 10reg100, 00rgBrgA, dword
;
; Сокращенно: "XXX BYTE PTR [...],REG" или "XXX REG, BYTE PTR [...]"
;
; on start: "id"    - ID инструкции (2 байта)
;           ECX     - Размер инструкции
;           EBX     - Содержит смещение из регистров
;           ESI=EDI - Указывает на начало инструкции
;             
; on exit : cf=0 - эмуляция прошла успешно
;           cf=1 - ошибка в ходе эмуляции
;
 proc emul_xxx_byte     id
;
                push    ebp
                lea     ebp,[edi+ecx]
                call    emul_count_offset  ; посчитаем смещение
                                           ; с учетом структуры инструкции
                pop     ebp
                jc      emul_xxx_byte_err_ret

		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_xxx_byte_err_ret
;
; Вытащим из регистра (в описании фигурирует как reg) ключ
;
 emul_xxx_byte_get_key:
                xor     al,ah
                sub     ah,ah
                call    reg8_pos
                mov     dl,byte [regs+eax]
;
; Формирование инструкции " XXX BYTE PTR [СМЕЩЕНИЕ],КЛЮЧ " (7 байт)
; и запуск в "специальных условиях" 
;                   
                mov     edi,temp_instr_buf
                mov     esi,edi                
                mov     eax,[id]           ; Идентификатор инструкции
                stosw
                mov     eax,ebx            ; СМЕЩЕНИЕ            
                stosd
                mov     eax,edx            ; КЛЮЧ
                stosb                      ; (BYTE-key size)
;
                sub     ecx,ecx
                mov     cl,2+4+1           ; размер инструкции
                call    run_instr          ; выполним инструкцию
;
                clc
                ret
;
 emul_xxx_byte_err_ret:
                stc
                ret
;
 endp
;─────────────────────────────────────────────────────────────────────────────
; Эмуляция инструкций структуры:
; 
; xxx dword ptr [EREG],ereg              │ XXX, 00regERG
; xxx dword ptr [esp],ereg               │ XXX, 00reg100,   024HEX
; xxx dword ptr [erega+eregb],ereg       │ XXX, 00reg100, 00rgBrgA 
; xxx dword ptr [dword],ereg             │ XXX, 00reg101,    dword
;
; xxx dword ptr [ereg*2],ereg            │ XXX, 00reg100, 01reg101, 00000000
; xxx dword ptr [ereg*2+dword],ereg      │ XXX, 00reg100, 01reg101,    dword
; xxx dword ptr [ereg*4],ereg            │ XXX, 00reg100, 10reg101, 00000000
; xxx dword ptr [ereg*4+dword],ereg      │ XXX, 00reg100, 10reg101,    dword
; xxx dword ptr [ereg*8],ereg            │ XXX, 00reg100, 11reg101, 00000000
; xxx dword ptr [ereg*8+dword],ereg      │ XXX, 00reg100, 11reg101,    dword
;
; xxx dword ptr [erega+eregb*2],ereg     │ XXX, 00reg100, 01rgBrgA
; xxx dword ptr [erega+eregb*4],ereg     │ XXX, 00reg100, 10rgBrgA
; xxx dword ptr [erega+eregb*8],ereg     │ XXX, 00reg100, 11rgBrgA
;
; xxx dword ptr [ebp],ereg               │ XXX, 01reg101, 00000000 
; xxx dword ptr [esp+byte],ereg          │ XXX, 01reg100,   024HEX, byte
; xxx dword ptr [EREG+byte],ereg         │ XXX, 01regERG,     byte
; xxx dword ptr [erega+eregb+byte],ereg  │ XXX, 01reg100, 00rgBrgA, byte 
;
; xxx dword ptr [EREG+dword],ereg        │ XXX, 10regERG,    dword
; xxx dword ptr [ESP+dword],ereg         │ XXX, 10reg100,   024HEX, dword
; xxx dword ptr [erega+eregb+dword],ereg │ XXX, 10reg100, 00rgBrgA, dword 
;
; Сокращенно: "XXX DWORD PTR [...],EREG" или "XXX EREG, DWORD PTR [...]"
;
; on start: "id"    - ID инструкции (2 байта)
;           ECX     - Размер инструкции
;           EBX     - Содержит смещение из регистров
;           ESI=EDI - Указывает на начало инструкции
;             
; on exit : cf=0 - эмуляция прошла успешно
;           cf=1 - ошибка в ходе эмуляции
;
 proc emul_xxx_dword    id
;
                push    ebp
                lea     ebp,[edi+ecx]
                call    emul_count_offset  ; посчитаем смещение
                                           ; с учетом структуры инструкции
                pop     ebp
                jc      emul_xxx_dword_err_ret

		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_xxx_dword_err_ret
;
; Вытащим из регистра (в описании фигурирует как ereg) ключ
;
                xor     al,ah
                sub     ah,ah
                call    two_regs
                call    mul_4
                mov     edx,dword [regs+eax]
;
; Формирование инструкции " XXX DWORD PTR [СМЕЩЕНИЕ],КЛЮЧ " (10 байт)
; и запуск в "специальных условиях" 
;                   
                mov     edi,temp_instr_buf
                mov     esi,edi                
                mov     eax,[id]           ; Идентификатор инструкции
                stosw
                mov     eax,ebx            ; СМЕЩЕНИЕ            
                stosd
                mov     eax,edx            ; КЛЮЧ
                stosd
;
                sub     ecx,ecx
                mov     cl,2+4+4           ; размер инструкции
                call    run_instr          ; выполним инструкцию
;
                clc
                ret
;
 emul_xxx_dword_err_ret:
                stc
                ret
;
 endp
;─────────────────────────────────────────────────────────────────────────────
; emul_count_offset - Расчет смещения используемого инструкцией,
;                     а так же разбор ее структуры
;
; Работает с инструкциями вида:
;
;  1) XXX BYTE PTR [...],REG
;  2) XXX BYTE PTR [...],BYTE
;  2) XXX BYTE PTR [...]
;
;  4) XXX DWORD PTR [...],EREG
;  5) XXX DWORD PTR [...],DWORD
;  6) XXX DWORD PTR [...]
;
;  7) XXX REG, BYTE PTR [...]
;  8) XXX EREG, DWORD PTR [...]
;
; on start: ECX     - Размер инструкции
;           EBX     - Содержит смещение из регистров
;           EBP     - Смещение по которому можно получить
;                     корректировку к смещению след образом:
;                     * "EBP-1" - BYTE
;                     * "EBP-4" - DWORD
;           ESI=EDI - Указывает на начало инструкции
;             
; on exit : CF=0    - корректировка смещения прошла успешно
;           CF=1    - ошибка
;
 proc emul_count_offset     
;
                lodsb                      ; EAX = ID инструкции
                lodsb                      ; EAX = регистры
;
; *** GET INSTRUCTION TYPE ***
;
 
;
; ... byte/word/dword ptr [EREG] ...              │ XXX, 00regERG
; ... byte/word/dword ptr [esp] ....              │ XXX, 00reg100,   024HEX
; ... byte/word/dword ptr [erega+eregb] ...       │ XXX, 00reg100, 00rgBrgA 
; ... byte/word/dword ptr [ereg*2/4/8] ...        │ XXX, 00reg100, ??reg101, 00000000
; ... byte/word/dword ptr [ereg*2/4/8+dword] ...  │ XXX, 00reg100, ??reg101,    dword
; ... byte/word/dword ptr [erega+eregb*2/4/8] ... │ XXX, 00reg100, ??rgBrgA
; ... byte/word/dword ptr [dword] ...             │ XXX, 00reg101,    dword
;
                sub     ah,ah
                cmp     al,00111111b
                jbe     emul_count_offset_1       ; one of listed types
;
; ... byte/word/dword ptr [ebp] ...               │ XXX, 01reg101, 000000000
; ... byte/word/dword ptr [EREG+byte] ...         │ XXX, 01regERG,     byte
; ... byte/word/dword ptr [erega+eregb+byte] ...  │ XXX, 01reg100, 00rgBrgA, byte
;
                mov     ah,01000000b              ; ID инструкции
                cmp     al,01111111b
                jbe     emul_count_offset_byte    ; one of listed types
;
; ... byte/word/dword ptr [EREG+dword] ...        │ XXX, 10regERG,    dword
; ... byte/word/dword ptr [ESP+dword] ...         │ XXX, 10reg100,   024HEX, dword
; ... byte/word/dword ptr [erega+eregb+dword] ... │ XXX, 10reg100, 00rgBrgA, dword 
;
                mov     ah,10000000b              ; ID инструкции
                cmp     al,10111111b
                jbe     emul_count_offset_dword   ; one of listed types
;
                stc                               ; Unknown instruction type
                ret
;----------------
 emul_count_offset_1:
                push    eax
                call    two_regs
                pop     eax
                cmp     dl,101b
                jz      emul_count_offset_to_dword ; ... byte/word/dword ptr [dword] ...
                cmp     dl,100b
                jnz     emul_count_offset_mov
;
; ... byte/word/dword ptr [esp] ....              │ XXX, 00reg100,   024HEX
; ... byte/word/dword ptr [erega+eregb] ...       │ XXX, 00reg100, 00rgBrgA 
;
; ... byte/word/dword ptr [ereg*2/4/8] ...        │ XXX, 00reg100, ??reg101, 00000000
; ... byte/word/dword ptr [ereg*2/4/8+dword] ...  │ XXX, 00reg100, ??reg101,    dword
; ... byte/word/dword ptr [erega+eregb*2/4/8] ... │ XXX, 00reg100, ??rgBrgA
;
                cmp     byte [esi],00111111b
                jbe     emul_count_offset_mov ;    ... byte/word/dword ptr [esp] ...
                                              ; OR ... byte/word/dword ptr [erega+eregb] ...
                push    eax
                mov     al,byte [esi]
                call    set_off_first_two_bits
                call    two_regs
                cmp     dl,101b
                pop     eax
                jnz     emul_count_offset_mov
;                
                jmp     emul_count_offset_dword ; = ... byte/word/dword ptr [ereg*2/4/8+dword] ...
;
; *** CALCULATE OFFSET ***
;
 emul_count_offset_byte:                        ; + BYTE
                push    eax
                sub     eax,eax
                mov     al,byte [ebp-1]
                call    add_sub_byte_ebx
                pop     eax
                jmp     emul_count_offset_mov
;
 emul_count_offset_dword:                       ; + DWORD
                add     ebx,dword [ebp-4]
                jmp     emul_count_offset_mov
;
 emul_count_offset_to_dword:                    ; = DWORD
                mov     ebx,dword [ebp-4]
;
 emul_count_offset_mov:
                clc     
                ret
;
 endp
;─────────────────────────────────────────────────────────────────────────────