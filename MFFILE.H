/* mffile.h */

/* MS-DOS file structures used for accessing MS-DOS files in Xinu	*/

#define	EOF		-2		/* value returned on end-of-file*/
#define	FLREAD		001		/* mf_mode bit for "read"	*/
#define	FLWRITE		002		/* mf_mode bit for "write"	*/
#define	FLRW		003		/* mf_mode bits for read+write	*/
#define	FLNEW		010		/* mf_mode bit for "new file"	*/
#define	FLOLD		020		/* mf_mode bit for "old file"	*/
#define FLBINARY	040		/* mf_mode bit for "binary file"*/

#define DBUFSIZ		512		/* size of data buffers		*/
#define DBMASK		(~(DBUFSIZ-1))	/* mask for stripping low bits	*/

struct	mfblk	{			/* file "device" control block	*/
	int	mf_id;			/* file's "device id" in devtab	*/
	int	mf_dev;			/* file is on this master device*/
	int	mf_pid;			/* process id accessing the file*/
	int	mf_handle;		/* MS-DOS file handle		*/
	int	mf_mode;		/* FLREAD, FLWRITE, etc.	*/
	long	mf_size;		/* file size in bytes		*/
	long	mf_pos;			/* current file position (bytes)*/
	long	mf_bpos;		/* position of first buffer byte*/
	Bool	mf_dch;			/* has mf_buff been changed?	*/
	char	*mf_bptr;		/* ptr to next char in mf_buff	*/
	char	mf_buff[DBUFSIZ];	/* current data block for file	*/
};

#ifdef	Nmf
extern	struct	mfblk	mftab[];
#endif

extern	int	dos_creat();
extern	int	dos_open();
extern	int	dos_close();
extern	int	dos_read();
extern	int	dos_write();
extern	long	dos_lseek();
