;──────────────────────────────────────────────────────────────────────────────
;
; Набор вспомогательных функций, использующихся эмулятором кода
;
;  (c) 2004 Most Needful Things 
;

;──────────────────────────────────────────────────────────────────────────────
 proc set_off_first_two_bits
;
                sub     ah,ah
                cmp     al,00111111b
                jbe     set_off_first_two_bits_ret
;
                mov     ah,01000000b
                cmp     al,01111111b
                jbe     set_off_first_two_bits_ret
;
                mov     ah,10000000b
                cmp     al,10111111b
                jbe     set_off_first_two_bits_ret
;
                mov     ah,11000000b
;
 set_off_first_two_bits_ret:
                xor     al,ah
                sub     ah,ah
                ret 
;
 endp 
;──────────────────────────────────────────────────────────────────────────────
; EBX +/- BYTE (EAX)
;
 proc add_sub_byte_ebx       
;──────────────────────────────────────────────────────────────────────────────
                push    edx
;
                cmp     al,127
                jbe     add_byte_ebx  ; <= 127
;
 sub_byte_ebx:  sub     edx,edx
                sub     dl,al
                sub     ebx,edx
;
                pop     edx
                ret
;
 add_byte_ebx:  add     ebx,eax
                pop     edx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; orthodox_offset - смещение внутри программы ("orthodox") или
;                   уже с учетом delta value (указывает на загруженный утилитой
;                   в память исполняемый файл)
;
; on start : EBX - смещение для проверки
;
; on exit  : смещение не нужно корректировать : CF  = 0 
;            смещение нужно корректировать    : CF  = 1  
;
 proc orthodox_offset
;──────────────────────────────────────────────────────────────────────────────
                push    eax
;
; Смещение указывает в область загружаемого файла? 
;
                mov     eax,dword [header_temp_buffer+034h] ; EAX = Image base
                cmp     ebx,eax
                jb      not_orthodox_offset   
                add     eax,dword [header_temp_buffer+050h] ; + Virtual size
                cmp     ebx,eax
                jbe     its_orthodox
;
 not_orthodox_offset:
                pop     eax
                clc
                ret
;
 its_orthodox:  pop     eax
                stc
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; calc_offset - корректируем смещение (находится в EBX) относительно 
;               участка памяти в который был загружен PE-файл
;
;               результат расчетов в EBX
;
 proc calc_offset            
;──────────────────────────────────────────────────────────────────────────────
                push    eax
;
                call    orthodox_offset
                jc      calc_orthodox_offset
; 
;  Проверим индикатор сегмента "FS:", на случай, если происходит обращение к
; указателю на обработчик SEH или PEB.
;                                          
                cmp     byte [segments+4],1
                jnz     calc_offset_ret
                cmp     ebx,030h           
                ja      calc_offset_ret    ; > FS:[030h]
;
                add     ebx,dword [virtual_stack_pointer]
                                           ; EBX = Указывает на начало вирт. стека
;
 calc_offset_ret:
                pop     eax
                ret
;
 calc_orthodox_offset:
                push    ebx                ; получаем смещение внутри
                call    calc_virtual_offset; антивирусного буфера
                xchg    ebx,eax
                pop     eax
                ret
;
 endp            
;──────────────────────────────────────────────────────────────────────────────
; Прочитаем двойное слово из "виртуального" стека в регистр eax
;
 proc get_from_vstack        
;──────────────────────────────────────────────────────────────────────────────
                push    ebx
                mov     ebx,dword [regs+016]
                call    test_off
                jc      get_from_vstack_err_ret
;
                add     ebx,4
                call    test_off
                jc      get_from_vstack_err_ret
;
                pop     ebx
                push    esi
                mov     esi,dword [regs+016]
                lodsd
                add     dword [regs+016],4
;
                pop     esi
                clc
                ret
;
 get_from_vstack_err_ret:
                pop     ebx
                mov     eax,-3
                stc
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; Положим двойное слово из регистра eax в "виртуальный" стек
;
 proc put_to_vstack          
;──────────────────────────────────────────────────────────────────────────────
                push    ebx
                mov     ebx,dword [regs+016]
                call    test_off
                jc      put_to_vstack_err_ret
;
                sub     ebx,4
                call    test_off
                jc      put_to_vstack_err_ret
