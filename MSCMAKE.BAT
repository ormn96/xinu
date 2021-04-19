echo off
rem batch file for remaking the PC-Xinu library
rem using the Microsoft MSC compiler
rem   object modules will be created in this directory
rem   xinu.lib library will be created in the ..\lib directory
if not exist turboc goto ok
echo Deleting obj files (is this what you really want?)
pause
del *.obj
del turboc
:ok
make mscmake.dep
lib proto @xlib.fil ,, ..\lib\xinu.lib;
