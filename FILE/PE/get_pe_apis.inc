;──────────────────────────────────────────────────────────────────────────────
; get_imported_apis - Get infomation about API imported by PE-file
;                     Update PE's import table with current Proc addreses
;
; on start: pe_header_location - PE header
;
; on exit:  "imported_api_number" countains number of imported APIs
;
;           buffer "imported_api_buffer" contains information about imported APIs
;           ( FORMAT: pointer to API's offset, pointer to API's ASCII name
;                     and finally pointer to imported API's library name )
;──────────────────────────────────────────────────────────────────────────────
 proc get_imported_apis pe_header_location
;
                push    eax ecx edx ebx ebp esi edi
                mov     esi,[pe_header_location]
                call    _get_imported_apis
                pop     edi esi ebp ebx edx ecx eax
                ret
;
 endp
;───────────────
; on start: ESI - Pointer to PE header
;
 proc _get_imported_apis
;
;
; Init buffers used by subroutine
;
                mov     byte [imported_api_buffer], 0
                mov     dword [imported_api_number], 0
;
                mov     edi,dword [esi+80h]     ; Virt. address of import table RVA
                movzx   edx,word [esi+14h]      ; NT header size
                movzx   ecx,word [esi+06h]      ; Number of sections in PE
 
                lea     edx,[esi+edx+18h]       ; First section information
;
; Find section with import table
;
 check_section: mov     eax,dword [edx+0Ch]     ; Section's VA (virtual)
                add     eax,dword [edx+08h]     ; + size of section (virtual)
;
                cmp     edi,dword [edx+0Ch]          
                jb      next_section            ; Import table < Section's start
                cmp     edi,eax
                jb      found_section           ; Import table fits in this section

 next_section:  add     edx,ot_size
                loop    check_section
                ret                             ; No location of import table has been found
;
 found_section: mov     eax,edi
                sub     eax,dword [edx+0Ch]
                add     eax,dword [edx+14h]
;
; EAX = Physical address of import table (relative)
; EDI = Virtual address of import table (relative)
;
                mov     dword [import_data_raw], eax
                mov     dword [import_data_va], edi
                
                mov     ebp,eax
                add     ebp,dword [file_map_address] ; Import directory start
                mov     ecx,ebp
                add     ecx,dword [esi+84h]     ; Import directory end
                mov     edx,imported_api_buffer
;
 process_libs:  mov     ebx,dword [ebp+0Ch]     ; Pointer to library name (in ASCII)
                or      ebx,ebx
                jz      process_libs_ret        ; all processed
;
                cmp     ebx,dword [esi+50h]
                ja      next_entry              
;
                sub     ebx,edi                 ; - Import table VA
                add     ebx,eax                 ; + Import table RAW data location
                add     ebx,dword [file_map_address] ; = Pointer to LIBRARY NAME
;
                push    eax ecx ebp esi edi
                call    retrive_apis            ; retrive LIB's apis imported by file
                pop     edi esi ebp ecx eax
;
 next_entry:    add     ebp,14h
                cmp     ebp,ecx
                jb      process_libs
;
 process_libs_ret:
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; retrive_apis - Get infomation about API imported by PE-file from 
;                library (library name in "EBX")
;
; on start:         EBX - Pointer to library name (in ASCII)
;                   ESI - Pointer to PE HEADER
;                   EBP - Pointer to current import directory entry
;                   EDX - Pointer to API information buffer to store info there
;
; on exit:          EDX - New pointer to API information buffer
;──────────────────────────────────────────────────────────────────────────────
 proc retrive_apis
;
                push    edx
;
                push    ebx                    
                invoke  GetModuleHandleA
                mov     dword [temp_module_handle],eax
;
                pop     edx
;
                mov     edi,edx                 ; EDI = API information buffer
;
                mov     ecx,dword [ebp+10h]     ; Adress table RVA
                add     ecx,dword [esi+34h]     ; Image base
                push    ecx
                call    real_off
                xchg    ecx,eax                 ; List of pointers to API offsets
;
                mov     edx,dword [ebp]         ; Import LookUp
                sub     edx,dword [import_data_va]
                add     edx,dword [import_data_raw]
                add     edx,dword [file_map_address] ; List of pointers to API names
;
; Get API information (pointers to: api offset, api name and lib name)
; and save it's in to buffer
;
 process_api:   cmp     dword [imported_api_number],MAX_APIS
                jg      process_api_end      
;
 get_api_name:  mov     eax,dword [ecx]         ; Get API name pointer
                or      eax,eax
                js      process_next_api        ; Avoid ordinals
                jz      process_api_end         ; All LIBRARY apis was processed
;
                push    ebx
                xchg    ebx,eax
                sub     ebx,dword [import_data_va]
                add     ebx,dword [import_data_raw]
                add     ebx,dword [file_map_address]
;
; IF API address is already set, then offset to API name is corrupt
;
                call    test_off
                xchg    eax,ebx             
                pop     ebx
                jnc     continue_proc_api
;
 get_api_name_again:
                mov     eax,dword [edx]       
                or      eax,eax
                js      process_next_api        ; Avoid ordinals
                jz      process_api_end         ; All LIBRARY apis was processed
;
                sub     eax,dword [import_data_va]
                add     eax,dword [import_data_raw]
                add     eax,dword [file_map_address]
                                                ; EAX = Pointer to API's name (in ASCII)
                push    ebx
                mov     ebx,eax      
                call    test_off
                pop     ebx
                jc      process_next_api
;
 continue_proc_api:
                inc     eax
                inc     eax                     ; Skip hint (WORD)
                stosd                           ; Store pointer to API NAME
;
 store_apis_offset:
                call    test_api_name
                jc      process_next_api        ; Invalid name
;
                push    ecx edx
                push    eax                     ; EAX = API name
                push    dword [temp_module_handle]
                invoke  GetProcAddress
                pop     edx ecx

                push    edi
                mov     edi,ecx 
                stosd                           ; Update PEs IMPORT table
                pop     edi                     
                
                stosd                           ; Store pointer API offset
;
 store_apis_lib_name:
                mov     eax,ebx
                stosd                           ; Store pointer to current LIB
                inc     dword [imported_api_number]
 process_next_api:
                add     ecx,4                   ; Next offset
                add     edx,4                   ; Next name
                jmp     process_api
;
 process_api_end:
                mov     edx,edi                 ; EDX = New pointer to buffer
                ret                                                    
 endp
;──────────────────────────────────────────────────────────────────────────────
; test_api_name - Test API ASCII name (must consist of english chars, numbers
;                 and may contain "_" char, ends with 0h, max length = 50 bytes)
;
; on start: EAX - Pointer to ASCII name
;
; on exit:  CX=0 - Name correct
;           CX=1 - Name incorrect
;──────────────────────────────────────────────────────────────────────────────
 proc test_api_name          
;
                push    eax ecx esi
                xchg    eax,esi
                sub     ecx,ecx
                add     cl,50
 test_api_loop: lodsb
                or      al,al
                jz      test_api_ret_ok
;
                cmp     al,'0'
                jb      test_api_ret_error
                cmp     al,'9'
                jbe     test_api_next_byte
;
                cmp     al,'A'
                jb      test_api_ret_error
                cmp     al,'Z'
                jbe     test_api_next_byte
                cmp     al,'_'
                jz      test_api_next_byte
                cmp     al,'a'
                jb      test_api_ret_error
                cmp     al,'z'
                ja      test_api_ret_error
;              
 test_api_next_byte:
                loop    test_api_loop
;
 test_api_ret_error:
                pop     esi ecx eax
                stc
                ret
;
 test_api_ret_ok:
                pop     esi ecx eax
                clc
                ret
;
 endp
;