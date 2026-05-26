
;──────────────────────────────────────────────────────────────────────────────
;
; Таблица ссылок на процедуры, предназначенные для эмуляции инструкций
;
 emul_table_of_procs:
;                          
 dd emul_000            ; 000: " add byte ptr [...],reg "
 dd emul_001            ; 001: " add dword ptr [...],ereg "
 dd emul_002            ; 002: " add reg, byte ptr [...] "
 dd emul_003            ; 003: " add ereg,dword ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_008            ; 008: " or byte ptr [...],reg "
 dd emul_009            ; 009: " or dword ptr [...],ereg "
 dd emul_00A            ; 00A: " or reg,byte ptr [...] "
 dd emul_00B            ; 00B: " or ereg, dword ptr [...] "
 dd 000
 dd 000
 dd emul_push_pop       ; 00E: " push cs "
 dd emul_00F            ; 00F: 
 dd emul_010
 dd emul_011            ; 011: " adc dword ptr [...],ereg " 
 dd emul_012            ; 012: " adc reg,byte ptr [...] "
 dd emul_013            ; 013: " adc ereg,dword ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_018            ; 018: " sbb byte ptr [...],reg "
 dd emul_019            ; 019: " sbb dword ptr [...],ereg "
 dd emul_01A            ; 01A: " sbb reg,byte ptr [...] "
 dd emul_01B            ; 01B: " sbb ereg,dword ptr [...] "
 dd 000
 dd 000
 dd emul_push_pop       ; 01E: " push ds "
 dd 000                 ; 01F
 dd emul_020            ; 020: " and byte ptr [...],reg "
 dd emul_021            ; 021: " and dword ptr [...],ereg "
 dd emul_022            ; 022: " and reg,byte ptr [...] "
 dd emul_023            ; 023: " and ereg, dword ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_028            ; 028: " sub byte ptr [...],reg "
 dd emul_029            ; 029: " sub dword ptr [EREG],ereg "
 dd emul_02A            ; 02A: " sub reg,byte ptr [...] / sub rega,regb "
 dd emul_02B            ; 02B: " sub ereg,dword ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_030            ; 030: " xor byte ptr [...],reg "
 dd emul_031            ; 031: " xor dword ptr [...],ereg "
 dd emul_032            ; 032: " xor reg,byte ptr [...] "
 dd emul_033            ; 033: " xor ereg,dword ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_push_pop       ; 050: " push eax "
 dd emul_push_pop       ; 051: " push ecx "
 dd emul_push_pop       ; 052: " push edx "
 dd emul_push_pop       ; 053: " push ebx "
 dd emul_push_pop       ; 054: " push esp "
 dd emul_push_pop       ; 055: " push ebp "
 dd emul_push_pop       ; 056: " push esi "
 dd emul_push_pop       ; 057: " push edi "
 dd emul_push_pop       ; 058: " pop eax "
 dd emul_push_pop       ; 059: " pop ecx "
 dd emul_push_pop       ; 05A: " pop edx "
 dd emul_push_pop       ; 05B: " pop ebx "
 dd emul_push_pop       ; 05C: " pop esp "
 dd emul_push_pop       ; 05D: " pop ebp "
 dd emul_push_pop       ; 05E: " pop esi "
 dd emul_push_pop       ; 05F: " pop edi "
 dd emul_push_pop       ; 060: " pushad "
 dd emul_push_pop       ; 061: " popad "
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_066            ; 066: " xxx word ptr [...],word "
 dd emul_067            ; 067: " Change default ADDRESS size "
 dd emul_push_pop       ; 068: " push dword "
 dd 000                 ; 069
 dd emul_push_pop       ; 06A: " push byte "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_jmp_near       ; 070: " jo near "
 dd emul_jmp_near       ; 071: " jno near "
 dd emul_jmp_near       ; 072: " jb near "
 dd emul_jmp_near       ; 073: " jnb near "
 dd emul_jmp_near       ; 074: " je near "
 dd emul_jmp_near       ; 075: " jnz near "
 dd emul_jmp_near       ; 076: " jbe near "
 dd emul_jmp_near       ; 077: " ja near "
 dd emul_jmp_near       ; 078: " js near "
 dd emul_jmp_near       ; 079: " jns near "
 dd emul_jmp_near       ; 07A: " jpe near "
 dd emul_jmp_near       ; 07B: " jpo near "
 dd emul_jmp_near       ; 07C: " jl near "
 dd emul_jmp_near       ; 07D: " jge near "
 dd emul_jmp_near       ; 07E: " jle near "
 dd emul_jmp_near       ; 07F: " jg near "
 dd emul_080            ; 080: " xxx byte ptr [...],byte "
 dd emul_081            ; 081: " xxx dword ptr [...],dword "
 dd 000                 ; 082
 dd emul_083            ; 083: " xxx dword ptr [...],byte "
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_088            ; 088: " mov byte ptr [...],reg "
 dd emul_089            ; 089: " mov dword ptr [...],ereg "
 dd emul_08A            ; 08A: " mov reg,byte ptr [...] "
 dd emul_08B            ; 08B: " mov ereg,dword ptr [...] "
 dd 000                 ; 08C
 dd emul_08D            ; 08D: " lea ereg,dword ptr [...] "
 dd 000                 ; 08E
 dd emul_08F            ; 08F: " pop dword ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_0AX            ; 0A0: " mov al, byte ptr [DWORD] "
 dd emul_0AX            ; 0A1: " mov eax, dword ptr [DWORD] "
 dd emul_0AX            ; 0A2: " mov byte ptr [DWORD], al "
 dd emul_0AX            ; 0A3: " mov dword ptr [DWORD], eax "
 dd emul_movs_bwd       ; 0A4: " movsb "
 dd emul_movs_bwd       ; 0A5: " movsd "
 dd emul_cmps_bwd       ; 0A6: " cmpsb "
 dd emul_cmps_bwd       ; 0A7: " cmpsd "
 dd 000
 dd 000
 dd emul_stos_bwd       ; 0AA: " stosb "
 dd emul_stos_bwd       ; 0AB: " stosd "
 dd emul_lods_bwd       ; 0AC: " lodsb "
 dd emul_lods_bwd       ; 0AD: " lodsd "
 dd emul_scas_bwd       ; 0AE: " scasb "
 dd emul_scas_bwd       ; 0AF: " scasd "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_ret            ; 0C3: " ret "
 dd 000
 dd 000
 dd emul_0C6            ; 0C6: " mov byte ptr [...],reg "
 dd emul_0C7            ; 0C7: " mov dword ptr [...],ereg "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_0DA            ; 0DA: COproc instr
 dd emul_0DB            ; 0DB: COproc instr
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_loop_byte      ; 0E2: " loop byte "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_call_far       ; 0E8: " call FAR dword "
 dd emul_jump_far_goto  ; 0E9: " jmp FAR dword "
 dd 000                 ; 0EA
 dd emul_jmp_near       ; 0EB: " jmp NEAR "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_0F6            ; 0F6: " xxx byte ptr [...] "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd emul_0FE            ; 0FE: " xxx byte ptr [...] "
 dd emul_0FF            ; 0FF: " xxx dword ptr [...] "
;
 emul_table_of_procs_end: