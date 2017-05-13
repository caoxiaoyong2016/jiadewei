/**********************************************************************************************************
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

#ifndef __OS_SEMAPHORE_H_
#define __OS_SEMAPHORE_H_

#include "OSType.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (OS_SEMAPHORE_ON==1)

typedef struct tOSSem
{
	char						pcSemName[ OSNAME_MAX_LEN ];

	tOSList_t 					tSemVTaskList;		//Semaphore Post TaskList;
	tOSList_t 					tSemPTaskList;		//Semaphore Pend TaskList;

	volatile uOSBase_t 			uxCurNum;	
	uOSBase_t 					uxMaxNum;

	volatile sOSBase_t 			xSemPLock;			// Record the number of task which pend from the semaphore while it was locked.
	volatile sOSBase_t 			xSemVLock;			// Record the number of task which post to the semaphore while it was locked.

	sOSBase_t					xID;
} tOSSem_t;

typedef tOSSem_t* OSSemHandle_t;

OSSemHandle_t 	OSSemCreate( const uOSBase_t uxInitialCount ) AIOS_FUNCTION;
OSSemHandle_t 	OSSemCreateCount( const uOSBase_t uxMaxNum, const uOSBase_t uxInitialCount ) AIOS_FUNCTION;
void 			OSSemDelete(OSSemHandle_t SemHandle) AIOS_FUNCTION;

sOSBase_t 		OSSemSetID(OSSemHandle_t const SemHandle, sOSBase_t xID) AIOS_FUNCTION;
sOSBase_t 		OSSemGetID(OSSemHandle_t const SemHandle) AIOS_FUNCTION;

uOSBool_t 		OSSemPend( OSSemHandle_t SemHandle, uOSTick_t uxTicksToWait) AIOS_FUNCTION;

uOSBool_t 		OSSemPost( OSSemHandle_t SemHandle) AIOS_FUNCTION;
uOSBool_t 		OSSemPostFromISR( OSSemHandle_t SemHandle ) AIOS_FUNCTION;

#endif //(OS_SEMAPHORE_ON==1)

#ifdef __cplusplus
}
#endif

#endif //__OS_SEMAPHORE_H_
