;────────────────────────
; * * * Имитация исполнения инструкции "cmps b/w/d"
;

;
; Для имитации выполнения CMPS B/W/D сформируем в буфере следующий код
; 
; MOV ESI, OFFSET
; MOV EDI, OFFSET
; CMPS(B/W/D)
; RET
;
; и запустим его на выполнение
;
 proc emul_cmps              
 local .temp_EREG:DWORD
;
                push    edi
                pop     esi                ; ESI указывает на начало инструкции
;
                mov     edi,temp_instr_buf
                mov     al,0BEh            ; ID = MOV ESI, dword
                stosb                      ; Store ID

                mov     ebx,dword [regs+6*4] 
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_cmps_ret
;
                mov     eax,ebx
                stosd                          ; Store DWORD
                mov     dword [.temp_EREG],eax ; Запомним состояние ESI
                                               ; до выполнения инструкции
;
                mov     al,0BFh               ; ID = MOV EDI, dword
                stosb                         ; Store ID
;
                mov     ebx,dword [regs+7*4]
		call    calc_offset           ; получим итоговое смещение
                                              ; скорректируем относительно
                                              ; буфера (с учетом секций)
                call    test_off
                jc      emul_cmps_ret
;
                mov     eax,ebx
                stosd                         ; Store DWORD
;
                rep     movsb                 ; Store CMPS BYTE/WORD/DWORD
;
                mov     al,0c3h
                stosb
;
                pushfd                     ; Сохраним значение флагов программы
                push    dword [flags]      ; Установим значение флагов эмулируемой
                popfd                      ; программы 
;
                mov     esi,temp_instr_buf
                call    esi
;
                pushfd                     ; Сохраним значение флагов
                pop     dword [flags]      ; эмулируемой программы
                popfd                      ; Восстановим состояние флагов программы
;
; Получим дельту изменения регистра ESI и изменим на
; нее состояние виртуального регистра ESI и EDI
;
                sub     esi,dword [.temp_EREG]
                add     dword [regs+6*4],esi
                add     dword [regs+7*4],esi
;
 emul_cmps_ret: ret
;
 endp