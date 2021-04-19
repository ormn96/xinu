/* sleep.h */

extern	int	clockq;		/* q index of sleeping process list	*/
extern	int	*sltop;		/* address of first key on clockq	*/
extern	int	slnempty;	/* 1 iff clockq is nonempty		*/
extern	long	tod;		/* time of day (ticks since startup)	*/

extern	int	defclk;		/* >0 iff clock interrupts are deferred	*/
extern	int	clkdiff;	/* number of clock ticks deferred	*/

#define TICSN	19663		/* no. of ticks per 1080 seconds	*/
#define TICSD	1080
