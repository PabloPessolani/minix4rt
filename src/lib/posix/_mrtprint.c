/*===========================================================================*
 *				mrt_print				     				*
 * Print a string in the system console							*
 * Input:												* 	
 * 			char string[];		; string to print in console		*
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_print _mrt_print
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_print(string)
char *string;
{
	return(_rtkrncall(MRTPRINT,(void *)string));
}
