; процедура лечения вируса w95/marburg (c) 2004, beauty on the fire
;
;──────────────────────────────────────────────────────────────────────────────
.code
;──────────────────────────────────────────────────────────────────────────────
; marburg_8582 - лечение w95/marburg [8582] в PE-файлах
;
 marburg_8582   proc
;──────────────────────────────────────────────────────────────────────────────
                pushad
;───────────────
                sub     eax,eax
                mov     ax,word ptr [ph.NOfSections]
                dec     eax                      ; eax - кол-во секций - 1
                sub     edx,edx
                mov     dl,ot_size               ; edx - размер одной секции
                mul     edx                      ; eax = eax * edx
                add     eax,offset all_ot        ; смещение последней секции
;                                                ; в буфере "all_ot"
                xchg    eax,esi
                mov     eax,dword ptr [esi+008d]
                add     eax,dword ptr [esi+012d]
                mov     ecx,dword ptr [ph.SectAlign]
                call    align_value              ; * выровняем относительно ОЗУ
                mov     dword ptr [ph.SizeOfIm],eax
;
                push    FILE_BEGIN 0 ph_loc
                push    handle                   ; * установим указатель на
                call    SetFilePointer           ;   смещение заголовка PE в файле
;
                push    054h offset ph
                call    write_file               ; * запишем измененный заголовок
;───────────────
; Востановим 256 оригинальный байт в точке входа.
;
                mov     eax,dword ptr [temp+012]
                sub     eax,offset buffer        ; - смещение точки входа в файл

                push    FILE_BEGIN 0 eax
                push    handle                   ; * установим указатель на
                call    SetFilePointer           ;   смещение точки входа

                mov     esi,dword ptr [emul_stop]
                add     esi,001595h              ; - esi - смещение ориг. байт
                push    100h esi                 ; * запишем оригинальные байты
                call    write_file               ;   вместо вирусного вызова
;───────────────
;  Точка входа в вирусный код расположена в точке остановки эмулятора.
;  Обрежем файл по эту позицию.
;
                mov     eax,dword ptr [emul_stop]
                sub     eax,offset buffer        ; - смещение точки входа в вирус

                push    FILE_BEGIN 0 eax
                push    handle                   ; * переводим указатель на
                call    SetFilePointer           ;   начало вирусного тела

                push    handle
                call    SetEndOfFile             ; * установим конец файла
;───────────────
                popad
                sub     edx,edx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
