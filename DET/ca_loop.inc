; Анализатор кода (c) 2004, beauty on the fire
;
; Часть анализатора кода, отвечающая за ведение базы данных циклов
;
; Формат базы данных:
;
; 1) dword - инструкция следующая за инструкцией перехода (откуда перешли)
; 2) dword - адрес перехода (куда перешли)
; 3) dword - количество совершенных переходов (сколько раз перешли)
;
;  Как только эмулятор натыкается на любую инструкцию, которая перемещает EIP
; он ищет в базе данных запись со аналогичным "адресом перехода" (1) и
; "смещением следующем за инструкцией перехода" (3), если такая есть в
; базе данных, он увеличивает "количество совершенных переходов" (2)
; иначе в базу заносится новая запись
;
; Описание "emul_stop" в "data\ca_emul.inc"
;
 this_is_loop          = 3           ; количество переходов после которого
                                     ; анализатор считает, что участок это цикл
;──────────────────────────────────────────────────────────────────────────────
; store_loop - заносим запись в базу данных
;
; on start :    esi - текущее положение EIP эмулятора
;               ecx - размер инструкции
; on exit  :
;
 proc ca_store_loop        
;──────────────────────────────────────────────────────────────────────────────

; *** Отключено (не дает значительного ускорения)
;
;                mov     eax,dword [ca_current_loop_ptr]
;                or      eax,eax
;                jnz     ca_store_loop_ret        ; -> Цикл уже исполняется
;
                mov     eax,dword [emul_stop+4]
                or      eax,eax
                jz      ca_store_loop_ret        ; -> Первый запуск
;
                add     eax,dword [emul_stop+8]  ; Смещение за проэмулированной
                                                 ; инструкцией
                cmp     esi,eax
                jz      ca_store_loop_ret        ; -> Равно текущему смещению
; 
; Если смещения не равны - осуществлен переход
;
                push    eax
                push    esi
                call    ca_process_loop_base     ; Проверим базу
;
 ca_store_loop_ret:
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc ca_process_loop_base dest_offset, last_offset
;
     		push    ecx esi edi
                mov     esi,dword [ca_base_pointer]
                mov     edi,esi
                lodsd
                or      eax,eax
                jz      ca_process_store_entry
;
                xchg    eax,ecx
 ca_process_loop:
                mov     eax,dword [esi+4]        ; адрес перехода в базе (2)
                cmp     eax,dword [dest_offset]  ; сравним
                jnz     ca_process_loop_next
;
                mov     eax,dword [esi]          ; адрес за инструкцией перехода в базе
                cmp     eax,dword [last_offset]  ; сравним (1)
                jnz     ca_process_loop_next
;
                inc     dword [esi+8]            ; переход повторился (3)
                cmp     dword [esi+8],this_is_loop
                jl      ca_process_loop_base_ret
                call    ca_set_current_loop
                jmp     ca_process_loop_base_ret
;
 ca_process_loop_next:
                add     esi,4*3
                loop    ca_process_loop
;
; Если в базе данных нет записей по этому переходу (или нет вообще), добавим
;
 ca_process_store_cmp:
                mov     edi,dword [ca_base_pointer]
                mov     eax,dword [edi]
                cmp     eax,(virtual_stack_size-2000)/(4*3)
                jge     ca_process_loop_base_ret      ; База данных переполнена
;
; Добавим новую запись
;
 ca_process_store_entry:
 		inc     dword [edi]
                xchg    esi,edi
                mov     eax,dword [last_offset]
                stosd
                mov     eax,dword [dest_offset]
                stosd
                sub     eax,eax
                stosd
 ca_process_loop_base_ret:                          
                pop     edi esi ecx
                ret 
 endp
;──────────────────────────────────────────────────────────────────────────────
; установим нижнюю границы текущего цикла
;
; esi = запись о цикле в базе данных кодо-анализатора
;
 proc ca_set_current_loop
;
                push    eax ecx edx ebx
 ca_set_current_loop_part_one:
                mov     eax,dword [esi]
                mov     ecx,dword [esi+4]
                cmp     eax,ecx                       
                jb      ca_set_current_loop_set      ; -> eax < ecx
                xchg    eax,ecx                      ; switch 'em
;
 ca_set_current_loop_set:
                mov     dword [ca_current_loop_ptr],eax
                mov     dword [ca_current_loop_ptr+4],ecx
 ca_set_current_loop_ret:
                pop     ebx edx ecx eax
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; Находится ли смещение ESI в текущем цикле
;
; on start:  esi = СМЕЩЕНИЕ
; on exit : CF=1 = не находится в цикле
;
 proc ca_in_loop
                push    eax
                mov     eax,dword [ca_current_loop_ptr]
                or      eax,eax
                jz      ca_in_loop_false        ; Цикл не установлен
                cmp     esi,eax
                jbe     ca_in_loop_false        ; ESI <= нижней границы цикла
;
                mov     eax,dword [ca_current_loop_ptr+4]
                or      eax,eax
                jz      ca_in_loop_false        ; Цикл не установлен
                cmp     esi,eax
                jae     ca_in_loop_false        ; ESI => нижней границы цикла
;
 ca_in_loop_true:                               ; ESI в цикле
                pop     eax                     
                clc
                ret
;
 ca_in_loop_false:                              ; ESI не в цикле
                mov     dword [ca_current_loop_ptr],0 ; <- сбросим цикл
                pop     eax                     
                stc
                ret
 endp
