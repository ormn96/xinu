echo off
if x%1 == x goto bad
if not exist %1 goto there
echo Can't use extension with file name
goto bad
:there
if exist %1.asm goto ok
echo File %1.asm does not exist
goto bad
:ok
masm %1.asm /mx;
if errorlevel 1 goto done
lib ..\lib\xinu.lib-+%1.obj;
goto done
:bad
echo usage: masmone file
:done
rem end of batch file
