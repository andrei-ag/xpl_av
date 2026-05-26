;══════════════════════
; 1a) shr dword ptr [EREG],reg              │ 0D3HEX, 00regERG
; 1b) shr dword ptr [esp],reg               │ 0D3HEX, 00reg100,   024HEX
; 1c) shr dword ptr [erega+eregb],reg       │ 0D3HEX, 00reg100, 00rgBrgA 
; 1d) shr dword ptr [dword],reg             │ 0D3HEX, 00reg101,    dword
;
;     shr dword ptr [ereg*2],reg            │ 0D3HEX, 00reg100, 01reg101, 00000000
;     shr dword ptr [ereg*2+dword],reg      │ 0D3HEX, 00reg100, 01reg101,    dword
;     shr dword ptr [ereg*4],reg            │ 0D3HEX, 00reg100, 10reg101, 00000000
;     shr dword ptr [ereg*4+dword],reg      │ 0D3HEX, 00reg100, 10reg101,    dword
;     shr dword ptr [ereg*8],reg            │ 0D3HEX, 00reg100, 11reg101, 00000000
;     shr dword ptr [ereg*8+dword],reg      │ 0D3HEX, 00reg100, 11reg101,    dword
;
;     shr dword ptr [erega+eregb*2],reg     │ 0D3HEX, 00reg100, 01rgBrgA
;     shr dword ptr [erega+eregb*4],reg     │ 0D3HEX, 00reg100, 10rgBrgA
;     shr dword ptr [erega+eregb*8],reg     │ 0D3HEX, 00reg100, 11rgBrgA
;
; 2a) shr dword ptr [ebp],reg               │ 0D3HEX, 01reg101, 00000000 
; 2b) shr dword ptr [esp+byte],reg          │ 0D3HEX, 01reg100,   024HEX, byte
; 2c) shr dword ptr [EREG+byte],reg         │ 0D3HEX, 01regERG,     byte
; 2d) shr dword ptr [erega+eregb+byte],reg  │ 0D3HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) shr dword ptr [EREG+dword],reg        │ 0D3HEX, 10regERG,    dword
; 3b) shr dword ptr [ESP+dword],reg         │ 0D3HEX, 10reg100,   024HEX, dword
; 3c) shr dword ptr [erega+eregb+dword],reg │ 0D3HEX, 10reg100, 00rgBrgA, dword 
;
;  4) rol reg,CL                            │ 0D3HEX, 11000reg
;     ror reg,CL                            │ 0D3HEX, 11001reg
;     rcl reg,CL                            │ 0D3HEX, 11010reg
;     rcr reg,CL                            │ 0D3HEX, 11011reg
;     shl reg,CL                            │ 0D3HEX, 11100reg
;     shr reg,CL                            │ 0D3HEX, 11101reg
;     sal reg,CL                            │ 0D3HEX, 11110reg
;     sar reg,CL                            │ 0D3HEX, 11111reg
;
; !! По размеру и строению аналогичны "089H" !!
;
 @0D3_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr

                jmp    _@two_bytes
                ;jmp    @089_disasm 
                