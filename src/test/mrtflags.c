/******************************************************************************/
/* 					mrtflags.c							*/
/******************************************************************************/

#include <minix/config.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <lib.h>
#include <minix/syslib.h>


_PROTOTYPE(int main, (int argc, char *argv []));

int main(argc, argv)
int argc;
char *argv[];
{
  mrt_sysval_t val;
  int rcode,i;
  char *opt;
  int flags = 0;

  /* Parse arguments; a '-' need not be present (V7/BSD compatability) */
   
  if (argc >= 2)
	{
	for (i = 1; i < argc; i++) 
		{
		opt = argv[i];
		if (opt[0] == '-') opt++;
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
					fprintf(stderr, "Usage: %s [-][dlimo]\n", argv[0]);
					printf("\td = Enables the usage of debug232() function\n"); 
					printf("\tl = Enables latency computation\n");
					printf("\ti = Enables the use of the Basic Priority Inheritance 						Protocol\n");
					printf("\tm = Enables the use of Priority Interrupt Masks\n");
					printf("\to = Sets the system to program the PIT in ONE SHOT mode\n");
					return(1);
					break;
				}
			opt++;
			}
		}	
	}

  rcode = mrt_setsval(flags);
  printf("mrt_setsval: rcode=%d.\n\n", rcode);


	rcode = mrt_getsval(&val);
	printf("mrt_getsval: rcode=%d.\n\n", rcode);
	if( rcode != 0) exit(1);

	printf("flags = x%-X\n",val.flags);
  return(0);
}

