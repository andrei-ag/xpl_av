; процедура лечения вируса w32/parite (c) 2004, beauty on the fire
;
;──────────────────────────────────────────────────────────────────────────────
.code
;──────────────────────────────────────────────────────────────────────────────
; parite_b_pe - лечение w32/parite (штамм b) в PE-файлах
;
 parite_b_pe    proc
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     esi,dword ptr [emul_stop]
;───────────────
;
;  Необходимо восстановить исходные значение в заголовке PE, которые были
; изменены. При внедрении в PE-файл, вирус изменяет в заголовке:
;
;  - ph.NOfSections - количество секций, добавляется еще одна (с кодом вируса)
;  - ph.EntryPoint  - точку входа указывает на вирусный декриптор, содержащийся
;                     в последней секции
;  - ph.SizeOfIm    - виртуальный размер, увеличивается на 1000h
;
;  Старая точка входа (предусмотрительно) сохраняется вирусом.
;
                mov     eax,dword ptr [esi-00000176h] ; ориг. entry_point
                mov     dword ptr [ph.EntryPoint],eax
                sub     dword ptr [ph.SizeOfIm],1000h
                dec     word ptr [ph.NOfSections]     ; уменьшим кол-во секций
;───────────────
                push    FILE_BEGIN 0 ph_loc
                push    handle                   ; установим указатель на
                call    SetFilePointer           ; смещение заголовка PE
;
                push    ph_size
                push    offset ph                ; запишем измененную
                call    write_file               ; часть заголовка
;───────────────
;  Вирус так же изменяет 12 байт в двух смещениях оригинальной программы
; (данные в таблице импорта функций), смещения и оригинальные значение
; запоминает в своем теле.
;
                sub     esi,00000156h
                sub     ecx,ecx
                mov     cl,2
 p_impr_res_lp: push    ecx
                lodsd
                push    FILE_BEGIN 0 eax
                push    handle
                call    SetFilePointer

                push    008 esi
                call    write_file
                lodsd
                lodsd
                pop     ecx
                loop    p_impr_res_lp
;───────────────
;  Данные о вирусной секции не будут мешать программе, но лучше будет их
; затереть, что бы не вызывали подозрений ...
;
                mov     eax,dword ptr [ot_loc]
                sub     eax,offset all_ot
                add     eax,dword ptr [ot_1st]

                push    FILE_BEGIN 0 eax
                push    handle                   ; установим указатель на
                call    SetFilePointer           ; данные последней секции
;
                push    ot_size                  ; затрем последнюю секцию
                push    offset zero_ot           ; блоком из 40 нулей, тк
                call    write_file               ; эта секция была создана вирусом
;───────────────
;  В данных о вирусной секции указано начало секции в файле (ot.PhOffset),
; именно с этой позиции и будет начинаться вирусный код. Так как вирус
; записывает свой код в самый конец программы, т.е. нужно установить указатель
; на начало последней секции в файле и обрезать его.
;
                mov     esi,dword ptr [ot_loc]               
                mov     eax,dword ptr [esi+20d]
                push    FILE_BEGIN 0 eax
                push    handle
                call    SetFilePointer

                push    handle
                call    SetEndOfFile
;───────────────
                popad
                sub     edx,edx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────