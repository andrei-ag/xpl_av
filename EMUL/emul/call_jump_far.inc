;───────────────────────
; * * * Имитация исполнения "условных" и "безусловных" переходов:
;
; jo, jno, jb, jnb, je, jne, jbe, ja, js, jns, jp, jnp, jl, jnl, jle, jg FAR
;
 emul_jump_far: 
                mov     al,byte [edi+1]
                sub     al,10h
                call    test_jump
                or      eax,eax
                jnz     spec_emul_ret
;
 emul_jump_far_goto:
                mov     esi,edi
                add     esi,ecx                   ; + instr_size
                add     esi,dword [edi+ecx-4]
;
 emul_goto_far_correct_offset:
                mov     ebx,esi                   ;
                call    test_off                  ; Тестируем полученное смещение
                jc      semul_err_ret
;
                pop     esi
                xchg    esi,ebx                   ; Установим новый EIP
                pop     ebx
                jmp     emulate_check
;───────────────
; * * * Имитация исполнения инструкций: call FAR dword
;                                       jmp  FAR dword
;
;  Необходимо изменить смещение (значение esi) инструкции, которая будет
; следующей исполняться эмулятором.
;  В случае если мы разбираем "call", необходимо сохранить в виртуальном стеке
; смещение инструкции следующей прямо за "call"'ом.
;
 emul_call_far: push    edi
                mov     eax,esi
                add     eax,ecx
                call    put_to_vstack
                pop     edi
                jc      semul_err_ret
                jmp     emul_jump_far_goto