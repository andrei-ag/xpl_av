;───────────────────────
; mov reg,byte ptr [EREG]               │ 08AHEX, 00regERG
; mov reg,byte ptr [esp]                │ 08AHEX, 00reg100,   024HEX
; mov reg,byte ptr [erega+eregb]        │ 08AHEX, 00reg100, 00rgBrgA 
; mov reg,byte ptr [dword]              │ 08AHEX, 00reg101,    dword
;
; mov reg,byte ptr [ereg*2]             │ 08AHEX, 00reg100, 01reg101, 00000000
; mov reg,byte ptr [ereg*2+dword]       │ 08AHEX, 00reg100, 01reg101,    dword
; mov reg,byte ptr [ereg*4]             │ 08AHEX, 00reg100, 10reg101, 00000000
; mov reg,byte ptr [ereg*4+dword]       │ 08AHEX, 00reg100, 10reg101,    dword
; mov reg,byte ptr [ereg*8]             │ 08AHEX, 00reg100, 11reg101, 00000000
; mov reg,byte ptr [ereg*8+dword]       │ 08AHEX, 00reg100, 11reg101,    dword
;
; mov reg,byte ptr [erega+eregb*2]      │ 08AHEX, 00reg100, 01rgBrgA
; mov reg,byte ptr [erega+eregb*4]      │ 08AHEX, 00reg100, 10rgBrgA
; mov reg,byte ptr [erega+eregb*8]      │ 08AHEX, 00reg100, 11rgBrgA
;
; mov reg,byte ptr [ebp]                │ 08AHEX, 01reg101, 00000000 
; mov reg,byte ptr [esp+byte]           │ 08AHEX, 01reg100,   024HEX, byte
; mov reg,byte ptr [EREG+byte]          │ 08AHEX, 01regERG,     byte
; mov reg,byte ptr [erega+eregb+byte]   │ 08AHEX, 01reg100, 00rgBrgA, byte 
;
; mov reg,byte ptr [EREG+dword]         │ 08AHEX, 10regERG,    dword
; mov reg,byte ptr [ESP+dword]          │ 08AHEX, 10reg100,   024HEX, dword
; mov reg,byte ptr [erega+eregb+dword]  │ 08AHEX, 10reg100, 00rgBrgA, dword 
;
 emul_08A: 
                sub     eax,eax
;
                push    ebp
                lea     ebp,[edi+ecx]
                call    emul_count_offset      ; корректируем смещение (если нужно)
                pop     ebp                    ; и разбираем структуру инструкции
                jc      semul_err_ret
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      semul_err_ret
;
                xor     al,ah
                sub     ah,ah
;
                push    eax                ; EAX = ID инструкции
                call    two_regs
                pop     eax 
                xor     al,dl              ; уберем из ereg
;
                mov     ah,al              ; Данные по регистру
                mov     al,byte [edi]      ; ID инструкции
                xor     ah,101b            ; закончим формирование данных по регистру
;
; Формирование инструкции " MOV EREG, DWORD PTR [СМЕЩЕНИЕ] "
; и запуск в "специальных условиях" 
;
                mov     edi,temp_instr_buf
                mov     esi,edi              
                stosw                      ; Перенесем ID инструкции
                mov     eax,ebx            ; EAX = СМЕЩЕНИЕ            
                stosd
;
                sub     ecx,ecx
                mov     cl,6               ; размер инструкции
                call    run_instr          ; выполним инструкцию
;                             
                jmp     spec_emul_ret