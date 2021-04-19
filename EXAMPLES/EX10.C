/* ex10.c - xmain, demonstrate printf of long int, float */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of formatting float, long int in PC-Xinu
 *------------------------------------------------------------------------
 */


void xmain()
{
    long int li;

    char str[20];

    li = 10000000;

    sprintf(str, "li = %ld\n", li);
    printf(str); 

} /* xmain */

