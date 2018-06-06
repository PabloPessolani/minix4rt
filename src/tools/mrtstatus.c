/* mrtstatus - print MINIX2RT status	Author: Pablo Pessolani */

/* Ps.c, Pablo Pessolani (ppessolani@hotmail.com), December 2005.
 *
 *
 * VERY IMPORTANT NOTE:
 *	To compile mrtstatus, the kernel/, fs/ and mm/ source directories must be in
 *	../ relative to the directory where mrtstatus is compiled (normally the
 *	tools source directory).
 *
 *	If you want your mrtstatus to be useable by anyone, you can arrange the
 *	following access permissions (note the protected memory files and set
 *	*group* id on mrtstatus):
 *	-rwxr-sr-x  1 bin   kmem       11916 Jul  4 15:31 /bin/mrtstatus
 *	crw-r-----  1 bin   kmem      1,   1 Jan  1  1970 /dev/mem
 *	crw-r-----  1 bin   kmem      1,   2 Jan  1  1970 /dev/kmem
 */

#include <minix/config.h>
#include <limits.h>
#include <sys/types.h>

#include <minix/const.h>
#include <minix/type.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <minix/com.h>
#include <fcntl.h>
#include <a.out.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdio.h>
#include <ttyent.h>

#include "../kernel/const.h"
#include "../kernel/type.h"
#include "../kernel/proc.h"
#undef printf			/* kernel's const.h defined this */

/* Number of tasks and processes. */
int nr_tasks;
int nr_procs;
int nr_vtimers; 
int nr_prty;	
int nr_irq_vectors;
int nr_irq_soft;
int nr_msgq;
int nr_messages;
int nr_sem;


/* pointers to kernel data structures */  
MRT_sysirq_t *psi;		/* pointer to system irq data 	*/ 
MRT_sysval_t *psv;		/* pointer to system values data 	*/ 
MRT_systmr_t *pst;		/* pointer to system timer  data 	*/ 
MRT_sysmsg_t *psm;		/* pointer to system message data 	*/ 
MRT_syssem_t *pss;		/* pointer to system semaphore data */ 

#define	KMEM_PATH	"/dev/kmem"	/* opened for kernel proc table */
#define	MEM_PATH	"/dev/mem"	/* opened for mm/fs + user processes */

int kmemfd, memfd;		/* file descriptors of [k]mem */

_PROTOTYPE(int main, (int argc, char *argv []));
_PROTOTYPE(int addrread, (int fd, phys_clicks base, vir_bytes addr, 
				    char *buf, int nbytes ));
_PROTOTYPE(void usage, (char *pname ));
_PROTOTYPE(void err, (char *s ));
_PROTOTYPE(void disaster, (int sig ));
_PROTOTYPE(void int2bin,(int number, char *bin));

/* If disaster is called some of the system parameters imported into mrtstatus are
 * probably wrong.  This tends to result in memory faults.
 */
void disaster(sig)
int sig;
{
  fprintf(stderr, "Ooops, got signal %d\n", sig);
  fprintf(stderr, "Was mrtstatus recompiled since the last kernel change?\n");
  exit(3);
}


/********************************************************************************
 * Main interprets arguments, gets system addresses, opens [k]mem, reads in
 * process tables from kernel and calls mrtstat() for relevant entries.
 ********************************************************************************/
