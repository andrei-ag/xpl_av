;───────────────────────
; xxx byte ptr [ereg]                │ 0F6HEX, 00XXXreg
; xxx byte ptr [esp]                 │ 0F6HEX, 00XXX100,   024HEX
; xxx byte ptr [erega+eregb]         │ 0F6HEX, 00XXX100, 00rgBrgA 
; xxx byte ptr [dword]               │ 0F6HEX, 00XXX101,    dword
;
; xxx byte ptr [ereg*2]              │ 0F6HEX, 00XXX100, 01reg101, 00000000                                
; xxx byte ptr [ereg*2+dword]        │ 0F6HEX, 00XXX100, 01reg101,    dword
; xxx byte ptr [ereg*4]              │ 0F6HEX, 00XXX100, 10reg101, 00000000                             
; xxx byte ptr [ereg*4+dword]        │ 0F6HEX, 00XXX100, 10reg101,    dword
; xxx byte ptr [ereg*8]              │ 0F6HEX, 00XXX100, 11reg101, 00000000                                
; xxx byte ptr [ereg*8+dword]        │ 0F6HEX, 00XXX100, 11reg101,    dword
;
; xxx byte ptr [erega+eregb*2]       │ 0F6HEX, 00XXX100, 01rgBrgA
; xxx byte ptr [erega+eregb*4]       │ 0F6HEX, 00XXX100, 10rgBrgA
; xxx byte ptr [erega+eregb*8]       │ 0F6HEX, 00XXX100, 11rgBrgA
;                                
; xxx byte ptr [ebp]                 │ 0F6HEX, 01XXX101, 00000000 
; xxx byte ptr [esp+byte]            │ 0F6HEX, 01XXX100,   024HEX,  byte
; xxx byte ptr [ereg+byte]           │ 0F6HEX, 01XXXreg,     byte
; xxx byte ptr [erega+eregb+byte]    │ 0F6HEX, 01XXX100, 00rgBrgA,  byte 
;
; xxx byte ptr [ereg+dword]          │ 0F6HEX, 10XXXreg,    dword
; xxx byte ptr [ESP+dword]           │ 0F6HEX, 10XXX100,   024HEX, dword
; xxx byte ptr [erega+eregb+dword]   │ 0F6HEX, 10XXX100, 00rgBrgA, dword
;
; LEGEND: 000 - TEST, 001 - ???,  010 - NOT, 011 - NEG,
;         100 - MUL,  101 - IMUL, 110 - DIV, 111 - IDIV 
;
; Инструкции TEST, DIV, IDIV пока не эмулируются
; 
 emul_0F6:      
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

;
; LEGEND: 000 - TEST, 001 - ???,  010 - NOT, 011 - NEG,
;         100 - MUL,  101 - IMUL, 110 - DIV, 111 - IDIV 
;
                push    eax
                xor     al,ah
                sub     ah,ah
                call    two_regs
                xchg    eax,edx             ; DL = Instruction TYPE 
                pop     eax                 ; (INC/DEC/CALL/JMP/PUSH ...)
                jmp     emul_0FF_inc_dec_build_instr