 proc load_pe   
;
                push    eax ecx ebx ebp edi     
;
                mov     eax,dword [temp+024]     ; Размер файла
                add     eax,virtual_stack_size*2 ; + виртуальный стек
                                                 ; + вспомогательный стек
                                                 ;   для хранения смещений
                push    eax
                push    4h+8h                    ; HEAP_GENERATE_EXCEPTIONS
                                                 ; HEAP_ZERO_MEMORY
                push    [heap_handle]            ; Выделим участок памяти
                invoke  HeapAlloc                ; под разбираемый файл
                mov     dword [file_map_address],eax
                or      eax,eax
                jnz     load_pe_read_file
                stc
                jmp     load_pe_ret
                 		
 load_pe_read_file:
                add     eax,dword [temp+024]     ; Указатель на конец файла в памяти
                                                 ; он же, начало вирт. стека
                mov     dword [virtual_stack_pointer],eax
                add     eax,virtual_stack_size*2
                mov     dword [offset_stack_ptr],eax
;                
                push    FILE_BEGIN 0 0 [handle]  ; установим указатель на
                invoke  SetFilePointer           ; самое начало файла
;
                mov     edi,header_temp_buffer   ; EDI = Начало заголовка PE
;
                sub     ebx,ebx
                mov     bx,word [edi+014h]       ; Size of optional header
                add     ebx,018h
                add     ebx,dword [ph_loc]       ; EDX = смещение данных 1ой
                                                 ;       секции, в файле
;
                sub     edx,edx
                mov     dx,word [edi+006h]       ; Количество секций файла
                push    edx
                mov     eax,028h
                mul     edx
;
                add     eax,ebx
                push    eax
                push    dword [file_map_address] ; Прочитаем весь заголовок 
                call    read_file                ; файла, включая данные о секциях
                                                 ; и первую секцию
;
                add     ebx,dword [file_map_address]; Смещение данных первой
                                                 ; секции в памяти
                sub     ebp,ebp                  ; EBP = Дельта на которую
                                                 ; был увеличен физический
                                                 ; размер предыдущих секции
                pop     ecx                      ; ECX = Количество секций
;
 load_pe_loop:  
;
; Если до этого момента размер секций увеличивался:
;
;  1) Увеличим положение секции в файле на дельту (увеличение предыдущих секций)
;  2) Перезапишем данные секции в новую позицию
;
                mov     eax,dword [ebx+020d]     ; Положение секции в файле
;
                push    ecx
                push    FILE_BEGIN
                push    0
                push    eax
                push    [handle]                 ; установим указатель на
                invoke  SetFilePointer           ; начало текущей секции в файле
                pop     ecx
;
                add     eax,ebp                  ; Увеличим физ. положение секции
                add     dword [ebx+020d],ebp  
                add     eax,dword [file_map_address]
                                                 ; Новое положение секции в буфере
;
                push    ecx
                push    dword [ebx+016d]         ; прочитаем секцию в 
                push    eax                      ; ее новое месторасположение
                call    read_file                ; относительно файлового буфера
                pop     ecx
;
 load_pe_process_section:
                mov     eax,dword [ebx+008d]     ; Виртуальный размер секции
                cmp     eax,dword [ebx+016d]     ; Сравниваем с физическим
                                                 ; размером секции
                jbe     load_pe_next_section     ; VSize <= Physical size
;
;  Если физический размер меньше виртуального, сравняем их (раздвинем 
; размер секции в файле и изменим параметры секции)
;
                sub     eax,dword [ebx+016d]     ; Разница размеров
                add     dword [ebx+016d],eax     ; Физ. размер = Вирт. размер
                add     ebp,eax                  ; Увеличим общую дельту
;
 load_pe_next_section:
                add     ebx,ot_size              ; След. секция
                loop    load_pe_loop
;
                clc
 load_pe_ret:   pop     edi ebp ebx ecx eax
                ret
;
 endp
;