int main(argc, argv)
int argc;
char *argv[];
{
  int i;
  char *opt;
  char bm_bin[17];
  MRT_irqd_t *d;

  struct mrtinfo mrtinfo;
  int opt_iattr  = FALSE;	/* -i */
  int opt_sstat  = FALSE;	/* -s */
  int opt_vtstat = FALSE;	/* -t */
  int opt_mstat  = FALSE;	/* -m */
  int opt_const  = FALSE;	/* -c */
  int opt_irqQ   = FALSE;	/* -I */
  int opt_vtQ    = FALSE;	/* -T */
  int opt_msgQ   = FALSE;	/* -M */
  int opt_sem    = FALSE;	/* -S */

  (void) signal(SIGSEGV, disaster);	/* catch a common crash */

  /* Parse arguments; a '-' need not be present (V7/BSD compatability) */
  if(argc > 2) 
	{
	usage(argv[0]);
	exit(1);
	}
  else if (argc == 1)
	{
	opt_sstat = TRUE;		/* -s */
	}
  else 
	{
	opt = argv[1];
	if (*opt == '-') opt++;
	switch (*opt) 
		{
		case 'i':	
			opt_iattr = TRUE;
			break;
		case 's':	
			opt_sstat = TRUE;
			break;
		case 't':	
			opt_vtstat = TRUE;
			break;
		case 'm':	
			opt_mstat = TRUE;
			break;
		case 'c':	
			opt_const = TRUE;
			break;
		case 'I':	
			opt_irqQ = TRUE;
			break;
		case 'T':	
			opt_vtQ = TRUE;
			break;
		case 'M':	
			opt_msgQ = TRUE;
			break;
		case 'S':	
			opt_sem = TRUE;
			break;
		case 'h':	
			fprintf(stderr, "Usage: %s [-][i|s|t|m|c|I|T|M]\n", argv[0]);
			printf("\ti = Interrupt descriptors status and statistics\n"); 
			printf("\ts = System-wide status and statistics\n");
			printf("\tt = Virtual Timer status and statistics\n");
			printf("\tm = Message status and statistics\n");
			printf("\tc = System Constants\n");
			printf("\tI = Interrupt Queues status and statistics\n"); 
			printf("\tT = Virtual Timer queues status and statistics\n");
			printf("\tM = Message Queues status and statistics\n");
			printf("\tM = RT-Semaphores status and statistics\n");
			break;
		default:	
			usage(argv[0]);
		}	
	}
  

  /* Open memory devices and get MRT info from the kernel */
  if ((kmemfd = open(KMEM_PATH, O_RDONLY)) == -1) err(KMEM_PATH);
  if ((memfd = open(MEM_PATH, O_RDONLY)) == -1) err(MEM_PATH);
 
  if (ioctl(memfd, MIOCGMRTINFO, (void *) &mrtinfo) == -1)
	err("can't get MRT info from kernel");

  nr_vtimers 	= mrtinfo.nr_vtimers; 
  nr_prty 		= mrtinfo.nr_prty;	
  nr_irq_vectors 	= mrtinfo.nr_irq_vectors;
  nr_irq_soft 	= mrtinfo.nr_irq_soft;
  nr_msgq 		= mrtinfo.nr_msgq;
  nr_messages	= mrtinfo.nr_messages;
  nr_sem		= mrtinfo.nr_sem;

  if( opt_const == TRUE )
	{
	printf("NR_VTIMERS      = %d\n",nr_vtimers );
	printf("NR_PRTY         = %d\n",nr_prty);
	printf("NR_IRQ_VECTORS  = %d\n",nr_irq_vectors);
	printf("NR_IRQ_SOFT     = %d\n",nr_irq_soft);
	printf("NR_MSGQ         = %d\n",nr_msgq);
	printf("NR_MESSAGES     = %d\n",nr_messages);
	printf("NR_SEM          = %d\n",nr_sem);
	}
  else if( opt_sstat == TRUE )
	{
  	/* Allocate memory for MRT_sysval_t data structure */
  	psv = (MRT_sysval_t*) malloc(sizeof(MRT_sysval_t));
  	if (psv == NULL) err("Out of memory");

	/* Get kernel MRT_sysval_t data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_sv
		,(char *) psv, sizeof(MRT_sysval_t))
		!= sizeof(MRT_sysval_t))
			err("Can't get kernel System Values MRT_sysval_t from /dev/kmem");

	printf("flags           = %X\n",psv->flags);
	printf("virtual_PIC     = %X\n",psv->virtual_PIC);
	printf("PIT_latency     = %d\n",psv->PIT_latency);
	printf("PIT_latch       = %d\n",psv->PIT_latch);
	printf("tickrate        = %d\n",psv->tickrate);
	printf("harmonic        = %d\n",psv->harmonic);
	printf("refresh         = %d\n",psv->refresh);
	printf("scheds          = %d\n",psv->counter.scheds);
	printf("messages        = %d\n",psv->counter.messages);
	printf("interrupts      = %d\n",psv->counter.interrupts);
	printf("ticks           = %d\n",psv->counter.ticks);
	printf("highticks       = %d\n",psv->counter.highticks);
	printf("idlemax         = %d\n",psv->counter.idlemax);
	printf("idlelast        = %d\n",psv->counter.idlelast);
	}
  else if( opt_iattr == TRUE )
	{
  	/* Allocate memory for sysirq data structure */
  	psi = (MRT_sysirq_t *) malloc(sizeof(MRT_sysirq_t));
  	if (psi == NULL) err("Out of memory");

	/* Get kernel sysirq data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_si
		,(char *) psi,sizeof(MRT_sysirq_t))
		!= sizeof(MRT_sysirq_t))
			err("Can't get kernel system interrupt MRT_sysirq_t from /dev/kmem");

	printf("IRQ PER TSK WDOG PTY TYPE COUNT MSHWR MSDL TIMESTAMP MAXL REENT NAME\n");
	for (i = 0; i < (nr_irq_vectors+nr_irq_soft); i++) 
		{
		printf("%3d %3d %3d %4d %3X %4X %5d %5d %4d %9d %4d %5d %s\n",
		     	i,
			psi->irqtab[i].period,
  			psi->irqtab[i].task,
  			psi->irqtab[i].watchdog,
  			psi->irqtab[i].priority,
  			psi->irqtab[i].irqtype,

  			psi->irqtab[i].count,
  			psi->irqtab[i].maxshower,
  			psi->irqtab[i].mdl,
  			psi->irqtab[i].timestamp,
  			psi->irqtab[i].maxlat,
  			psi->irqtab[i].reenter,

  			psi->irqtab[i].name);
		}
	}
  else if( opt_vtstat == TRUE )
	{
  	/* Allocate memory for MRT_systmr_t data structure */
  	pst = (MRT_systmr_t *) malloc(sizeof(MRT_systmr_t));
  	if (pst == NULL) err("Out of memory");

	/* Get kernel MRT_systmr_t data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_st
		,(char *) pst,sizeof(MRT_systmr_t))
		!= sizeof(MRT_systmr_t))
			err("Can't get kernel system timer MRT_systmr_t from /dev/kmem");

	printf("-VT --PERIOD-- -NEXTEXP-- LIMIT ACT PAR FLAG NR OWN PRTY EXPIRE TIMESTAMP\n");
	for (i = 0; i < nr_vtimers; i++) 
		{
		printf("%3d %10d %10d %5d %3d %3d %4X %2d %3d %4d %6d %9d\n",
			i,
			pst->vtimer[i].period,		/* Period in ticks		*/
			pst->vtimer[i].nextexp,		/* ticks to next expiration	*/
	
			pst->vtimer[i].limit,		/* expirations to be removed  */
			pst->vtimer[i].action,		/* Action				*/
			pst->vtimer[i].param,		/* Action parameter		*/
			pst->vtimer[i].flags,		/* Action parameter		*/

			pst->vtimer[i].index,			/* VT number 			*/
			pst->vtimer[i].owner,			/* nbr of the owner process	*/
			pst->vtimer[i].priority,		/* VT	priority		 	*/

			pst->vtimer[i].expired,		/* number of vtimer expirations */
			pst->vtimer[i].timestamp		/* timestamp of the last expire */
			);
		}
	}
  else if( opt_mstat == TRUE )
	{
  	/* Allocate memory for MRT_sysmsg_t data structure */
  	psm = (MRT_sysmsg_t *) malloc(sizeof(MRT_sysmsg_t));
  	if (psm == NULL) err("Out of memory");

	/* Get kernel MRT_sysmsg_t data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_sm
		,(char *) psm,sizeof(MRT_sysmsg_t))
		!= sizeof(MRT_sysmsg_t))
			err("Can't get kernel system message MRT_sysmsg_t from /dev/kmem");

	printf("MSG SPID/SNBR DPID/DNBR TYPE -MID- SEQNO TIMESTAMP PRI DLINE LAXTY\n");
	for (i = 0; i < nr_messages; i++) 
		{
		printf("%3d %4d/%4d %4d/%4d %4d %5d %5d %9d %3X %5d %5d \n",
			i,
			psm->mqe[i].msgd.hdr.src.p_nr,
			psm->mqe[i].msgd.hdr.src.pid,
			psm->mqe[i].msgd.hdr.dst.p_nr,
			psm->mqe[i].msgd.hdr.dst.pid,
			psm->mqe[i].msgd.hdr.mtype,

			psm->mqe[i].msgd.hdr.mid,
			psm->mqe[i].msgd.hdr.seqno,

			psm->mqe[i].msgd.hdr.tstamp,

			psm->mqe[i].msgd.hdr.priority,
			psm->mqe[i].msgd.hdr.deadline,
			psm->mqe[i].msgd.hdr.laxity 
			);
		}
	}
  else if( opt_irqQ == TRUE )
	{
  	/* Allocate memory for sysirq data structure */
  	psi = (MRT_sysirq_t *) malloc(sizeof(MRT_sysirq_t));
  	if (psi == NULL) err("Out of memory");

	/* Get kernel sysirq data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_si
		,(char *) psi,sizeof(MRT_sysirq_t))
		!= sizeof(MRT_sysirq_t))
			err("Can't get kernel system interrupt MRT_sysirq_t from /dev/kmem");

	int2bin(psi->irqQ.bitmap,bm_bin);
	printf("\nInterrupt Pending Queues bitmap = %17s\n",bm_bin);

	printf("PRI INQ MAXINQ\n");
	for( i=0; i < nr_prty ; i++)
		{
		printf("%3d %3d %6d \n",
			i,
			psi->irqQ.inQ,
			psi->irqQ.maxinQ);
		}
	}
  else if( opt_vtQ == TRUE )
	{
  	/* Allocate memory for MRT_systmr_t data structure */
  	pst = (MRT_systmr_t *) malloc(sizeof(MRT_systmr_t));
  	if (pst == NULL) err("Out of memory");

	/* Get kernel MRT_systmr_t data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_st
		,(char *) pst,sizeof(MRT_systmr_t))
		!= sizeof(MRT_systmr_t))
			err("Can't get kernel system timer MRT_systmr_t from /dev/kmem");

	printf("\n\nTYPE PRTY INQ MAXINQ\n");

	/*  Active timer queue dump */
	printf("ACTV  ALL %3d %6d\n",pst->actQ.list.inQ,pst->actQ.list.maxinQ);

	/*  Expired timer queues dump */
	for( i = 0; i < nr_prty; i++)
		printf("EXPD %4d %3d %6d\n",
			i,pst->expQ.inQ,pst->expQ.maxinQ);

	/*  Free  timer queue dump */
	printf("FREE NONE %3d %6d\n",pst->freeQ.inQ,pst->freeQ.maxinQ);
	}
  else if( opt_msgQ == TRUE )
	{
  	/* Allocate memory for MRT_sysmsg_t data structure */
  	psm = (MRT_sysmsg_t *) malloc(sizeof(MRT_sysmsg_t));
  	if (psm == NULL) err("Out of memory");

	/* Get kernel MRT_sysmsg_t data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_sm
		,(char *) psm,sizeof(MRT_sysmsg_t))
		!= sizeof(MRT_sysmsg_t))
			err("Can't get kernel system message MRT_sysmsg_t from /dev/kmem");

	printf("ID SZ FLAG INQ MAX OWN --DLVD-- --ENQD--\n");
	for( i=0; i < nr_msgq; i++)
		{
		printf("%2d %2d %4X %3d %3d %3d %8d %8d\n",
			psm->msgQ[i].index,
			psm->msgQ[i].size,
			psm->msgQ[i].flags,
			psm->msgQ[i].plist.inQ,
			psm->msgQ[i].plist.maxinQ,
			psm->msgQ[i].owner,
			psm->msgQ[i].delivered,
			psm->msgQ[i].enqueued);
		}
	}
  else if( opt_sem == TRUE )
	{
  	/* Allocate memory for MRT_syssem_t data structure */
  	pss = (MRT_syssem_t *) malloc(sizeof(MRT_syssem_t));
  	if (pss == NULL) err("Out of memory");

	/* Get kernel MRT_syssem_t data structure */
	if (addrread(kmemfd, (phys_clicks) 0,mrtinfo.mrt_ss
		,(char *) pss,sizeof(MRT_syssem_t))
		!= sizeof(MRT_syssem_t))
			err("Can't get kernel system semaphore MRT_syssem_t from /dev/kmem");

	printf("SEM --VALUE- PTY FLAG OWN --#UPS-- -#DOWNS- INQ- MAXQ NAME \n");
	for( i=0; i < nr_sem; i++)
		{
		printf("%3d %8d %3d %4X %3d %8d %8d %4d %4d %s\n",
			i,
			pss->sem[i].value,
			pss->sem[i].priority,
			pss->sem[i].flags,
			pss->sem[i].owner,
			pss->sem[i].ups,
			pss->sem[i].downs,
			pss->sem[i].plist.inQ,
			pss->sem[i].plist.maxinQ,
			pss->sem[i].name);
		}
	}
  return(0);
}



/* Addrread reads nbytes from offset addr to click base of fd into buf. */
int addrread(fd, base, addr, buf, nbytes)
int fd;
phys_clicks base;
vir_bytes addr;
char *buf;
int nbytes;
{
  if (lseek(fd, ((off_t) base << CLICK_SHIFT) + addr, 0) < 0)
	return -1;

  return read(fd, buf, nbytes);
}

void usage(pname)
char *pname;
{
  fprintf(stderr, "Usage: %s [-][i|s|t|m|c|I|T|M|S]\n", pname);
  exit(1);
}

void err(s)
char *s;
{
  extern int errno;

  if (errno == 0)
	fprintf(stderr, "mrtstatus: %s\n", s);
  else
	fprintf(stderr, "mrtstatus: %s: %s\n", s, strerror(errno));

  exit(2);
}

/*===========================================================================*
 *				int2bin 			   				     *
 *===========================================================================*/
void int2bin( number, bin)
int number;
char *bin;
{
	int sreg;

	for( sreg = 0x8000; sreg != 0 ; sreg = sreg >> 1, bin++)
		*bin = (number & sreg)?'1':'0';

	*bin = '\0';
}

