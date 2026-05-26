;───────────────────────
; XXX dword ptr [ereg]                │ 0DBHEX, 00XXXreg
; XXX dword ptr [esp]                 │ 0DBHEX, 00XXX100,   024HEX
; XXX dword ptr [erega+eregb]         │ 0DBHEX, 00XXX100, 00rgBrgA 
; XXX dword ptr [dword]               │ 0DBHEX, 00XXX101,    dword
;
; XXX dword ptr [ereg*2]              │ 0DBHEX, 00XXX100, 01reg101, 00000000                                
; XXX dword ptr [ereg*2+dword]        │ 0DBHEX, 00XXX100, 01reg101,    dword
; XXX dword ptr [ereg*4]              │ 0DBHEX, 00XXX100, 10reg101, 00000000                             
; XXX dword ptr [ereg*4+dword]        │ 0DBHEX, 00XXX100, 10reg101,    dword
; XXX dword ptr [ereg*8]              │ 0DBHEX, 00XXX100, 11reg101, 00000000                                
; XXX dword ptr [ereg*8+dword]        │ 0DBHEX, 00XXX100, 11reg101,    dword
;
; XXX dword ptr [erega+eregb*2]       │ 0DBHEX, 00XXX100, 01rgBrgA
; XXX dword ptr [erega+eregb*4]       │ 0DBHEX, 00XXX100, 10rgBrgA
; XXX dword ptr [erega+eregb*8]       │ 0DBHEX, 00XXX100, 11rgBrgA
;                                
; XXX dword ptr [ebp]                 │ 0DBHEX, 01XXX101, 00000000 
; XXX dword ptr [esp+byte]            │ 0DBHEX, 01XXX100,   024HEX,  byte
; XXX dword ptr [ereg+byte]           │ 0DBHEX, 01XXXreg,     byte
; XXX dword ptr [erega+eregb+byte]    │ 0DBHEX, 01XXX100, 00rgBrgA,  byte 
;
; XXX dword ptr [ereg+dword]          │ 0DBHEX, 10XXXreg,    dword
; XXX dword ptr [ESP+dword]           │ 0DBHEX, 10XXX100,   024HEX, dword
; XXX dword ptr [erega+eregb+dword]   │ 0DBHEX, 10XXX100, 00rgBrgA, dword
;
; LEGEND: 000 - FILD, 001 - ???, 010 - FIST, 011 - FISTP
;         100 - ???,  101 - FLD, 110 - ???,  111 - FSTP
;
 emul_0DB:      
                sub     eax,eax
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
                pop     eax                 ; (FILD/FIST/FISTP ...)
;
                cmp     dl,001b
                jz      semul_err_ret
                cmp     dl,100b
                jz      semul_err_ret
                cmp     dl,110b
                jz      semul_err_ret
;
;═══════════════
; XXX DWORD PTR [ ... ]
;
                jmp     emul_0FF_inc_dec_build_instr