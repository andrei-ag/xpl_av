;────────────────────────
; * * * Имитация исполнения инструкции "scas b/w/d"
;
;
 proc emul_scas
 local .temp_EDI:DWORD              
;
                mov     esi,edi
                mov     ebx,dword [regs+7*4]
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_scas_ret
;
; Сформируем в буфере следующий код
; 
; MOV EDI, РАСЧИТАННОЕ ВЫШЕ СМЕЩЕНИЕ
; SCAS(B/W/D)
; RET
;
; и запустим его на выполнение
;
                mov     edi,temp_instr_buf
                push    edi
                mov     al,0BFh            ; ID = Инструкции (MOV EDI, ...)
                stosb                      
                mov     eax,ebx
                stosd                      ; СМЕЩЕНИЕ
                mov     dword [.temp_EDI],eax ; Запомним состояние EDI
                                              ; до выполнения инструкции
;
    ;            push    ecx esi
                rep     movsb              ; перенесем инструкцию (scas...)
    ;            pop     esi ecx
; 
                mov     al,0c3h
                stosb                      ; RET
                pop     edi
;
                pushfd                     ; Сохраним значение флагов программы
                push    dword [flags]      ; Установим значение флагов
                popfd                      ; эмулируемой программы 
                mov     eax,dword [regs+0*4]
                call    edi                ; выполним инструкции
                pushfd                     ; Сохраним значение флагов
                pop     dword [flags]      ; эмулируемой программы
                popfd                      ; Восстановим состояние флагов программы
;
; Получим дельту изменения регистра EDI и изменим на
; нее состояние виртуального регистра EDI
;
                sub     edi,dword [.temp_EDI]
                add     dword [regs+7*4],edi
 emul_scas_ret: ret
;
 endp