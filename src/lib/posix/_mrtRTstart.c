/*===========================================================================*
 *				mrt_RTstart				     				*
 * Input:												* 	
 * 		int harmonic;	MRT_sv.tickrate = MRT_sv.harmonic * HZ 		*
 *   					0<  harmonic <= MAXHARMONIC  				*
 * 		int refresh;      Idle counter refresh count in ticks			*
 *   					0<  refresh <= MAXREFRESH  				*
 * Output:												*
 *		returns 										*
 *			OK (0) 			: if all it's OK				*
 *			E_MRT_BADHARM  (-2005)	: for an invalid harmonic value	*
 *			E_MRT_BADRFSH (-2023)	: for an invalid refresh value	*
 *			E_MRT_RTACTIVE(-2006)	: Some Real-Time process or		* 
 *							interrupt handler is active.		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_RTstart _mrt_RTstart
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_RTstart( harmonic, refresh )
int harmonic;
int refresh;
{
  message m;

	m.m1_i2  = harmonic;
	m.m1_i3  = refresh;
	m.m1_i1  = MRT_RTSTART;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
