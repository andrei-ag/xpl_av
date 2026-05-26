;───────────────────────
; xxx byte ptr [ereg],byte              │ 080HEX, 00XXXreg,     byte
; xxx byte ptr [esp],byte               │ 080HEX, 00XXX100,   024HEX,  byte
; xxx byte ptr [erega+eregb],byte       │ 080HEX, 00XXX100, 00rgBrgA,  byte 
; xxx byte ptr [dword],byte             │ 080HEX, 00XXX101,    dword,  byte
;
; xxx byte ptr [ereg*2],byte            │ 080HEX, 00XXX100, 01reg101, 00000000,  byte
; xxx byte ptr [ereg*2+dword],byte      │ 080HEX, 00XXX100, 01reg101,    dword,  byte
; xxx byte ptr [ereg*4],byte            │ 080HEX, 00XXX100, 10reg101, 00000000,  byte                             
; xxx byte ptr [ereg*4+dword],byte      │ 080HEX, 00XXX100, 10reg101,    dword,  byte
; xxx byte ptr [ereg*8],byte            │ 080HEX, 00XXX100, 11reg101, 00000000,  byte                                
; xxx byte ptr [ereg*8+dword],byte      │ 080HEX, 00XXX100, 11reg101,    dword,  byte
;
; xxx byte ptr [erega+eregb*2],byte     │ 080HEX, 00XXX100, 01rgBrgA,  byte
; xxx byte ptr [erega+eregb*4],byte     │ 080HEX, 00XXX100, 10rgBrgA,  byte
; xxx byte ptr [erega+eregb*8],byte     │ 080HEX, 00XXX100, 11rgBrgA,  byte
;                                             
; xxx byte ptr [ebp],byte               │ 080HEX, 01XXX101, 00000000,  byte 
; xxx byte ptr [esp+byte],byte          │ 080HEX, 01XXX100,   024HEX,  byte
; xxx byte ptr [ereg+byte],byte         │ 080HEX, 01XXXreg,     byte,  byte
; xxx byte ptr [erega+eregb+byte],byte  │ 080HEX, 01XXX100, 00rgBrgA,  byte, byte 
;
; xxx byte ptr [ereg+dword],byte        │ 080HEX, 10XXXreg,    dword   byte,
; xxx byte ptr [ESP+dword],byte         │ 080HEX, 10XXX100,   024HEX, dword, byte
; xxx byte ptr [erega+eregb+dword],byte │ 080HEX, 10XXX100, 00rgBrgA, dword, byte
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
 emul_080:    
;
                push    ebp
                lea     ebp,[edi+ecx-1]
                call    emul_count_offset      ; корректируем смещение (если нужно)
                pop     ebp                    ; и разбираем структуру инструкции
                jc      semul_err_ret
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      semul_err_ret
;
; Create instruction " XXX BYTE PTR [OFFSET],KEY " and run it
;
                xor     al,ah
                sub     ah,ah
                push    eax
                call    two_regs
                pop     eax
                xor     al,dl
                xor     al,00000101b
                mov     ah,al
                mov     al,byte [edi]
;
                sub     edx,edx            
                mov     dl,byte [edi+ecx-1]; KEY (byte) 
;
                mov     edi,temp_instr_buf
                mov     esi,edi                
                stosw                      ; Store instruction ID
                mov     eax,ebx                        
                stosd                      ; Store OFFSET 
                mov     eax,edx
                stosb                      ; Store key (byte)
;
                sub     ecx,ecx
                mov     cl,2+4+1           ; размер инструкции
                call    run_instr          ; выполним инструкцию
                jmp     spec_emul_ret