@echo off
tasm32 -ml -m5 xpl.asm
if not exist==xpl.obj goto end
tlink32 -Tpe -c -x xpl.obj ,,, import32 ,, icon.res
del *.obj
:end
