;───────────────────────
; pop dword ptr [ereg]                │ 08FHEX, 00000reg
; pop dword ptr [esp]                 │ 08FHEX, 00000100,   024HEX
; pop dword ptr [erega+eregb]         │ 08FHEX, 00000100, 00rgBrgA 
; pop dword ptr [dword]               │ 08FHEX, 00000101,    dword
;
; pop dword ptr [ereg*2]              │ 08FHEX, 00000100, 01reg101, 00000000                                
; pop dword ptr [ereg*2+dword]        │ 08FHEX, 00000100, 01reg101,    dword
; pop dword ptr [ereg*4]              │ 08FHEX, 00000100, 10reg101, 00000000                             
; pop dword ptr [ereg*4+dword]        │ 08FHEX, 00000100, 10reg101,    dword
; pop dword ptr [ereg*8]              │ 08FHEX, 00000100, 11reg101, 00000000                                
; pop dword ptr [ereg*8+dword]        │ 08FHEX, 00000100, 11reg101,    dword
;
; pop dword ptr [erega+eregb*2]       │ 08FHEX, 00000100, 01rgBrgA
; pop dword ptr [erega+eregb*4]       │ 08FHEX, 00000100, 10rgBrgA
; pop dword ptr [erega+eregb*8]       │ 08FHEX, 00000100, 11rgBrgA
;                                
; pop dword ptr [ebp]                 │ 08FHEX, 01000101, 00000000 
; pop dword ptr [esp+byte]            │ 08FHEX, 01000100,   024HEX,  byte
; pop dword ptr [ereg+byte]           │ 08FHEX, 01000reg,     byte
; pop dword ptr [erega+eregb+byte]    │ 08FHEX, 01000100, 00rgBrgA,  byte 
;
; pop dword ptr [ereg+dword]          │ 08FHEX, 10000reg,    dword
; pop dword ptr [ESP+dword]           │ 08FHEX, 10000100,   024HEX, dword
; pop dword ptr [erega+eregb+dword]   │ 08FHEX, 10000100, 00rgBrgA, dword
;
 emul_08F:
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

;═══════════════
; POP DWORD PTR [ ... ]
;
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      semul_err_ret
;
                call    get_from_vstack    ; Читаем DWORD из виртуального стека в EAX
                jc      semul_err_ret
;
; Команда POP не влияет на флаги, так что имитация выполняется
; с помощью выполнения одной инструкции
;
                mov     dword [ebx],eax
                jmp     spec_emul_ret           