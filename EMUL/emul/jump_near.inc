; * * * Имитация исполнения "условных" и "безусловных" переходов:
;
;       jo, jno, jb, jnb, je, jne, jbe, ja, js,
;       jns, jp, jnp, jl, jnl, jle, jg & jmp NEAR
;
; 1) Перенесем в буфер "instr_buf" инструкции для имитации исполнения перехода.
;    В результате получим в "instr_buf" :
;
;               инструкция для (без)условного перехода к метке _label
;               inc     eax
; _label:       ret
;
; 2) установим значение флагов эмулируемого кода (из переменной "flags")
;    обнулим регистр eax
; 3) запустим на исполнение содержимое "instr_buf"
;    в том случае, если переход выполнится, значение eax будет равно нулю
;
;  Если переход выполнился, соотвественно необходимо изменить смещение,
; исследуемого кода (значение регистра esi). Для этого рассмотрим второй байт
; инструкции, который содержит данные для вычисления нового смещения.
;
 emul_jmp_near: 
                mov     al,byte [edi]
                call    test_jump
                or      eax,eax
                jnz     spec_emul_ret
;───────────────
                add     esi,ecx                   ; + instr_size
                mov     ebx,esi
                cmp     byte [edi+1],07fh
                jna     emul_jmp_near_goto_down
;
 emul_jmp_near_goto_up:
                sub     al,byte [edi+1]
                sub     ebx,eax
                jmp     emul_jmp_near_test_offset
;
 emul_jmp_near_goto_down:
                mov     al,byte [edi+1]
                add     ebx,eax
;
 emul_jmp_near_test_offset:
                call    test_off
                jc      semul_err_ret
;
                pop     esi
                mov     esi,ebx
                pop     ebx
                jmp     emulate_check
;═══════════════
 proc test_jump
;    
                mov     ah,1
                push    edi
                mov     edi,instr_buf
                stosw
                mov     ax,0c340h
                stosw
                pop     edi
;───────────────
 jmp_ser_eflag: pushfd
                pushfd
                mov     eax,dword [flags]
                mov     dword [esp],eax
                sub     eax,eax
                popfd
                call    instr_buf
                popfd
                ret
;
 endp