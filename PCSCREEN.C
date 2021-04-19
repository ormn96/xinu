/* pcscreen.c - putcsrpos, scrollup, putchr */

#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <window.h>
#include <vidio.h>

#define INT10H(r)	vidint(&(r))

#define AH(r)		((r).h.ah)
#define AL(r)		((r).h.al)
#define BH(r)		((r).h.bh)
#define BL(r)		((r).h.bl)
#define CH(r)		((r).h.ch)
#define CL(r)		((r).h.cl)
#define DH(r)		((r).h.dh)
#define DL(r)		((r).h.dl)
#define AX(r)		((r).x.ax)
#define BX(r)		((r).x.bx)
#define CX(r)		((r).x.cx)
#define DX(r)		((r).x.dx)
#define CF(r)		((r).x.cflag)

#define UNSIGNED(x)	( *( (unsigned int *) &(x) ) )	/* alias!	*/

#define SCSRPOS	2
#define SCRLUP	6
#define WCHR	10

/*------------------------------------------------------------------------
 *  putcsrpos  --  put the cursor at a given position
 *------------------------------------------------------------------------
 */
putcsrpos(csr,page)
CURSOR csr;
unsigned page;
{
	union REGS r;

	AH(r)=SCSRPOS;
	DX(r)=UNSIGNED(csr);
	BH(r)=page;
	INT10H(r);
}

/*------------------------------------------------------------------------
 *  scrollup  --  scroll a window up
 *------------------------------------------------------------------------
 */
scrollup(tl,br,lines,attr)
CURSOR tl,br;
unsigned lines;
unsigned char attr;
{
	union REGS r;

	AH(r)=SCRLUP;
	AL(r)=lines;
	CX(r)=UNSIGNED(tl);
	DX(r)=UNSIGNED(br);
	BH(r)=attr;
	INT10H(r);
}

/*------------------------------------------------------------------------
 *  putchr  --  put character at current cursor position
 *------------------------------------------------------------------------
 */
putchr(ch,count,page)
char ch;
unsigned count,page;
{
	union REGS r;

	AH(r)=WCHR;
	AL(r)=ch;
	CX(r)=count;
	BH(r)=page;
	INT10H(r);
}
