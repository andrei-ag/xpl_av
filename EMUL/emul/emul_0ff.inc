;───────────────────────  
; xxx dword ptr [ereg]                │ 0FFHEX, 00XXXreg
; xxx dword ptr [esp]                 │ 0FFHEX, 00XXX100,   024HEX
; xxx dword ptr [erega+eregb]         │ 0FFHEX, 00XXX100, 00rgBrgA 
; xxx dword ptr [dword]               │ 0FFHEX, 00XXX101,    dword
;
; xxx dword ptr [ereg*2]              │ 0FFHEX, 00XXX100, 01reg101, 00000000                                
; xxx dword ptr [ereg*2+dword]        │ 0FFHEX, 00XXX100, 01reg101,    dword
; xxx dword ptr [ereg*4]              │ 0FFHEX, 00XXX100, 10reg101, 00000000                             
; xxx dword ptr [ereg*4+dword]        │ 0FFHEX, 00XXX100, 10reg101,    dword
; xxx dword ptr [ereg*8]              │ 0FFHEX, 00XXX100, 11reg101, 00000000                                
; xxx dword ptr [ereg*8+dword]        │ 0FFHEX, 00XXX100, 11reg101,    dword
;
; xxx dword ptr [erega+eregb*2]       │ 0FFHEX, 00XXX100, 01rgBrgA
; xxx dword ptr [erega+eregb*4]       │ 0FFHEX, 00XXX100, 10rgBrgA
; xxx dword ptr [erega+eregb*8]       │ 0FFHEX, 00XXX100, 11rgBrgA
;                                
; xxx dword ptr [ebp]                 │ 0FFHEX, 01XXX101, 00000000 
; xxx dword ptr [esp+byte]            │ 0FFHEX, 01XXX100,   024HEX,  byte
; xxx dword ptr [ereg+byte]           │ 0FFHEX, 01XXXreg,     byte
; xxx dword ptr [erega+eregb+byte]    │ 0FFHEX, 01XXX100, 00rgBrgA,  byte 
;
; xxx dword ptr [ereg+dword]          │ 0FFHEX, 10XXXreg,    dword
; xxx dword ptr [ESP+dword]           │ 0FFHEX, 10XXX100,   024HEX, dword
; xxx dword ptr [erega+eregb+dword]   │ 0FFHEX, 10XXX100, 00rgBrgA, dword
;
; LEGEND: 000 - INC, 001 - DEC, 010 - CALL, 011 - CALL FAR,
;         100 - JMP, 101 - ???, 110 - PUSH, 111 - ??? 
;
; call ereg                                │ 0FFHEX, 11010reg
; jmp  ereg                                │ 0FFHEX, 11100reg
; 
 emul_0FF:      
                sub     eax,eax
                lodsb
                lodsb
                cmp     al,11010000b 
                jb      emul_0FF_xxx
;
; call ereg                                │ 0FFHEX, 11010reg
; jmp  ereg                                │ 0FFHEX, 11100reg
;
 emul_0FF_call_jmp_ereg:
                cmp    al,11010000b
                jb     semul_err_ret           ; < call eax
                cmp    al,11010111b
                jbe    emul_0FF_call_ereg      ; <= call edi
;
                cmp    al,11100000b
                jc     semul_err_ret           ; < jmp eax
                cmp    al,11100111b
                jbe    emul_0FF_jmp_ereg       ; <= jmp edi
                jmp    semul_err_ret
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;          

;
; LEGEND: 000 - INC, 001 - DEC, 010 - CALL, 011 - CALL FAR,
;         100 - JMP, 101 - ???, 110 - PUSH, 111 - ??? 
;
 emul_0FF_xxx:  push    edi
                pop     esi
;
                push    ebp
                lea     ebp,[edi+ecx]
                call    emul_count_offset      ; корректируем смещение (если нужно)
                pop     ebp                    ; и разбираем структуру инструкции
                jc      semul_err_ret
