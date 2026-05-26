; Анализатор кода (c) 2004, beauty on the fire
;
; Часть анализатора кода, отвечающая за поиск сигнатур известных вирусов.
;

;──────────────────────────────────────────────────────────────────────────────
; detect - сравнение сигнатур из базы "bloc" с участком кода из "cloc"
;
; on start :     ftype - тип файла
;                 bloc - расположение базы сигнатур
;                 cloc - смещение кода
; on exit  :  eax == 0 - ни одна из сигнатур не совпала
;             eax != 0 - содержит номер совпавшей сигнатуры в базе
;
 proc detect            cloc, bloc, ftype
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx esi
                sub     edx,edx                ; edx - регистр под номер записи
                inc     edx                    ; edx=1 - рассмотрим 1 запись
                mov     esi,[bloc]             ; esi - расположение базы
                mov     ecx,dword [esi]        ; ecx - кол-во записей в базе
                add     esi,4                  ; esi - данные первой записи
 det_main_loop: mov     eax,[ftype]         ;┐
                cmp     byte [esi],al       ;│сравним тип файла и записи
                jnz     det_main_next       ;┘не совпадает, значит не проверяем
                sub     eax,eax
                mov     al,byte [esi+e_type+e_name]
                push    eax
                add     esi,e_type+e_name+e_slen+e_dlen
                push    esi                    ; расположение сигнатуры в записи
                push    [cloc]                 ; расположение кода для сравнения
                call    detect_sig             ; совпадает сигнатура ?
                or      eax,eax
                jnz     det_sig_found          ; если сигнатура совпала
;───────────────
 det_main_next: inc     edx                    ; если сигнатура не совпала,
                add     esi,e_sdata+e_curem    ; проверим следующую запись
                loop    det_main_loop
                sub     eax,eax                ; если ни одна из сигнатур не
 det_main_ret:  pop     esi edx ecx            ; совпала - eax = 0
                ret
;───────────────
 det_sig_found: xchg    eax,edx            ; eax<>edx=номер совпавшей сигнатуры
                jmp     det_main_ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; проверка участка на совпадение с сигнатурой
;
; on start : slen - длина сигнатуры
;            sloc - расположение сигнатуры
;            cloc - расположение кода для сверки
; on exit  : eax = 0 - не совпадают, иначе сигнатура совпала
;
 proc detect_sig        cloc, sloc, slen
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx esi edi
                                         ; установим параметры сигнатуры:
                mov     ecx,[slen]       ; ecx - размер
                mov     esi,[sloc]       ; esi - расположение
                mov     edi,[cloc]       ; edi - код для сравнения с сигнатурой
 detect_loop:   lodsb
                cmp     al,'?'
                jnz     det_compare
;
; если встретился символ "?", значит необходимо пропустить количество
; (указанное в следующем dword'е сигнатуры) байт в исследуемом коде
;
                sub     eax,eax
                lodsb
                dec     ecx
                add     edi,eax
                jmp     detect_cont+1
;
 det_compare:   mov     ebx,edi                  ; Тестируем текущее смещение
                call    test_off                 ; (может выходить за границы памяти)
                jc      detect_err_ret           
;
                cmp     byte [edi],al            ; сравним байты сигнатуры и кода
                jz      detect_cont
                sub     eax,eax
                jmp     detect_ret
 detect_cont:   inc     edi
                loop    detect_loop              ; если цикл окончен и сигнатура
                mov     al,1                     ; совпала полностью EAX = 1
 detect_ret:    pop     edi esi edx ecx
                ret
;
 detect_err_ret:
                sub     eax,eax
                jmp     detect_ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────