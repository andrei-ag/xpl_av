;────────────────────────
; * * * Эмуляция инструкций PUSH/POP
;
 emul_push_pop: mov     ebx,dword [regs+016]      ; EBX = Virtual ESP
                call    test_off
                jc      emul_push_pop_error       ; Указатель расположен вне 
                                                  ; виртуального стека и загруженной программы
;
                sub     eax,eax
                lodsb
                mov     ah,4                      ; size of saved data
                cmp     al,00Eh
                jz      emul_push                 ; " push cs "
                cmp     al,01Eh
                jz      emul_push                 ; " push ds "
;
                cmp     al,050h
                jb      semul_err_ret
                cmp     al,057h
                jbe     emul_push                 ; " push ereg "
                cmp     al,05fh
                jbe     emul_pop                  ; " pop ereg "
;
                mov     ah,20h                    ; size of saved data
                cmp     al,060h
                jz      emul_push                 ; " pushad "
                cmp     al,061h
                jz      emul_pop                  ; " popad "
;
                mov     ah,4                      ; size of saved data
                cmp     byte [edi],066h           ; Save WORD in stack? 
                jnz     emul_push_pop_2
                sub     ah,2                      ; new size of saved data
 emul_push_pop_2:
                cmp     al,068h
                jz      emul_push                 ; " push dword "
                cmp     al,06Ah
                jz      emul_push                 ; " push byte "
                jmp     semul_err_ret
;
;  Проверим находится ли виртуальный регистр ESP в области стека или
; загруженной программы и достаточно ли в этой области места для выполнения
; инструкции PUSH или POP
;
 emul_push:     mov     al,ah
                sub     ah,ah
                sub     ebx,eax                   ; PUSH
                jmp     emul_push_pop_build_instr

 emul_pop:      mov     al,ah
                sub     ah,ah
                add     ebx,eax                   ; POP
;
 emul_push_pop_build_instr:
                call    test_off
                jc      emul_push_pop_error       ; Указатель расположен вне 
                                                  ; виртуального стека и загруженной программы
;                
                mov     esi,edi
                mov     edi,temp_instr_buf
                push    edi ecx
                rep     movsb              ; Перенесем инструкцию в спец. буфер
                pop     ecx esi
                call    run_instr          ; выполним инструкцию
                jmp     spec_emul_ret
;
 emul_push_pop_error:
                mov     eax,-3             ; ERROR
                jmp     semul_err_ret