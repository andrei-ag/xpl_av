;────────────────────────
; * * * Имитация исполнения инструкции "lods b/w/d"
;

;
 proc emul_lods      
 local .temp_ESI:DWORD        
;
                mov     esi,edi
		mov     ebx,dword [regs+6*4]
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_lods_ret
;
; Сформируем в буфере следующий код
; 
; MOV ESI, РАСЧИТАННОЕ ВЫШЕ СМЕЩЕНИЕ
; LODS(B/W/D)
; RET
;
; и запустим его на выполнение
;
                mov     edi,temp_instr_buf
                push    edi
                mov     al,0BEh            ; ID = Инструкции (MOV ESI, ...)
                stosb                      
                mov     eax,ebx
                stosd                      ; СМЕЩЕНИЕ
;
                mov     dword [.temp_ESI],eax ; Запомним состояние ESI
                                              ; до выполнения инструкции
;
 ;               push    esi ecx
                rep     movsb              ; перенесем инструкцию (lods...)
 ;               pop     ecx esi
;
                mov     al,0c3h
                stosb                      ; RET
                pop     edi
;
                pushfd                     ; Сохраним значение флагов программы
                push    dword [flags]      ; Установим значение флагов эмулируемой
                popfd                      ; программы 
                mov     eax,dword [regs+0*4]
                call    edi                ; выполним инструкции
                popfd                      ; Восстановим состояние флагов программы
;
; Получим дельту изменения регистра ESI и изменим на
; нее состояние виртуального регистра ESI
;
                sub     esi,dword [.temp_ESI]
                add     dword [regs+6*4],esi
;
                mov     dword [regs+0*4],eax
 emul_lods_ret: ret
;
 endp