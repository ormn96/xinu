/* teststd.c - xmain for testing stdio.c */

#include <conf.h>
#include <kernel.h>
#include "stdio.h"

void second()
{
  int x, y, z;

  puts("Enter x, y, z");
  scanf("%d %d %d", &x, &y, &z);
  printf("%d * %d * %d = %d\n", x, y, z, x*y*z);

} /* second */

xmain()
{
  int x, y, z;

  resume(create(second,INITSTK,INITPRIO,"cons",0));

  sleep(10);
  puts("Enter x, y, z");
  scanf("%d %d %d", &x, &y, &z);
  printf("%d +%d + %d = %d\n", x, y, z, x+y+z);
}
