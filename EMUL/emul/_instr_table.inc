;──────────────────────────────────────────────────────────────────────────────
; get_emul_proc - ищет процедуру для эмуляции в таблице
;
; on start : al     - первый байт инструкции
; on exit  :    EAX = Смещение процедуры 
;               EAX = 0 если процедура для эмуляции отсутствует
;
 proc get_emul_proc          
;
                push    ecx ebx esi
                mov     ebx,eax
                mov     esi,emul_table_of_procs
                sub     ecx,ecx 
                mov     cl,(emul_table_of_procs_end-emul_table_of_procs)/5
;
 get_emul_proc_loop:
                lodsb
                cmp     al,bl
                jnz     get_emul_proc_next
                lodsd
                jmp     get_emul_proc_ret  ; Found emulation PROC
;
 get_emul_proc_next:
                lodsd
                loop    get_emul_proc_loop
;
                sub     eax,eax
 get_emul_proc_ret:
                pop     esi ebx ecx
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
;
; Таблица ссылок на процедуры предназначенные для эмуляции инструкций
;
 emul_table_of_procs:
;                          
 db 000h
 dd emul_000
 db 001h
 dd emul_001
 db 003h
 dd emul_003
 db 008h
 dd emul_008
 db 009h
 dd emul_009
 db 00Ah
 dd emul_00A
 db 00Eh
 dd emul_push_pop ; " push cs "
 db 00Fh
 dd emul_00F
 db 010h
 dd emul_010
 db 019h
 dd emul_019
 db 01Bh
 dd emul_01B
 db 01Eh
 dd emul_push_pop ; " push ds "
 db 020h
 dd emul_020
 db 021h
 dd emul_021
 db 029h
 dd emul_029
 db 02Bh
 dd emul_02B
 db 030h
 dd emul_030
 db 031h
 dd emul_031
 db 032h
 dd emul_032
 db 033h
 dd emul_033
 db 060h
 dd emul_push_pop ; " pushad "
 db 061h
 dd emul_push_pop ; " popad "
 db 066h
 dd emul_066
 db 068h
 dd emul_push_pop ; " push dword "
 db 06Ah
 dd emul_push_pop ; " push byte "
 db 080h
 dd emul_080
 db 081h
 dd emul_081
 db 083h
 dd emul_083
 db 088h
 dd emul_088
 db 089h
 dd emul_089
 db 08Ah
 dd emul_08A
 db 08Bh
 dd emul_08B
 db 08Dh
 dd emul_08D
 db 08Fh
 dd emul_08F
 db 0A6h                 ;
 dd emul_cmps_bwd ; " cmpsb "
 db 0A7h
 dd emul_cmps_bwd ; " cmpsd "
 db 0AAh
 dd emul_stos_bwd ; " stosb "
 db 0ABh
 dd emul_stos_bwd ; " stosd "
 db 0ACh
 dd emul_lods_bwd ; " lodsb "
 db 0ADh
 dd emul_lods_bwd ; " lodsd "
 db 0C3h
 dd emul_ret      ; " ret "
 db 0C7h
 dd emul_0C7
 db 0E2h
 dd emul_loop_byte      ; " loop byte "
 db 0E8h
 dd emul_call_far       ; " call FAR dword "
 db 0E9h
 dd emul_jump_far_goto  ; " jmp FAR dword "
 db 0EBh                       
 dd emul_jmp_near       ; " jmp NEAR "
 db 0F6h
 dd emul_0F6
 db 0FEh
 dd emul_0FE
 db 0FFh
 dd emul_0FF
;
 emul_table_of_procs_end: