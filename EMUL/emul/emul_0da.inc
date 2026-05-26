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
; LEGEND: 000 - FIADD, 001 - FIMUL,  010 - FICOM, 011 - FICOMP
;         100 - FISUB, 101 - FISUBR, 110 - FIDIV, 111 - FIDIVR
;
 emul_0DA:      
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
                pop     eax                 ; (FIADD/FIMUL/FICOM ...)
;═══════════════
; XXX DWORD PTR [ ... ]
;
                jmp     emul_0FF_inc_dec_build_instr         
