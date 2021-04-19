echo off
rem batch file for remaking the PC-Xinu library
rem using the Turbo C compiler
rem   object modules will be created in this directory
rem   xinu.lib library will be created in the ..\lib directory
if exist turboc goto ok
echo Deleting obj files (is this what you really want?)
pause
del *.obj
echo TURBOC TAG FILE -- DO NOT DELETE > turboc
:ok
make tccmake.dep
lib proto @xlib.fil ,, ..\lib\xinu.lib;