;
                push    eax
                xor     al,ah
                sub     ah,ah
                call    two_regs
                xchg    eax,edx             ; DL = Instruction TYPE 
                pop     eax                 ; (INC/DEC/CALL/JMP/PUSH ...)
;
                cmp     dl,101b
                jz      semul_err_ret
                cmp     dl,111b
                jz      semul_err_ret
;
                cmp     dl,001b
                jbe     emul_0FF_inc_dec_build_instr
                cmp     dl,011b
                jbe     emul_0FF_call_build_instr
                cmp     dl,100b
                jz      emul_0FF_jmp_build_instr
                jmp     emul_0FF_push_build_instr
;═══════════════
; INC/DEC DWORD PTR [ ... ]
;
 emul_0FF_inc_dec_build_instr:
		call    calc_offset            ; получим итоговое смещение
                call    test_off
                jc      semul_err_ret
;
; Формирование инструкции " INC/DEC BYTE/DWORD PTR [СМЕЩЕНИЕ] "
; и запуск в "специальных условиях" 
;
                mov     al,byte [edi]
                mov     edi,temp_instr_buf
                mov     esi,edi
                stosb
;
                mov     al,dl
                shl     al,3
                xor     al,101b
                stosb                          ; Перенесем ID инструкции
;                                              
                mov     eax,ebx                       
                stosd                          ; Перенесем смещение 
;
                sub     ecx,ecx
                mov     cl,6                   ; размер инструкции
                call    run_instr              ; выполним инструкцию
                jmp     spec_emul_ret
;═══════════════
; CALL / CALL FAR DWORD PTR [ ... ] 
;
 emul_0FF_call_build_instr:
		call    calc_offset            ; получим итоговое смещение
                call    test_off
                jc      semul_err_ret

                push    edi
                mov     eax,edi
                add     eax,ecx             ; EAX = Смещение сразу после инструкции CALL
                call    put_to_vstack
                pop     edi
;
                mov     ebx,dword [ebx]     ; ESI Смещение для перехода (в памяти программы)
;
 emul_0FF_call_save_ip:
		call    calc_offset            ; получим итоговое смещение
		call    test_off
                jc      emul_0FF_check_API_call
;
                pop     esi
                mov     esi,ebx
                pop     ebx
                jmp     emulate_check
;
; API call
;
 emul_0FF_check_API_call:
                mov     eax,ebx                ; Может быть вызывается API ?
                call    check_if_imported_api_call
                jnc     semul_err_ret
;
                call    emul_API_call
                jc      semul_err_ret
                jmp     emul_ret               ; Выполним выход из API
;═══════════════
; JMP DWORD PTR [ ... ]
;
 emul_0FF_jmp_build_instr:
		call    calc_offset            ; получим итоговое смещение
                call    test_off
                jc      semul_err_ret
;
                mov     ebx,dword [ebx]
                jmp     emul_0FF_jmp_ereg
;═══════════════
; PUSH DWORD PTR [ ... ]
;
 emul_0FF_push_build_instr:
		call    calc_offset            ; получим итоговое смещение
                call    test_off
                jc      semul_err_ret
;
                mov     eax,dword [ebx]
                call    put_to_vstack
                jc      semul_err_ret
                jmp     spec_emul_ret
;═══════════════
; " call ereg "
;
 emul_0FF_call_ereg:
                mov     eax,edi
                add     eax,ecx             ; EAX = Смещение сразу после инструкции CALL
                call    put_to_vstack
                jc      semul_err_ret
;
                mov     esi,ebx             ; смещение для перехода (виртуальное)
                jmp     emul_0FF_call_save_ip
;═══════════════
; " jmp ereg "
;
 emul_0FF_jmp_ereg:
		call    calc_offset            ; получим итоговое смещение
                call    test_off
                jc      semul_err_ret
;
                pop     esi
                mov     esi,ebx
                pop     ebx
                jmp     emulate_check