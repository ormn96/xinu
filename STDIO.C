/* stdio.c - gets, scanf */

/* These routines emulete standerd routines gets, scanf, puts
   under XINU.  They are NOT part of the sandard XINU, added later
   to prevent problems */

#include <conf.h>
#include <stdarg.h>
#define getchar()       getc(CONSOLE)
#define	putchar(ch)	putc(CONSOLE,(ch))


/*------------------------------------------------------------------------
 *  gets  --  read until end-of-line into string
 *------------------------------------------------------------------------
 */
char  *gets(char *s)
{
   int i;

    i = 0;
    s[0] = '\0'; 
    while( (s[i] = getc(CONSOLE)) != '\n')
     {
        i++;
        s[i] = '\0'; 
     }
    return s;
} /* gets */

/*------------------------------------------------------------------------
 *  scanf  --  formatted keyboard input
 *  Is different from standard scanf - always reads till end of line
 *------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------
 *  puts  --  Write string to console + End-Of-Line
 *------------------------------------------------------------------------
 */

 int scanf (const char *format,... )
 {
   char tempstr[200];
   va_list arglist;

   va_start(arglist, format);

   gets(tempstr);
   /* Let TURBO C vsscanf do the hard part, source has to be compiled
      with tcc */
          /* sscanf answer is passed on */
   return (vsscanf(tempstr, format, arglist));

 } /* scanf */



char *puts(char *s)
{
    int i, c;
    i = 0;

    c = s[i++];
    while( c  != '\0')
    {
         putc(CONSOLE, c);
         c = s[i++];
    }
    putc(CONSOLE, '\n');
    return s;
} /* gets */

