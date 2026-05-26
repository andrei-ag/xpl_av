; процедура лечения вируса w32/krized (c) 2004, beauty on the fire
;
;──────────────────────────────────────────────────────────────────────────────
.code
;──────────────────────────────────────────────────────────────────────────────
; krized_4029 - лечение w32/krized [4029]
;               в PE-файлах и библиотеке KERNEL32.DLL
;
 krized_4029    proc
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     esi,dword ptr [emul_stop] ; точка остановки эмулятора
;
                cmp     word ptr [ph.Flags],2000h
                jc      kriz_4029_pe
;───────────────
; * * * Вирус обнаружен в библиотеке KERNEL32.DLL (формат PE)
;
;  - Смещения 16 функций API, в системной библиотеке, указывают на обработчик
;    внутри вируса.
;
;  - ph.DLL_flags   - характеристика DLL (было 0, стало 1)
;
 kriz_4029_k32: push    esi
                add     esi,2996+164
                sub     ecx,ecx
                mov     cl,16
 k4029_k32_lp:  lodsd
                push    ecx esi
                push    FILE_BEGIN 0 eax
                push    handle
                call    SetFilePointer

                sub     esi,164+4
                push    004 esi
                call    write_file
                pop     esi ecx
                loop    k4029_k32_lp
                pop     esi
;
                mov     word ptr [ph.DLL_Flags],0
;───────────────
; * * * Вирус обнаружен в исполняемом файле, формата Portable Executable
;
;  Необходимо восстановить исходные значение в заголовке PE, которые были
; изменены. При внедрении в PE-файл, вирус изменяет в заголовке:
;
;  - ph.EntryPoint  - точку входа
;                     сохраняется вирусом в оригинальном виде
;  - ph.SizeOfIm    - виртуальный размер
;  - ph.CSum        - этот параметр не используется (т.е. не будем его менять),
;                     но может применяться вирусами, как метка зараженности
;                     файла
;
 kriz_4029_pe:  mov     eax,dword ptr [esi+000027]
                mov     dword ptr [ph.EntryPoint],eax
;
                mov     eax,dword ptr [temp+012]
                sub     eax,offset buffer        ; точка входа в файле
                mov     edi,dword ptr [ot_loc]   ; edi - данные о последней секции
                sub     eax,dword ptr [edi+20d]  ; eax - длина данных в секции без вируса
                push    eax                      ; ***
                mov     ecx,dword ptr [ph.SectAlign]
                call    align_value              ; выровняем относительно ОЗУ
;
                mov     dword ptr [edi+08d],eax  ; размер данных последней секции в ОЗУ
                add     eax,dword ptr [edi+12d]  ; размер данных всех секций в ОЗУ
                mov     dword ptr [ph.SizeOfIm],eax
;───────────────
                push    FILE_BEGIN 0 ph_loc
                push    handle                   ; установим указатель на
                call    SetFilePointer           ; смещение заголовка PE
;
                push    ph_size
                push    offset ph                ; запишем измененную
                call    write_file               ; часть заголовка
;───────────────
; В последней секции меняется:
;
;  - ot.VirtSize - если свободного места не достаточно,
;                  увеличивается на размер вирусного кода
;  - ot.PhSize   - аналогично изменениям в ot.VirtSize
;  - ot.ObjFlags - 11010000b
;
                pop     eax                      ; ***
                mov     ecx,dword ptr [ph.FileAlign]
                call    align_value              ; выровняем относительно файла
                mov     dword ptr [edi+16d],eax
;
                mov     eax,dword ptr [ot_loc]
                sub     eax,offset all_ot
                add     eax,dword ptr [ot_1st]
;
                push    FILE_BEGIN 0 eax
                push    handle                   ; установим указатель на
                call    SetFilePointer           ; данные последней секции
;
                mov     eax,dword ptr [ot_loc]
                push    ot_size eax
                call    write_file               ; перезапишем данные секции
;───────────────
;  Вирусный код начинается с полиморфного декриптора, в точке входа. Так как
; вирус записывает свой код в самый конец программы, нужно установить
; указатель на смещение точки входа и обрезать его.
;
                mov     eax,dword ptr [temp+012]
                sub     eax,offset buffer
                push    FILE_BEGIN 0 eax
                push    handle
                call    SetFilePointer

                push    handle
                call    SetEndOfFile
;───────────────
 krized_4029_r: popad
                sub     edx,edx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────