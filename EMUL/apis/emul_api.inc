;──────────────────────────────────────────────────────────────────────────────
; Emulation of API calls
;
; Code and data for emulation of API calls (some high polymorphic engines
; can generate API calls and use them in decryptors to fool AV)
;──────────────────────────────────────────────────────────────────────────────

;
; Emulator can run list of API located in "emul\emul_api_db.inc" 
; (of course with some tests to avoid possible errors)
;

;
;                       * * *   SUBROUTINES   * * *
;

;──────────────────────────────────────────────────────────────────────────────
; emul_API_call - Emulate API call
;
; on start: EAX - Pointer to API information ( in "imported_api_buffer" )
; on exit :  CF = 0 - API can be emulated
;           EAX = API address
;
;            CF = 1 - API cannot be emulated
;
 proc emul_API_call
;
                push    ecx edx ebx esi edi
                mov     esi,imported_api_buffer
                cmp     eax,esi
                jc      emul_API_call_error ; Must be somewhere in "imported_api_buffer"
;                
                mov     ecx,dword [imported_api_number]
                push    eax ecx
                mov     eax,4*3             ; one entry size (in buffer)
                mul     ecx      
                add     eax,esi
                xchg    eax,edx             ; EDX - end of the "imported_apis_buffer"
                pop     ecx eax
                cmp     eax,edx
                ja      emul_API_call_error ; Must be somewhere in "imported_api_buffer"
;
; Find API's information in AV's API database
; (contains all APIs can be emulated by an Explosion)
;           
                mov     edi,dword [eax]       ; API's ASCII name
                call    check_base
                or      eax,eax                
                jz      emul_API_call_error   ; API is not found in list
;
; EMULATE API CALL
;
                xchg    eax,esi               ; Pointer to API's data block
                sub     eax,eax
                mov     al,byte [esi+4+4]     ; Number of API's required params
                or      eax,eax
                jz      emul_API_end          ; No params? Emulation ended
;
; Check if needful number of parameters loaded in virtual stack
;
                push    eax
                call    mul_4
;
                mov     ebx,dword [regs+016]
                add     ebx,eax               ; VStack pointer after API's runned
                xchg    edx,eax               ; Size of params
                pop     eax
;
                call    test_off              ; Test stack pointer
                jc      emul_API_call_error   ; error
;
                add     dword [regs+016],edx  ; Free parameters used by API
;
 emul_API_end:  clc
                jmp     emul_API_call_ret
;
 emul_API_call_error:
                mov     eax,-5                ; Set error code
                stc
 emul_API_call_ret:
                pop     edi esi ebx edx ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; Search current API's ascii name in emulator base
;
;  on start: EDI - ascii name ("000h" means end of string)
;
;  on exit:  EAX - number in emulator base (if founded, or = 0)
;
 proc check_base
;
                push    ecx ebx esi
                sub     ecx,ecx                 ; counter = 0
                inc     ecx                     ; counter = 1
                mov     ebx,edi                 ; Save ASCII name
                mov     esi,API_database
;
 cmp_ascii_names:
                lodsd                           ; API's name offset (in base)
                push    esi                     
                mov     esi,eax
;
                mov     edi,ebx
                call    cmpi_str
                pop     esi                     
                jnc     check_base_ok
;
                add     esi,4+3
                cmp     esi,API_database_end
                jb      cmp_ascii_names
                jmp     check_base_not_match
;
 check_base_ok: sub     esi,4
                mov     eax,esi
                jmp     check_base_ret
;
 check_base_not_match:
                sub     eax,eax
 check_base_ret:
                pop     esi ebx ecx
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; check_if_imported_api_call - This procedure checks one of PE's imported
;                              APIs if probably will be called
;
; on start: EAX = API virtual offset
;
; on exit :  CF = 0 - API is not called 
;           EAX = 0
;
;            CF = 1 - API is called
;           EAX = Pointer to API information ( in "imported_api_buffer" )
;
 proc check_if_imported_api_call
;
                push    ecx ebp esi
                mov     ecx,dword [imported_api_number]
                or      ecx,ecx
                jz      check_if_imported_api_call_ret ; PE not import APIs
;
                mov     ebp,eax
                mov     esi,imported_api_buffer
 api_check_loop:
                lodsd                     ; Skip pointer to API name
                lodsd
                cmp     ebp,eax           ; !!!
                jz      one_of_imported_apis_called
                lodsd                     ; Skip pointer to API's library name
                loop    api_check_loop    ; NEXT API
                sub     eax,eax       
;
 check_if_imported_api_call_ret:
                clc
                pop     esi ebp ecx
                ret
;
 one_of_imported_apis_called:
 
                sub     esi,8
                mov     eax,esi  ; Set pointer to API information ( in "imported_api_buffer" )                 
                stc
                jmp     check_if_imported_api_call_ret+1
;
 endp
;