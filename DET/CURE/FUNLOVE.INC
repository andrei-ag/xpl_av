; процедура лечения вируса win32/funlove (c) 2004, beauty on the fire
;
;──────────────────────────────────────────────────────────────────────────────
.code
;──────────────────────────────────────────────────────────────────────────────
; funlove - лечение win32/funlove [4099] в PE-файлах
;
 funlove        proc
;──────────────────────────────────────────────────────────────────────────────
                pushad
;───────────────
;  Заражая исполняемые файлы, формата PE, вирус увеличивает размер последней
; секции (ot.PhSize, на 1000h) и записывает туда свое тело. В точке входа
; изменяет 8 байт оригинального кода, в результате получается:
;
;      nop
;      nop
;      call  gs:[смещение указывающее на начало вирусного кода]
;
                mov     eax,dword ptr [temp+012]
                sub     eax,offset buffer        ; смещение точки входа в файл

                push    FILE_BEGIN 0 eax
                push    handle                   ; установим указатель на
                call    SetFilePointer           ; смещение точки входа

                mov     esi,dword ptr [emul_stop]
                add     esi,0009A7h              ; esi - смещение ориг. байт
                push    008 esi                  ; запишем оригинальные байты
                call    write_file               ; вместо вирусного вызова
;───────────────
; В последней секции изменяется значение ot.PhSize
;
                sub     eax,eax
                sub     edx,edx
                mov     ax,word ptr [ph.NOfSections]
                dec     eax                      ; eax - кол-во секций - 1
                mov     dl,ot_size               ; edx - размер одной секции
                mul     edx                      ; eax = eax * edx
                add     eax,dword ptr [ot_1st]   ; смещение последней секции в файле
;
                push    eax
                push    FILE_BEGIN 0 eax
                push    handle                   ; установим указатель на
                call    SetFilePointer           ; данные последней секции
;
                pop     eax
                add     eax,offset buffer        ; расположение секции в буфере
                sub     dword ptr [eax+016d],1000h
;               sub     dword ptr [eax+008d],4000h
                push    020 eax                  ; перезапишем все данные
                call    write_file               ; последней секции, до PhOffset
;───────────────
;  Точка входа в вирусный код рассположена в 512 байтах от начала вирусного
; тела, таким образом если расчитать расположение точки входа в файле и
; отнять 512 байт получим позицию, по которую необходимо обрезать файл.
;
                mov     eax,dword ptr [emul_stop]
                sub     eax,offset buffer        ; смещение точки входа в вирус
                sub     eax,000512d              ; начало вирусного тела

                push    FILE_BEGIN 0 eax
                push    handle                   ; переводим указатель на
                call    SetFilePointer           ; начало вирусного тела

                push    handle
                call    SetEndOfFile             ; устанавливаем конец файла
;───────────────
 funlove_ret:   popad
                sub     edx,edx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
