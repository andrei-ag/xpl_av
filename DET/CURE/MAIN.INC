; главный модуль для лечения вирусов (c) 2004, beauty on the fire
;
;──────────────────────────────────────────────────────────────────────────────

;──────────────────────────────────────────────────────────────────────────────
; cure_vir - лечение обнаруженных вирусов
;
; on start :  entry_no - номер записи
; on exit  :
;
 proc cure_vir          entry_no
;──────────────────────────────────────────────────────────────────────────────
                cmp     byte [try_cure],0        ; если в параметрах коммандной
                jnz     cure_vir_init            ; строки небыло указаний по
                ret                              ; лечению файлов ("/c") выйдем
;───────────────
 cure_vir_init: pushad
                push    msg_next
                call    write_string
                mov     eax,dword [file_map_address]
                add     eax,dword [ph_loc]       ; EAX = Start of PE header
                cmp     word [eax+006h],1        ; Количество секций
                jnz     cure_fnd_entry           ; в файле 1 секция ?
;───────────────
;  Если в файле одна секция, значит перед нами червь, троянец или дроппер
; файлового вируса. Но все этих типы "зараженных" файлов "неизлечимы".
; Выведем информацию о том, что файлы лучше удалить и выйдем.
;
 vir_incurable: push    msg_incurable
                call    write_string
                inc     byte [cure_result]
                jmp     cure_vir_ret
 vir_disinf_er: push    msg_disinf_er
                call    write_string
                jmp     cure_vir_ret
;───────────────
;  Производим поиск записи, которая содержит информацию о вирусе обнаруженном
; в файле.
;
 cure_fnd_entry:mov     esi,@001_type
                mov     eax,[entry_no]
                mov     edx,entry_size
                mul     edx
                add     esi,eax
;
; Проверим метод лечения указанный в записи:
;
;  * "cmethod_del" - вирус неизлечим (зараженные файл необходимо удалить)
;  * "cmethod_unk" - процедура лечения еще не реализованна
;
; Во всех остальных случаях будет указываться смещение процедуры (расположенной
; в антивирусе) предназначенной для лечения записи.
;
                mov     eax,dword [esi+entry_size-e_curem]
                cmp     eax,cmethod_del
                jz      vir_incurable
                cmp     eax,cmethod_unk
                jz      vir_disinf_er
;
; Запустим процедуру, смещение которой указанно в записи.
; Возвращаемые значения: edx == 0 - лечение прошло успешно
;                        edx != 0 - произошла ошибка, файл не вылечен
;
                call    eax
                or      edx,edx
                jnz     vir_disinf_er
                push    msg_cured
                call    write_string
                inc     dword [cleaned]
                mov     byte [cure_result],2
;───────────────
 cure_vir_ret:  popad
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────