;
                pop     ebx
                push    edi
;
                sub     dword [regs+016],4
                mov     edi,dword [regs+016]
                stosd
;
                pop     edi
                clc
                ret
;
 put_to_vstack_err_ret:
                pop     ebx
                mov     eax,-3
                stc
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; reg8_pos - определяет смещение 8-ми битного регистра (номер которого указан
;            в регистре al) относительно таблицы "regs"
;
; on start : eax - номер 8-ми битного регистра
; on exit  : eax - смещение, на значение о регистре относительно таблицы "regs"
;
 proc reg8_pos               
;──────────────────────────────────────────────────────────────────────────────
                push    ecx
                push    edx
;
                sub     ecx,ecx
                call    two_regs
                cmp     al,4
                jc      reg8_low
                sub     al,4
                inc     ecx
 reg8_low:      call    mul_4
                add     eax,ecx
;
                pop     edx
                pop     ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; test_off - проверка "допустимости" смещения указанного в регистре EBX
;
;  Имитируя исполнение инструкций, которые обращаются по смещениям, есть очень
; большой риск ошибки в расчете "нового" смещения.
; Это может быть следствием:
;
; - ошибки или недоработки анализатора кода, который пропустил процедуру
;   подсчета delta value или любую другую важную особенность кода.
; - неправильной эмуляции участка кода или инструкции.
; - в результате непредвиденной (разработчиками) ошибки в программе.
;
;  В случае если одно или несколько из этих утверждений верно, будет изменен
; код одной из частей антивируса или важные данные, которые используются
; программой для различных целей и "работать" с ними может только антивирус, а
; не никак не исследуемая программа. Результатом может стать зависание
; антивируса.
;
;  Исследуемая программа может пользоваться данными только из участка памяти
; в который она была загружена антивирусом, а так же виртуальным стеком,
; который расположен в этом же участке памяти, сразу за загруженной программой.
;
; Структура загруженного в память исполняемого файла описана в:
;                                                         "FILE\PE\LOAD_PE.INC" 
;
; on start : EBX - смещение для проверки
;
; on exit  : нормальное смещение  : CF  = 0
;
;            недопустимое смещение: CF  = 1  
;                                   EAX = -2
;
;
 proc test_off          
;──────────────────────────────────────────────────────────────────────────────
                push    eax
                mov     eax,dword [file_map_address]
                cmp     ebx,eax
                jb      test_off_ret
                add     eax,dword [temp+024]     ; + размер загруженной программы
                add     eax,virtual_stack_size*2 ; + размер вирт. стека (расположен
                                                 ;   сразу за программой) и других
                                                 ; структур
                cmp     ebx,eax
                ja      test_off_ret
 test_off_ok:   pop     eax
                clc
                ret
;
 test_off_ret:  pop     eax
                mov     eax,-2                 ; Set error code: EAX = -2
                stc
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; Указывает ли смещение "off" на виртуальный стэк
;
; on start : off    - смещение
; on exit  : cf = 0 - указывает на вирт. стэк
;            cf = 1 - не указывает на вирт. стэк
;
 proc test_in_vstack    off
;──────────────────────────────────────────────────────────────────────────────
                push    eax ebx
                mov     eax,dword [virtual_stack_pointer]
                mov     ebx,[off]
;
                cmp     ebx,eax
                jb      not_in_vstack
; 
                add     eax,virtual_stack_size
                cmp     ebx,eax
                ja      not_in_vstack
;
                pop     ebx eax 
                clc
                ret
;
 not_in_vstack: pop     ebx eax
                stc
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; real_off - Calculate "real offset" (in antivirus buffer) from
;            the originally virtual adress
;
; on start: voff - virtual adress
;
; on exit:   EAX - calculated "real offset"
;
 proc calc_virtual_offset voff
;──────────────────────────────────────────────────────────────────────────────
                mov     eax,[voff]               ; смещение инструкции в памяти
                sub     eax,dword [image_base_of_pe]
                add     eax,dword [file_map_address]
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; eax = eax * 4
; первоначальное значение регистров ecx, edx сохраняется
;
 proc mul_4                  
;──────────────────────────────────────────────────────────────────────────────
                push    ecx
                push    edx
;
                sub     edx,edx
                mov     dl,4
                mul     edx
;
                pop     edx
                pop     ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────