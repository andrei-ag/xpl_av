;───────────────────────
; xxx byte ptr [ereg]                │ 0FEHEX, 00XXXreg
; xxx byte ptr [esp]                 │ 0FEHEX, 00XXX100,   024HEX
; xxx byte ptr [erega+eregb]         │ 0FEHEX, 00XXX100, 00rgBrgA 
; xxx byte ptr [dword]               │ 0FEHEX, 00XXX101,    dword
;
; xxx byte ptr [ereg*2]              │ 0FEHEX, 00XXX100, 01reg101, 00000000                                
; xxx byte ptr [ereg*2+dword]        │ 0FEHEX, 00XXX100, 01reg101,    dword
; xxx byte ptr [ereg*4]              │ 0FEHEX, 00XXX100, 10reg101, 00000000                             
; xxx byte ptr [ereg*4+dword]        │ 0FEHEX, 00XXX100, 10reg101,    dword
; xxx byte ptr [ereg*8]              │ 0FEHEX, 00XXX100, 11reg101, 00000000                                
; xxx byte ptr [ereg*8+dword]        │ 0FEHEX, 00XXX100, 11reg101,    dword
;
; xxx byte ptr [erega+eregb*2]       │ 0FEHEX, 00XXX100, 01rgBrgA
; xxx byte ptr [erega+eregb*4]       │ 0FEHEX, 00XXX100, 10rgBrgA
; xxx byte ptr [erega+eregb*8]       │ 0FEHEX, 00XXX100, 11rgBrgA
;                                
; xxx byte ptr [ebp]                 │ 0FEHEX, 01XXX101, 00000000 
; xxx byte ptr [esp+byte]            │ 0FEHEX, 01XXX100,   024HEX,  byte
; xxx byte ptr [ereg+byte]           │ 0FEHEX, 01XXXreg,     byte
; xxx byte ptr [erega+eregb+byte]    │ 0FEHEX, 01XXX100, 00rgBrgA,  byte 
;
; xxx byte ptr [ereg+dword]          │ 0FEHEX, 10XXXreg,    dword
; xxx byte ptr [ESP+dword]           │ 0FEHEX, 10XXX100,   024HEX, dword
; xxx byte ptr [erega+eregb+dword]   │ 0FEHEX, 10XXX100, 00rgBrgA, dword
;
; LEGEND: 000 - INC, 001 - DEC, 010 - ???, 011 - ???,
;         100 - ???, 101 - ???, 110 - ???, 111 - ??? 
; 
 emul_0FE:      
                sub     eax,eax
;
                push    ebp
                lea     ebp,[edi+ecx]
                call    emul_count_offset      ; корректируем смещение (если нужно)
                pop     ebp                    ; и разбираем структуру инструкции
                jc      semul_err_ret
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;          

; LEGEND: 000 - INC, 001 - DEC, 010 - ???, 011 - ???,
;         100 - ???, 101 - ???, 110 - ???, 111 - ??? 
;
 emul_0FE_xxx:
                push    eax
                xor     al,ah
                sub     ah,ah
                call    two_regs
                xchg    eax,edx             ; DL = Instruction TYPE 
                pop     eax                 ; (INC/DEC/CALL/JMP/PUSH ...)
                jmp     emul_0FF_inc_dec_build_instr