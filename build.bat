@echo off

set BASE_PATH="%~dp0"
cd %BASE_PATH%

if not exist bin mkdir bin

pushd bin

set debug=/Zi
set release=/O2 /Zi
set mode=%debug%
if "%1" == "release" (set mode=%release%)

set complierflags=/D_CRT_SECURE_NO_WARNINGS -diagnostics:column -WL
set complierflags=%complierflags% -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -wd4201 -wd4100 
set complierflags=%complierflags% -wd4189 -wd4505 -wd4127 -wd4204 -wd4221 -FC -GS- -Gs9999999 %mode%
set linkflags=-incremental:no -opt:ref "kernel32.lib" "user32.lib" "gdi32.lib" "opengl32.lib"

call cl %complierflags% -Feepsilon "..\src\win32.c" /link %linkflags%

del *.obj

popd