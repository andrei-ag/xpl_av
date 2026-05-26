
;──────────────────────────────────────────────────────────────────────────────
;
; Таблица ссылок на процедуры, предназначенные для дизассемблирования инструкций
;
 disasm_table_of_procs:
;                          
 dd @000_disasm         ; 000: " add byte ptr [...],reg "
 dd @001_disasm         ; 001: " add dword ptr [...],ereg "
 dd @002_disasm         ; 002: " add reg, byte ptr [...] / add rega,regb "
 dd @003_disasm         ; 003: " add ereg,dword ptr [...] "
 dd _@two_bytes         ; 004: " add al, byte "
 dd _@five_bytes        ; 005: " add eax, dword "
 dd 000
 dd 000
 dd @008_disasm         ; 008: " or byte ptr [...],reg "
 dd @009_disasm         ; 009: " or dword ptr [...],ereg "
 dd @00A_disasm         ; 00A: " or reg,byte ptr [...] "
 dd @00B_disasm         ; 00B: " or ereg, dword ptr [...] / or erega,eregb "
 dd _@two_bytes         ; 00C: " or al, byte "
 dd _@five_bytes        ; 00D: " or eax,dword "
 dd _@one_spec          ; 00E: " push cs "
 dd @00F_disasm         ; 00F: 
 dd @010_disasm         ; 010:
 dd @011_disasm         ; 011: " adc dword ptr [...],ereg " 
 dd @012_disasm         ; 012: " adc reg,byte ptr [...] "
 dd @013_disasm         ; 013: " adc ereg,dword ptr [...] "
 dd _@two_bytes         ; 014: " adc al, byte "
 dd _@five_bytes        ; 015: " adc eax, dword "
 dd 000
 dd 000
 dd @018_disasm         ; 018: " sbb byte ptr [...],reg "
 dd @019_disasm         ; 019: " sbb dword ptr [...],ereg "
 dd @01A_disasm         ; 01A: " sbb reg,byte ptr [...] "
 dd @01B_disasm         ; 01B: " sbb ereg,dword ptr [...] "
 dd _@two_bytes         ; 01C: " sbb al, byte "
 dd _@five_bytes        ; 01D: " sbb eax, dword "
 dd _@one_spec          ; 01E: " push ds "
 dd 000                 ; 01F
 dd @020_disasm         ; 020: " and byte ptr [...],reg "
 dd @021_disasm         ; 021: " and dword ptr [...],ereg "
 dd @022_disasm         ; 022: " and reg,byte ptr [...] "
 dd @023_disasm         ; 023: " and ereg, dword ptr [...] "
 dd _@two_bytes         ; 024: " and al, byte "
 dd _@five_bytes        ; 025: " and eax, dword "
 dd 000
 dd 000
 dd @028_disasm         ; 028: " sub byte ptr [...],reg "
 dd @029_disasm         ; 029: " sub dword ptr [EREG],ereg "
 dd @02A_disasm         ; 02A: " sub reg,byte ptr [...] / sub rega,regb "
 dd @02B_disasm         ; 02B: " sub ereg,dword ptr [...] "
 dd _@two_bytes         ; 02C: " sub al, byte "
 dd _@five_bytes        ; 02D: " sub eax, dword "
 dd 000
 dd 000
 dd @030_disasm         ; 030: " xor byte ptr [...],reg "
 dd @031_disasm         ; 031: " xor dword ptr [...],ereg "
 dd @032_disasm         ; 032: " xor reg,byte ptr [...] "
 dd @033_disasm         ; 033: " xor ereg,dword ptr [...] "
 dd _@two_bytes         ; 034: " xor al, byte "
 dd _@five_bytes        ; 035: " xor eax, dword "
 dd 000
 dd 000
 dd _@instr_2ereg       ; 038: " cmp rega, regb " (8 bit)
 dd _@instr_2ereg       ; 039: " cmp erega, eregb " (32 bit)
 dd _@instr_2ereg       ; 03A: " cmp rega, regb " (8 bit)
 dd _@instr_2ereg       ; 03B: " cmp erega, eregb " (32 bit)
 dd _@two_bytes         ; 03C: " cmp al, byte "
 dd _@five_bytes        ; 03D: " cmp eax, dword "
 dd @03E_disasm         ; 03E: " ds: "
 dd 000
 dd _@one_byte          ; 040: " inc eax "
 dd _@one_byte          ; 041: " inc ecx "
 dd _@one_byte          ; 042: " inc edx "
 dd _@one_byte          ; 043: " inc ebx "
 dd _@one_byte          ; 044: " inc esp "
 dd _@one_byte          ; 045: " inc ebp "
 dd _@one_byte          ; 046: " inc esi "
 dd _@one_byte          ; 047: " inc edi "
 dd _@one_byte          ; 048: " dec eax "
 dd _@one_byte          ; 049: " dec ecx "
 dd _@one_byte          ; 04A: " dec edx "
 dd _@one_byte          ; 04B: " dec ebx "
 dd _@one_byte          ; 04C: " dec esp "
 dd _@one_byte          ; 04D: " dec ebp "
 dd _@one_byte          ; 04E: " dec esi "
 dd _@one_byte          ; 04F: " dec edi "
 dd _@one_spec          ; 050: " push eax "
 dd _@one_spec          ; 051: " push ecx "
 dd _@one_spec          ; 052: " push edx "
 dd _@one_spec          ; 053: " push ebx "
 dd _@one_spec          ; 054: " push esp "
 dd _@one_spec          ; 055: " push ebp "
 dd _@one_spec          ; 056: " push esi "
 dd _@one_spec          ; 057: " push edi "
 dd _@one_spec          ; 058: " pop eax "
 dd _@one_spec          ; 059: " pop ecx "
 dd _@one_spec          ; 05A: " pop edx "
 dd _@one_spec          ; 05B: " pop ebx "
 dd _@one_spec          ; 05C: " pop esp "
 dd _@one_spec          ; 05D: " pop ebp "
 dd _@one_spec          ; 05E: " pop esi "
 dd _@one_spec          ; 05F: " pop edi "
 dd _@one_spec          ; 060: " pushad "
 dd _@one_spec          ; 061: " popad "
 dd 000
 dd 000
 dd @064_disasm         ; 064: " fs: "
 dd @065_disasm         ; 065: " gs: "
 dd @066_disasm         ; 066: " xxx word ptr [...],word "
 dd @067_disasm         ; 067: " Change default ADDRESS size "
 dd @068_disasm         ; 068: " push dword "
 dd 000                 ; 069
 dd _@two_spec          ; 06A: " push byte "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd _@two_spec          ; 070: " jo near "
 dd _@two_spec          ; 071: " jno near "
 dd _@two_spec          ; 072: " jb near "
 dd _@two_spec          ; 073: " jnb near "
 dd _@two_spec          ; 074: " je near "
 dd _@two_spec          ; 075: " jnz near "
 dd _@two_spec          ; 076: " jbe near "
 dd _@two_spec          ; 077: " ja near "
 dd _@two_spec          ; 078: " js near "
 dd _@two_spec          ; 079: " jns near "
 dd _@two_spec          ; 07A: " jpe near "
 dd _@two_spec          ; 07B: " jpo near "
 dd _@two_spec          ; 07C: " jl near "
 dd _@two_spec          ; 07D: " jge near "
 dd _@two_spec          ; 07E: " jle near "
 dd _@two_spec          ; 07F: " jg near "
 dd @080_disasm         ; 080: " xxx byte ptr [...],byte "
 dd @081_disasm         ; 081: " xxx dword ptr [...],dword "
 dd 000                 ; 082
 dd @083_disasm         ; 083: " xxx dword ptr [...],byte "
 dd _@instr_2ereg       ; 084: " test rega,regb " (8 bit)
 dd _@instr_2ereg       ; 085: " test erega,eregb " (32 bit)
 dd _@instr_2ereg       ; 086: " xchg rega,regb " (8 bit)
 dd _@instr_2ereg       ; 087: " xchg erega,eregb " (32 bit)
 dd @088_disasm         ; 088: " mov byte ptr [...],reg "
 dd @089_disasm         ; 089: " mov dword ptr [...],ereg "
 dd @08A_disasm         ; 08A: " mov reg,byte ptr [...] "
 dd @08B_disasm         ; 08B: " mov ereg,dword ptr [...] "
 dd 000                 ; 08C
 dd @08D_disasm         ; 08D: " lea ereg,dword ptr [...] "
 dd 000                 ; 08E
 dd @08F_disasm         ; 08F: " pop dword ptr [...] "
 dd @one_byte_junk      ; 090: " nop "
 dd _@one_byte          ; 091: " xchg eax,ecx "
 dd _@one_byte          ; 092: " xchg eax,edx "
 dd _@one_byte          ; 093: " xchg eax,ebx "
 dd _@one_byte          ; 094: " xchg eax,esp "
 dd _@one_byte          ; 095: " xchg eax,ebp "
 dd _@one_byte          ; 096: " xchg eax,esi "
 dd _@one_byte          ; 097: " xchg eax,edi "
 dd _@one_byte          ; 098: " cwde "
 dd _@one_byte          ; 099: " cdq "
 dd 000                 ; 09A
 dd _@one_byte          ; 09B: " wait "
 dd _@one_byte          ; 09C: " pushfd "
 dd _@one_byte          ; 09D: " popfd "
 dd _@one_byte          ; 09E: " sahf "
 dd _@one_byte          ; 09F: " lahf "
 dd @0AX_disasm         ; 0A0: " mov al, byte ptr [DWORD] "
 dd @0AX_disasm         ; 0A1: " mov eax, dword ptr [DWORD] "
 dd @0AX_disasm         ; 0A2: " mov byte ptr [DWORD], al "
 dd @0AX_disasm         ; 0A3: " mov dword ptr [DWORD], eax "
 dd _@one_spec          ; 0A4: " movsb "
 dd _@one_spec          ; 0A5: " movsd "
 dd _@one_spec          ; 0A6: " cmpsb "
 dd _@one_spec          ; 0A7: " cmpsd "
 dd _@two_bytes         ; 0A8: " test al, byte "
 dd _@five_bytes        ; 0A9: " test eax, dword "
 dd _@one_spec          ; 0AA: " stosb "
 dd _@one_spec          ; 0AB: " stosd "
 dd _@one_spec          ; 0AC: " lodsb "
 dd _@one_spec          ; 0AD: " lodsd "
 dd _@one_spec          ; 0AE: " scasb "
 dd _@one_spec          ; 0AF: " scasd "
 dd _@two_bytes         ; 0B0: " mov al, byte "
 dd _@two_bytes         ; 0B1: " mov cl, byte "
 dd _@two_bytes         ; 0B2: " mov dl, byte "
 dd _@two_bytes         ; 0B3: " mov bl, byte "
 dd _@two_bytes         ; 0B4: " mov ah, byte "
 dd _@two_bytes         ; 0B5: " mov ch, byte "
 dd _@two_bytes         ; 0B6: " mov dh, byte "
 dd _@two_bytes         ; 0B7: " mov bh, byte "
 dd _@five_bytes        ; 0B8: " mov eax,dword "
 dd _@five_bytes        ; 0B9: " mov ecx,dword "
 dd _@five_bytes        ; 0BA: " mov edx,dword "
 dd _@five_bytes        ; 0BB: " mov ebx,dword "
 dd _@five_bytes        ; 0BC: " mov esp,dword "
 dd _@five_bytes        ; 0BD: " mov ebp,dword "
 dd _@five_bytes        ; 0BE: " mov esi,dword "
 dd _@five_bytes        ; 0BF: " mov edi,dword "
 dd @0C0_disasm         ; 0C0:
 dd @0C1_disasm         ; 0C1:
 dd 000
 dd _@one_spec          ; 0C3: " ret "
 dd 000
 dd 000
 dd @0C6_disasm         ; 0C6: " mov byte ptr [...],reg "
 dd @0C7_disasm         ; 0C7: " mov dword ptr [...],ereg "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd @0D1_disasm         ; 0D1: COproc
 dd 000
 dd @0D3_disasm         ; 0D3:
 dd 000
 dd 000
 dd 000
 dd 000
 dd @0D8_disasm
 dd @0D9_disasm
 dd @0DA_disasm         ; 0DA: COproc instr
 dd @0DB_disasm         ; 0DB: COproc instr
 dd @0DC_disasm
 dd @0DD_disasm
 dd @0DE_disasm
 dd 000
 dd 000
 dd 000
 dd _@two_spec          ; 0E2: " loop byte "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd _@cj_dword          ; 0E8: " call FAR dword "
 dd _@cj_dword          ; 0E9: " jmp FAR dword "
 dd 000                 ; 0EA
 dd _@two_spec          ; 0EB: " jmp NEAR "
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd 000
 dd _@one_byte          ; 0F5: " cmc "
 dd @0F6_disasm         ; 0F6: " xxx byte ptr [...] "
 dd @0F7_disasm
 dd _@one_byte          ; 0F8: " clc "
 dd _@one_byte          ; 0F9: " stc "
 dd _@one_byte          ; 0FA: " cli "
 dd @one_byte_junk      ; 0FB: " sti "
 dd _@one_byte          ; 0FC: " cld "
 dd _@one_byte          ; 0FD: " std "
 dd @0FE_disasm         ; 0FE: " xxx byte ptr [...] "
 dd @0FF_disasm         ; 0FF: " xxx dword ptr [...] "
;
 disasm_table_of_procs_end: