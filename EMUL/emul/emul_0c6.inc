;───────────────────────
; mov byte ptr [ereg],byte                │ 0C6HEX, 00000reg,     byte
; mov byte ptr [esp],byte                 │ 0C6HEX, 00000100,   024HEX,  byte
; mov byte ptr [erega+eregb],byte         │ 0C6HEX, 00000100, 00rgBrgA,  byte 
; mov byte ptr [dword],byte               │ 0C6HEX, 00000101,    dword,  byte
;
; mov byte ptr [ereg*2],byte              │ 0C6HEX, 00000100, 01reg101, 00000000,  byte
; mov byte ptr [ereg*2+dword],byte        │ 0C6HEX, 00000100, 01reg101,    dword,  byte
; mov byte ptr [ereg*4],byte              │ 0C6HEX, 00000100, 10reg101, 00000000,  byte                             
; mov byte ptr [ereg*4+dword],byte        │ 0C6HEX, 00000100, 10reg101,    dword,  byte
; mov byte ptr [ereg*8],byte              │ 0C6HEX, 00000100, 11reg101, 00000000,  byte                                
; mov byte ptr [ereg*8+dword],byte        │ 0C6HEX, 00000100, 11reg101,    dword,  byte
;
; mov byte ptr [erega+eregb*2],byte       │ 0C6HEX, 00000100, 01rgBrgA,  byte
; mov byte ptr [erega+eregb*4],byte       │ 0C6HEX, 00000100, 10rgBrgA,  byte
; mov byte ptr [erega+eregb*8],byte       │ 0C6HEX, 00000100, 11rgBrgA,  byte
;                                             
; mov byte ptr [ebp],byte                 │ 0C6HEX, 01000101, 00000000,  byte 
; mov byte ptr [esp+byte],byte            │ 0C6HEX, 01000100,   024HEX,  byte,  byte
; mov byte ptr [ereg+byte],byte           │ 0C6HEX, 01000reg,     byte,  byte
; mov byte ptr [erega+eregb+byte],byte    │ 0C6HEX, 01000100, 00rgBrgA,  byte,  byte
; 
; mov byte ptr [ereg+dword],byte          │ 0C6HEX, 10000reg,    dword,  byte
; mov byte ptr [ESP+dword],byte           │ 0C6HEX, 10000100,   024HEX, dword,  byte
; mov byte ptr [erega+eregb+dword],byte   │ 0C6HEX, 10000100, 00rgBrgA, dword,  byte
;                      
 emul_0C6:      jmp    emul_080