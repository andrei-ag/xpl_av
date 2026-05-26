;───────────────────────
; mov al,byte ptr [dword]                │ 0A0HEX,    dword
; mov eax,dword ptr [dword]              │ 0A1HEX,    dword
; mov byte ptr [dword],al                │ 0A2HEX,    dword
; mov dword ptr [dword],eax              │ 0A3HEX,    dword
;
 emul_0AX:
                mov     ebx,dword [edi+1]
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      semul_err_ret
;
; Формирование инструкции
;
                mov     al,byte [edi]      ; ID инструкции
                mov     edi,temp_instr_buf
                mov     esi,edi              
                stosb                      ; Перенесем ID инструкции
                mov     eax,ebx            ; EAX = СМЕЩЕНИЕ            
                stosd
;
                sub     ecx,ecx
                mov     cl,5               ; размер инструкции
                call    run_instr          ; выполним инструкцию
;                             
                jmp     spec_emul_ret