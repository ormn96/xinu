tcc -I.. -DTURBOC -c *.c
tlib myXINU.lib -+ CREATE.obj -+ INITIALI.obj -+ INSERT.obj -+ CLKINT.obj
tlib myXINU.lib -+ QUEUE.obj -+ RESCHED.obj -+ CHPRIO.obj + tar3prio.obj
tcc -I.. ex9.c myxinu.lib