tcc -I.. -DTURBOC -c *.c
tlib myXINU.lib -+ FREEMEM.obj -+ INITIALI.obj -+ GETMEM.obj -+ CLKINT.obj -+ KILL.obj 
tlib myXINU.lib -+ freep.obj -+ insertdm.obj -+ memq.obj -+ memslp.obj
tlib myXINU.lib -+ memwake.obj -+XFORK.obj -+ xforkSF.obj -+ xwait.obj
tcc -I.. test.c myxinu.lib