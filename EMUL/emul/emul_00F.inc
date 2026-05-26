;───────────────────────
; movzx ereg,byte ptr [rega+regb]          │ 00FHEX, 0B6HEX, 00reg100, 00rgBrgA
;
; jne jo, jno, jb, jnb, je, jne,           │ 00FHEX, 080 ... 08FHEX, dword
; jbe, ja, js, jns, jp, jnp, jl,
; jnl, jle, jg FAR dword
;
 emul_00F:      
                lodsb                      ; First byte ("00F")
                lodsb  
;
; *** ОПРЕДЕЛЕНИЕ ТИПА ИНСТРУКЦИИ ***
;
                cmp     al,080h
                jc      semul_err_ret      
                cmp     al,08fh
                jna     emul_jump_far      ; One of JUMPS
;
                cmp     al,0b6h
                jnz     semul_err_ret
                lodsb
                cmp     al,00111100b
                ja      semul_err_ret      ; > movzx edi,byte ptr [...]
;

                call    two_regs
                cmp     dl,100b
                jnz     semul_err_ret      ; ???
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      semul_err_ret
;
                mov     esi,edi            ; Build " MOVZX EREG, BYTE PTR [OFFSET] "
                mov     edi,temp_instr_buf
                push    edi

                lodsw                      ; Current instruction ID 
                stosw
                lodsb
                xor     al,00000001b       ; Correct ID 
                stosb
;
                mov     eax,ebx
                stosd                      ; Store OFFSET ( =EREGA+EREGB )
;
                pop     esi
                sub     ecx,ecx
                mov     cl,7
                call    run_instr          ; Run instruction
                jmp     spec_emul_ret