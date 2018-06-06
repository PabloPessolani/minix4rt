/******************************************************************************/
/* 					mrtstart.c							*/
/* Starts the Real Time mode using mrt_RTstart() System Call			*/
/* Usage:												*/
/* 	mrtstart [-dlimo] [harmonic [refresh]]						*/
/* where:												*/
/* 	d,l,i,m,o: There are system processing mode flags (see usage())		*/
/* 	harmonic: The Harmonic Number of the MINIX timer frequency (HZ = 50 Hz).*/
/*	refresh: The idle refresh counter in timer ticks.				*/
/******************************************************************************/


#include <minix/config.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <minix/type.h>
#include <unistd.h>
#include <stdio.h>
 
#include <lib.h>
#include <minix/syslib.h>
#include <minix/const.h>

_PROTOTYPE(int main, (int argc, char *argv []));
_PROTOTYPE(void usage, (void));

int main(argc, argv)
int argc;
char *argv[];
{
 	mrt_sysval_t val;
	char *opt;
	int rcode, harmonic, refresh;
	int flags, nflg, nop;
	
	flags 	= 0;
	nflg  	= 0;
	harmonic 	= 4;
	refresh 	= 4 * HZ;

  /* Parse arguments; a '-' need not be present (V7/BSD compatability) */
   
	opt = argv[1];
	if (opt[0] == '-')
		{
		nflg = 1;
		opt++;
		while (*opt != 0) 
			{
			switch (*opt) 
				{
				case 'd':	
					flags |= MRT_DBG232;
					break;
				case 'l':	
					flags |= MRT_LATENCY;
					break;
				case 'i':	
					flags |= MRT_BPIP;
					break;
				case 'o':	
						flags |= MRT_ONESHOT;
					break;
				case 'm':	
					flags |= MRT_PRTYMASK;
						break;
				default:	
					usage();
					exit(1);
				break;
				}
			opt++	;
			}
		/* printf("flags=x%-X\n",flags); */
		}	
	
	nop = argc - nflg;
	switch(nop)
		{
		case 1:
			break;
		case 2:
			harmonic = atoi(argv[nop-1+nflg]);
			/* printf("harmonic=%d\n",harmonic); */
			break;
		case 3:
			harmonic = atoi(argv[nop-2+nflg]);
			refresh = atoi(argv[nop-1+nflg]);
			/* printf("harmonic=%d\nrefresh=%d\n",harmonic, refresh); */
			break;
		default:
			usage();	
			exit(1);
		}

	if(nflg == 1)
		{
		rcode = mrt_setsval(flags);
  		rcode = mrt_getsval(&val);
		if( rcode != 0) exit(1);
		}

	rcode = mrt_RTstart(harmonic,refresh);
	if( rcode != 0)
		{
		printf("mrt_RTstart: rcode=%5d.\n", rcode);
		exit(rcode);
		}
	else
		{
		printf("System is in Realtime Mode.\n");
		printf("Flags=x%-X\n",val.flags);
		printf("Harmonic=%d \(%d \[ticks\/s\]\)\n",harmonic,harmonic*HZ);
		printf("Refresh=%d \[ticks\]\n",refresh);
		}

	exit(0);
}

void usage(void)
{
	fprintf(stderr, "Usage: mrtstart [-dlimo] [harmonic [refresh]] \n");
	printf("Starts Real Time Processing Mode\n");
	printf("\td = Enables the usage of debug232() function\n"); 
	printf("\tl = Enables latency computation\n");
	printf("\ti = Enables the use of the Basic Priority Inheritance Protocol\n");
	printf("\tm = Enables the use of Priority Interrupt Masks\n");
	printf("\to = Sets the system to program the PIT in ONE SHOT mode\n");
	printf("harmonic: The Harmonic Number of the MINIX timer frequency (HZ=50Hz)\n");
	printf("refresh: The Idle Refresh counter in timer ticks.\n\n");
}
