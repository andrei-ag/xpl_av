;
; Текстовое представление всего перечня инструкций, разбираемых дизассемблером
;
 _add                         db 'add',0
 _adc                         db 'adc',0
 _and                         db 'and',0
 _call                        db 'call',0
 _clc                         db 'clc',0
 _cld                         db 'cld',0
 _cli                         db 'cli',0
 _cdq                         db 'cdq',0
 _cmc                         db 'cmc',0
 _cmp                         db 'cmp',0
 _cwde                        db 'cwde',0
 _dec                         db 'dec',0
 _div                         db 'div',0
 _idiv                        db 'idiv',0
 _imul                        db 'imul',0
 _inc                         db 'inc',0
 _jmp                         db 'jmp',0
 _jne_                        db 'jne',0
 _jo                          db 'jo',0
 _jno                         db 'jno',0
 _jb                          db 'jb',0
 _jnb                         db 'jnb',0
 _je                          db 'je',0
 _jne                         db 'jne',0
 _jbe                         db 'jbe',0
 _ja                          db 'ja',0
 _js                          db 'js',0
 _jns                         db 'jns',0
 _jp                          db 'jp',0
 _jnp                         db 'jnp',0
 _jl                          db 'jl',0
 _jnl                         db 'jnl',0
 _jle                         db 'jle',0
 _jg                          db 'jg',0
 _lahf                        db 'lahf',0
 _lea                         db 'lea',0
 _lodsb                       db 'lodsb',0
 _lodsd                       db 'lodsd',0
 _loop                        db 'loop',0
 _mov                         db 'mov',0
 _movsx                       db 'movsx',0
 _movzx                       db 'movzx',0
 _mul                         db 'mul',0
 _neg                         db 'neg',0
 _nop                         db 'nop',0
 _not                         db 'not',0
 _or                          db 'or',0
 _push                        db 'push',0
 _pop                         db 'pop',0
 _popa                        db 'popa',0
 _popad                       db 'popad',0
 _popfd                       db 'popfd',0
 _pusha                       db 'pusha ',0
 _pushad                      db 'pushad',0
 _pushfd                      db 'pushfd',0
 _rcl                         db 'rcl',0
 _rcr                         db 'rcr',0
 _ret_                        db 'ret',0
 _rol                         db 'rol',0
 _ror                         db 'ror',0
 _sal                         db 'sal',0
 _sahf                        db 'sahf',0
 _sar                         db 'sar',0
 _sbb                         db 'sbb',0
 _shl                         db 'shl',0
 _shr                         db 'shr',0
 _sub                         db 'sub',0
 _stc                         db 'stc',0
 _std                         db 'std',0
 _sti                         db 'sti',0
 _stosb                       db 'stosb',0
 _stosd                       db 'stosd',0
 _test                        db 'test',0
 _xor                         db 'xor',0
 _wait                        db 'wait',0
 _xchg                        db 'xchg',0
; 
; Сопроцессор:
;
 fcom_st1                     db 'fcom st(1)',0
 fcompp_st1                   db 'fcompp st(1)',0
 fchs                         db 'fchs',0
 fabs                         db 'fabs',0
 f2xmi                        db 'f2xmi',0
 faddp_st1_st                 db 'faddp st(1),st',0
 fcompp                       db 'fcompp',0
 finit                        db 'finit',0
; 
; Segments
;
 _es                          db 'es:',0
 _cs                          db 'cs:',0
 _ss                          db 'ss:',0
 _ds                          db 'ds:',0
 _fs                          db 'fs:',0
 _gs                          db 'gs:',0
;
; Registers
;
 _regs_8bit          db 'al',0,'cl',0,'dl',0,'bl',0,'ah',0,'ch',0,'dh',0,'bh',0
 _regs_16bit         db 'ax',0,'cx',0,'dx',0,'bx',0,'sp',0,'bp',0,'si',0,'di',0
 _regs_32bit         db 'eax',0,'ecx',0,'edx',0,'ebx',0,'esp',0,'ebp',0,'esi',0,'edi',0
;
; Size
; 
 _byte               db 'byte ptr',0
 _word               db 'word ptr',0
 _dword              db 'dword ptr',0
;
 unknown_instruction db 'unknown',0