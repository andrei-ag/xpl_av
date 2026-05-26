;══════════════════════
; lea eregb,dword ptr [erega]            │ 08DHEX, 00rgBrgA
; lea ereg,dword ptr [esp]               │ 08DHEX, 00reg100,   024HEX
; lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
; lea ereg,dword ptr [dword]             │ 08DHEX, 00reg101,    dword
;
; lea ereg,dword ptr [ereg*2]            │ 08DHEX, 00reg100, 01reg101, 00000000
; lea ereg,dword ptr [ereg*2+dword]      │ 08DHEX, 00reg100, 01reg101,    dword
; lea ereg,dword ptr [ereg*4]            │ 08DHEX, 00reg100, 10reg101, 00000000
; lea ereg,dword ptr [ereg*4+dword]      │ 08DHEX, 00reg100, 10reg101,    dword
; lea ereg,dword ptr [ereg*8]            │ 08DHEX, 00reg100, 11reg101, 00000000
; lea ereg,dword ptr [ereg*8+dword]      │ 08DHEX, 00reg100, 11reg101,    dword
;
; lea ereg,dword ptr [erega+eregb*2]     │ 08DHEX, 00reg100, 01rgBrgA
; lea ereg,dword ptr [erega+eregb*4]     │ 08DHEX, 00reg100, 10rgBrgA
; lea ereg,dword ptr [erega+eregb*8]     │ 08DHEX, 00reg100, 11rgBrgA
;
; lea ereg,dword ptr [ebp]               │ 08DHEX, 01reg101, 00000000 
; lea ereg,dword ptr [esp+byte]          │ 08DHEX, 01reg100,   024HEX, byte
; lea eregb,dword ptr [erega+byte]       │ 08DHEX, 01rgBrgA,     byte
; lea ereg,dword ptr [erega+eregb+byte]  │ 08DHEX, 01reg100, 00rgBrgA, byte 
;
; lea eregb,dword ptr [erega+dword]      │ 08DHEX, 10rgBrgA,    dword
; lea ereg,dword ptr [ESP+dword]         │ 08DHEX, 10reg100,   024HEX, dword
; lea ereg,dword ptr [erega+eregb+dword] │ 08DHEX, 10reg100, 00rgBrgA, dword 
;
; По размеру и строению аналогичны "089H"
;
 emul_08D:      
                sub     eax,eax
;
                push    ebp
                lea     ebp,[edi+ecx]
                call    emul_count_offset      ; корректируем смещение (если нужно)
                pop     ebp                    ; и разбираем структуру инструкции
                jc      semul_err_ret
;
                xor     al,ah
                sub     ah,ah
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;
                push    eax                ; EAX = ID инструкции
                call    two_regs
                pop     eax 
                xor     al,dl              ; уберем из ereg
;
                mov     ah,al              ; Данные по регистру
                mov     al,byte [edi]      ; ID инструкции
                xor     ah,101b            ; закончим формирование данных по регистру
;
; Формирование инструкции " LEA EREG, [СМЕЩЕНИЕ] "
; и запуск в "специальных условиях" 
;
                mov     edi,temp_instr_buf
                mov     esi,edi              
                stosw                      ; Перенесем ID инструкции
                mov     eax,ebx            ; EAX = СМЕЩЕНИЕ            
                stosd
;
                sub     ecx,ecx
                mov     cl,6               ; размер инструкции
                call    run_instr          ; выполним инструкцию
;                             
                jmp     spec_emul_ret