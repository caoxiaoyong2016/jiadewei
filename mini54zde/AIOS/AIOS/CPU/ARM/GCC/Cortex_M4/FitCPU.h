﻿/**********************************************************************************************************
AIOS(Advanced Input Output System) - An Embedded Real Time Operating System (RTOS)
Copyright (C) 2012~2017 SenseRate.Com All rights reserved.
http://www.aios.io -- Documentation, latest information, license and contact details.
http://www.SenseRate.com -- Commercial support, development, porting, licensing and training services.
--------------------------------------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met: 
1. Redistributions of source code must retain the above copyright notice, this list of 
conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other materials 
provided with the distribution. 
3. Neither the name of the copyright holder nor the names of its contributors may be used 
to endorse or promote products derived from this software without specific prior written 
permission. 
--------------------------------------------------------------------------------------------------------
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
--------------------------------------------------------------------------------------------------------
 Notice of Export Control Law 
--------------------------------------------------------------------------------------------------------
 SenseRate AIOS may be subject to applicable export control laws and regulations, which might 
 include those applicable to SenseRate AIOS of U.S. and the country in which you are located. 
 Import, export and usage of SenseRate AIOS in any manner by you shall be in compliance with such 
 applicable export control laws and regulations. 
***********************************************************************************************************/

#ifndef __FIT_CPU_H_
#define __FIT_CPU_H_

#include "OSType.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Constants used with memory barrier intrinsics. */
#define FitSY_FULL_READ_WRITE		( 15 )

/* Scheduler utilities. */
#define FitSchedule()															\
{																				\
	/* Set a PendSV to request a context switch. */								\
	FitNVIC_INT_CTRL_REG = FitNVIC_PENDSVSET_BIT;								\
																				\
	/* Barriers are normally not required but do ensure the code is completely	\
	within the specified behaviour for the architecture. */						\
	__asm volatile( "dsb" );													\
	__asm volatile( "isb" );													\
}
/*-----------------------------------------------------------*/

#define FitNVIC_INT_CTRL_REG					( * ( ( volatile uOS32_t * ) 0xe000ed04 ) )
#define FitNVIC_PENDSVSET_BIT					( 1UL << 28UL )
#define FitScheduleFromISR( b ) 				if( b ) FitSchedule()


/* Critical section management. */
extern void FitIntLock( void );
extern void FitIntUnlock( void );

#define FitIntMask()							FitRaiseBasePRI()
#define FitIntUnmask( x )						FitSetBasePRI( x )

#define FitIntMaskFromISR()						FitIntMask()
#define FitIntUnmaskFromISR( x )				FitIntUnmask( x )

#define FIT_QUICK_GET_PRIORITY	1
/* Generic helper function. */
__attribute__( ( always_inline ) ) static inline uint8_t FitCountLeadingZeros( uint32_t ulBitmap )
{
	uOS8_t ucReturn;

	__asm volatile ( "clz %0, %1" : "=r" ( ucReturn ) : "r" ( ulBitmap ) );
	return ucReturn;
}
#define FitGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31 - FitCountLeadingZeros( ( uxReadyPriorities ) ) )


#ifndef FIT_FORCE_INLINE
	#define FIT_FORCE_INLINE inline __attribute__(( always_inline))
#endif


FIT_FORCE_INLINE static uOSBase_t FitIsInsideInterrupt( void )
{
	uOS32_t ulCurrentInterrupt;
	uOSBase_t xReturn;

	/* Obtain the number of the currently executing interrupt. */
	__asm volatile( "mrs %0, ipsr" : "=r"( ulCurrentInterrupt ) );

	if( ulCurrentInterrupt == 0 )
	{
		xReturn = OS_FALSE;
	}
	else
	{
		xReturn = OS_TRUE;
	}

	return xReturn;
}

/*-----------------------------------------------------------*/

static FIT_FORCE_INLINE void FitSetBasePRI( uOS32_t ulBASEPRI )
{
	__asm volatile
	(
		"	msr basepri, %0	" :: "r" ( ulBASEPRI )
	);	
}

/*-----------------------------------------------------------*/

static FIT_FORCE_INLINE uOS32_t FitRaiseBasePRI( void )
{	
	uOS32_t ulOriginalBASEPRI, ulNewBASEPRI;

	__asm volatile
	(
		"	mrs %0, basepri											\n" \
		"	mov %1, %2												\n"	\
		"	msr basepri, %1											\n" \
		"	isb														\n" \
		"	dsb														\n" \
		:"=r" (ulOriginalBASEPRI), "=r" (ulNewBASEPRI) : "i" ( OSMAX_HWINT_PRI )
	);

	/* This return will not be reached but is necessary to prevent compiler
	warnings. */
	return ulOriginalBASEPRI;	
}

uOSStack_t *FitInitializeStack( uOSStack_t *pxTopOfStack, OSTaskFunction_t TaskFunction, void *pvParameters );
uOSBase_t FitStartScheduler( void );

void FitPendSVHandler( void ) __attribute__ (( naked ));
void FitOSTickISR( void );
void FitSVCHandler( void ) __attribute__ (( naked ));

#ifdef __cplusplus
}
#endif

#endif //__FIT_CPU_H_
