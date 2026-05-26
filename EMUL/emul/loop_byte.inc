;───────────────
; * * * Имитация исполнения инструкции "loop byte"
;
; 1) Необходимо уменьшить значение виртуельного регистра "ecx" (regs+004) на 1
; 2) Если полученное значение регистра будет равно 0, переходим к смещению
;    данные о котором содержатся в инструкции
;
 emul_loop_byte:
;
                dec     dword [regs+004]       ; уменьшим значение регистра
                cmp     dword [regs+004],0     ; если "ecx" эмулируемого кода
                jz      spec_emul_ret          ; равно 0, выйдем из цикла
;
                inc     esi
                inc     esi                    ; skip instruction (LOOP BYTE)
;
                sub     eax,eax
                mov     al,byte [edi+1]
                xchg    ebx,esi
                call    add_sub_byte_ebx
                pop     esi
                xchg    esi,ebx
                pop     ebx
                jmp     emulate_check