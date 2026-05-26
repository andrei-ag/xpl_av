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
;  Проверим полученное смещение, возможно был осуществлен переход
; в другую секцию. Тогда нужно будет откорректировать смещение. 
;
 emul_goto_far_correct_offset:

                sub     esi,[.codeloc] ; МИНУС положение относительно антивируса
                add     esi,[.ip]      ; ПЛЮС начальное положение в памяти
                                       ; ESI - оригинальное смещение текущего участка в памяти
;
                push    esi
                call    real_off       ; * превращаем реальное смещение в смещение относительно антивирусного буфера
                mov     [.ip],esi      ; * изменим оригинальное положение в памяти
                mov     [.codeloc],eax ; *         положение относительно антивирусного буфера
;
                mov     ebx,eax
                call    test_off
                jc      semul_err_ret
;
                pop     esi
                pop     ebx
                xchg    esi,eax
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
 emul_call_far: 
                push    eax edi
;
                mov     eax,esi
                add     eax,ecx
                call    put_to_vstack
                pop     edi eax
                jc      semul_err_ret
;
; Сохраним значения Codeloc и IP что бы использовать при возврате из CALL
;
                push    eax edi
                sub     dword [offset_stack_ptr],8
                mov     edi,dword [offset_stack_ptr]
                mov     eax,[.codeloc]
                stosd
                mov     eax,[.ip]
                stosd            
                pop     edi eax
;
                jmp     emul_jump_far_goto