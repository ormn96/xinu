echo off
rem batch file to recompile one object module with Microsoft MSC
if x%1 == x goto bad
if not exist %1 goto t1
echo Can't use extension with file name
goto bad
:t1
if not exist turboc goto t2
echo Object modules not compatible with Microsoft MSC
echo Must use mscmake first
goto done
:t2
if exist %1.c goto ok
echo File %1.c does not exist
goto bad
:ok
msc %1.c /Gs /FPa /I. ;
if errorlevel 1 goto done
lib ..\lib\xinu.lib-+%1.obj;
goto done
:bad
echo usage: mscone file
:done
rem end of batch file
