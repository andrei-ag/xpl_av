;───────────────────────
; Prefix 67h - Change default ADDRESS size.
;
; mov al,byte ptr [word]                │ 067HEX, 0A0HEX,    word
; mov eax,dword ptr [word]              │ 067HEX, 0A1HEX,    word
; mov byte ptr [word],al                │ 067HEX, 0A2HEX,    word
; mov dword ptr [word],eax              │ 067HEX, 0A3HEX,    word
;
; 08B:
;      mov ereg,dword ptr [word]        │ 067HEX, 08BHEX, 00reg101,    word
;
 emul_067:
                sub     ebx,ebx
                mov     bx,word [edi+ecx-2]
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
                sub     ecx,ecx
                cmp     byte [edi+1],08bh
                jz      emul_067_set_08b
;
                mov     al,byte [edi+1]    ; ID инструкции
                mov     edi,temp_instr_buf
                mov     esi,edi              
                stosb                      ; Перенесем ID инструкции
                jmp     emul_067_set_offset
;
 emul_067_set_08b:
                inc     ecx
                mov     ax,word [edi+1]    ; ID инструкции
                xor     ah,00000011b
                mov     edi,temp_instr_buf
                mov     esi,edi              
                stosw                      ; Перенесем ID инструкции
;
 emul_067_set_offset:
                mov     eax,ebx            ; EAX = СМЕЩЕНИЕ            
                stosd
;
                add     cl,5               ; размер инструкции
                call    run_instr          ; выполним инструкцию
;                             
                jmp     spec_emul_ret