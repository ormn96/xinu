/* twest_slp.c - xmain */

#include <conf.h>

/*------------------------------------------------------------------------
 *  xmain  --  test new sleep
 *------------------------------------------------------------------------
 */

extern long int tod;

xmain()
{
  char str[80];

  sprintf(str, "Before sleep, tod = %ld\n", tod);
  printf(str);
  sleep(1200);
  sprintf(str, "After sleep, tod = %ld\n", tod);
  printf(str);


}
