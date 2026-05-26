; процедура лечения вируса win32/thorin.XXXXX
;
;──────────────────────────────────────────────────────────────────────────────
.code
;──────────────────────────────────────────────────────────────────────────────
; win32_thorin - лечение win32/thorin [xxxxx] в PE-файлах
;
 win32_thorin   proc
                int     03h
                call    win32_thorin_rda_crypt
                mov     ebx, dword ptr [emul_stop]
     
                mov     dword ptr [ph.EntryPoint],eax
                ret
 win32_thorin   endp
                
;──────────────────────────────────────────────────────────────────────────────
win32_thorin_rda_crypt       proc
	xor     ebx,ebx                         ; Clear counter
        
try_another_key:
	call    crypt                           ; Try to decrypt it
	push    ebx                             ; Save counter
        mov     esi,dword ptr [regs+024]        ; Load address to crypt
        mov     edi,dword ptr [regs+028]        ; Size to crypt
	call    win32_thorin_CRC32              ; Get its CRC32
	pop     ebx                             ; Restore counter
        mov     ecx, dword ptr [emul_stop]
       	cmp     eax, dword ptr [ecx+07]         ; Actual CRC32=CRC32 unencrypted? 
	jz      rda_done                        ; Yeah, then we decrypted it
	call    crypt                           ; Nopes, fix it
	inc     ebx                             ; increase key
	jmp     try_another_key                 ; Try with another key
rda_done:
        int 3h
        
	ret
win32_thorin_rda_crypt       endp

crypt           proc                            ; This procedures simplifies
	mov     ecx,dword ptr [regs+028]        ; the task (and optimizes) of
	mov     edi,dword ptr [regs+024]        ; encrypt with a determinated
rda_:   xor     byte ptr [edi],bl               ; key
	inc     edi
	loop    rda_
	ret
crypt           endp


win32_thorin_CRC32           proc
	cld
	push    ebx
	xor     ecx,ecx                         ; Optimized by me - 2 bytes
	dec     ecx                             ; less
	mov     edx,ecx
NextByteCRC:
	xor     eax,eax
	xor     ebx,ebx
	lodsb
	xor     al,cl
	mov     cl,ch
	mov     ch,dl
	mov     dl,dh
	mov     dh,8
NextBitCRC:
	shr     bx,1
	rcr     ax,1
	jnc     NoCRC
	xor     ax,08320h
	xor     bx,0EDB8h
NoCRC:  dec     dh
	jnz     NextBitCRC
	xor     ecx,eax
	xor     edx,ebx
	dec     edi                             ; Another fool byte less
	jnz     NextByteCRC
	not     edx
	not     ecx
	pop     ebx
	mov     eax,edx
	rol     eax,16
	mov     ax,cx
	ret
win32_thorin_CRC32           endp

comment %
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
 %
;──────────────────────────────────────────────────────────────────────────────
