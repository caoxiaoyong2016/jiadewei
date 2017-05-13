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

#ifndef __OS_MSGQ_H_
#define __OS_MSGQ_H_

#include "OSType.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tOSMsgQ
{
	char						pcMsgQName[ OSNAME_MAX_LEN ];

	sOS8_t *					pcHead;	
	sOS8_t *					pcTail;	
	sOS8_t *					pcWriteTo;
	sOS8_t *					pcReadFrom;
	
	tOSList_t 					tMsgQVTaskList;		// MsgQ Send TaskList;
	tOSList_t 					tMsgQPTaskList;		// MsgQ Recv TaskList;

	volatile uOSBase_t 			uxCurNum;	
	uOSBase_t 					uxMaxNum;
	uOSBase_t 					uxItemSize;

	volatile sOSBase_t 			xMsgQPLock;			// Record the number of task which received msg from the MsgQ while it was locked.
	volatile sOSBase_t 			xMsgQVLock;			// Record the number of task which send msg to the MsgQ while it was locked.

	sOSBase_t					xID;
} tOSMsgQ_t;

typedef tOSMsgQ_t* 	OSMsgQHandle_t;

OSMsgQHandle_t	OSMsgQCreate( const uOSBase_t uxQueueLength, const uOSBase_t uxItemSize ) AIOS_FUNCTION;
void 			OSMsgQDelete( OSMsgQHandle_t MsgQHandle ) AIOS_FUNCTION;

sOSBase_t 		OSMsgQSetID(OSMsgQHandle_t const MsgQHandle, sOSBase_t xID) AIOS_FUNCTION;
sOSBase_t 		OSMsgQGetID(OSMsgQHandle_t const MsgQHandle) AIOS_FUNCTION;

uOSBool_t 		OSMsgQSend( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue, uOSTick_t uxTicksToWait) AIOS_FUNCTION;
uOSBool_t 		OSMsgQOverwrite( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue) AIOS_FUNCTION;

uOSBool_t 		OSMsgQSendFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue) AIOS_FUNCTION;
uOSBool_t 		OSMsgQOverwriteFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue) AIOS_FUNCTION;

uOSBool_t 		OSMsgQSendToHead( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue, uOSTick_t uxTicksToWait) AIOS_FUNCTION;
uOSBool_t 		OSMsgQSendToHeadFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue) AIOS_FUNCTION;

uOSBool_t 		OSMsgQPeek( OSMsgQHandle_t MsgQHandle, void * const pvBuffer, uOSTick_t uxTicksToWait) AIOS_FUNCTION;
uOSBool_t 		OSMsgQReceive( OSMsgQHandle_t MsgQHandle, void * const pvBuffer, uOSTick_t uxTicksToWait) AIOS_FUNCTION;

uOSBool_t 		OSMsgQPeekFromISR( OSMsgQHandle_t MsgQHandle, void * const pvBuffer ) AIOS_FUNCTION;
uOSBool_t 		OSMsgQReceiveFromISR( OSMsgQHandle_t MsgQHandle, void * const pvBuffer) AIOS_FUNCTION;

uOSBase_t 		OSMsgQGetSpaceNum( const OSMsgQHandle_t MsgQHandle ) AIOS_FUNCTION;
uOSBase_t 		OSMsgQGetMsgNum( const OSMsgQHandle_t MsgQHandle ) AIOS_FUNCTION;

sOSBase_t		OSMsgQReset( OSMsgQHandle_t MsgQHandle, uOSBool_t bNewQueue ) AIOS_FUNCTION;

#if (OS_TIMER_ON==1)
void 			OSMsgQWait( OSMsgQHandle_t MsgQHandle, uOSTick_t uxTicksToWait, uOSBool_t bNeedSuspend ) AIOS_FUNCTION;
#endif /* (OS_TIMER_ON==1) */

#ifdef __cplusplus
}
#endif

#endif /* __OS_MSGQ_H_ */

