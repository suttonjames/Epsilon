@echo off

set BASE_PATH="%~dp0"
cd %BASE_PATH%

if not exist bin mkdir bin

pushd bin

del *.exe
del *.pdb
del *.dll

set debug=/Zi
set release=/O2 /Zi
set mode=%debug%
if "%1" == "release" (set mode=%release%)

set complierflags=/I..\deps\ /D_CRT_SECURE_NO_WARNINGS -diagnostics:column -WL
set complierflags=%complierflags% -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 
set complierflags=%complierflags% -wd4189 -wd4505 -wd4127 -wd4204 -wd4221 -FC -GS- -Gs9999999 %mode%
set linkflags=-incremental:no -opt:ref "kernel32.lib" "user32.lib" "gdi32.lib" "opengl32.lib"

call cl %complierflags% -Feepsilon "..\src\epsilon.c" -LD /link %linkflags% -PDB:epsilon_%random%.pdb -EXPORT:init_game -EXPORT:update_game -EXPORT:shutdown_game
call cl %complierflags% -Feepsilon "..\src\win32.c" /link %linkflags%

del *.obj
del *.exp


popd