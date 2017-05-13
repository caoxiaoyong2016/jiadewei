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

#include <stdlib.h>
#include <string.h>

#include "AIOS.h"
#include "OSTask.h"

/* Constants used with the xMsgQPLock and xMsgQVLock structure members. */
AIOS_DATA static sOSBase_t const OSMSGQ_UNLOCKED			= ( ( sOSBase_t ) -1 );
AIOS_DATA static sOSBase_t const OSMSGQ_LOCKED				= ( ( sOSBase_t ) 0 );

/* OSMsgQ send mode. */
AIOS_DATA static sOSBase_t const OSMSGQ_SEND_TO_BACK		= ( ( sOSBase_t ) 0 );
AIOS_DATA static sOSBase_t const OSMSGQ_SEND_TO_FRONT		= ( ( sOSBase_t ) 1 );
AIOS_DATA static sOSBase_t const OSMSGQ_SEND_OVERWRITE		= ( ( sOSBase_t ) 2 );

static uOSBool_t OSMsgQIsEmpty( const tOSMsgQ_t *ptMsgQ )
{
	uOSBool_t bReturn;

	OSIntLock();
	{
		if( ptMsgQ->uxCurNum == ( uOSBase_t )  0 )
		{
			bReturn = OS_TRUE;
		}
		else
		{
			bReturn = OS_FALSE;
		}
	}
	OSIntUnlock();

	return bReturn;
}

static uOSBool_t OSMsgQIsFull( const tOSMsgQ_t *ptMsgQ )
{
	uOSBool_t bReturn;

	OSIntLock();
	{
		if( ptMsgQ->uxCurNum == ptMsgQ->uxMaxNum )
		{
			bReturn = OS_TRUE;
		}
		else
		{
			bReturn = OS_FALSE;
		}
	}
	OSIntUnlock();

	return bReturn;
}

#define OSMsgQLock( ptMsgQ )								\
	OSIntLock();											\
	{														\
		if( ( ptMsgQ )->xMsgQPLock == OSMSGQ_UNLOCKED )		\
		{													\
			( ptMsgQ )->xMsgQPLock = OSMSGQ_LOCKED;			\
		}													\
		if( ( ptMsgQ )->xMsgQVLock == OSMSGQ_UNLOCKED )		\
		{													\
			( ptMsgQ )->xMsgQVLock = OSMSGQ_LOCKED;			\
		}													\
	}														\
	OSIntUnlock()


static void OSMsgQUnlock( tOSMsgQ_t * const ptMsgQ )
{
	/* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER LOCKED. */

	OSIntLock();
	{
		sOSBase_t xMsgQVLock = ptMsgQ->xMsgQVLock;
		
		while( xMsgQVLock > OSMSGQ_LOCKED )
		{
			if( OSListIsEmpty( &( ptMsgQ->tMsgQPTaskList ) ) == OS_FALSE )
			{
				if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQPTaskList ) ) != OS_FALSE )
				{
					OSTaskNeedSchedule();
				}
			}
			else
			{
				break;
			}

			--xMsgQVLock;
		}

		ptMsgQ->xMsgQVLock = OSMSGQ_UNLOCKED;
	}
	OSIntUnlock();

	/* Do the same for the MsgQP lock. */
	OSIntLock();
	{
		sOSBase_t xMsgQPLock = ptMsgQ->xMsgQPLock;
		
		while( xMsgQPLock > OSMSGQ_LOCKED )
		{
			if( OSListIsEmpty( &( ptMsgQ->tMsgQVTaskList ) ) == OS_FALSE )
			{
				if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQVTaskList ) ) != OS_FALSE )
				{
					OSTaskNeedSchedule();
				}

				--xMsgQPLock;
			}
			else
			{
				break;
			}
		}

		ptMsgQ->xMsgQPLock = OSMSGQ_UNLOCKED;
	}
	OSIntUnlock();
}

static uOSBool_t OSMsgQCopyDataIn( tOSMsgQ_t * const ptMsgQ, const void *pvItemToQueue, const sOSBase_t xPosition )
{
	uOSBool_t bReturn = OS_FALSE;
	uOSBase_t uxCurNum = ptMsgQ->uxCurNum;
	
	if( xPosition == OSMSGQ_SEND_TO_BACK )
	{
		( void ) memcpy( ( void * ) ptMsgQ->pcWriteTo, pvItemToQueue, ( size_t ) ptMsgQ->uxItemSize );
		ptMsgQ->pcWriteTo += ptMsgQ->uxItemSize;
		if( ptMsgQ->pcWriteTo >= ptMsgQ->pcTail )
		{
			ptMsgQ->pcWriteTo = ptMsgQ->pcHead;
		}
	}
	else
	{
		( void ) memcpy( ( void * ) ptMsgQ->pcReadFrom, pvItemToQueue, ( size_t ) ptMsgQ->uxItemSize );
		ptMsgQ->pcReadFrom -= ptMsgQ->uxItemSize;
		if( ptMsgQ->pcReadFrom < ptMsgQ->pcHead )
		{
			ptMsgQ->pcReadFrom = ( ptMsgQ->pcTail - ptMsgQ->uxItemSize );
		}

		if( xPosition == OSMSGQ_SEND_OVERWRITE )
		{
			if( uxCurNum > ( uOSBase_t ) 0 )
			{
				--uxCurNum;
			}
		}
	}

	ptMsgQ->uxCurNum = uxCurNum + 1;

	return bReturn;
}

static void OSMsgQCopyDataOut( tOSMsgQ_t * const ptMsgQ, void * const pvBuffer )
{
	ptMsgQ->pcReadFrom += ptMsgQ->uxItemSize;
	if( ptMsgQ->pcReadFrom >= ptMsgQ->pcTail )
	{
		ptMsgQ->pcReadFrom = ptMsgQ->pcHead;
	}

	( void ) memcpy( ( void * ) pvBuffer, ( void * ) ptMsgQ->pcReadFrom, ( size_t ) ptMsgQ->uxItemSize );
}

sOSBase_t OSMsgQReset( OSMsgQHandle_t MsgQHandle, uOSBool_t bNewQueue )
{
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	OSIntLock();
	{
		ptMsgQ->pcTail = ptMsgQ->pcHead + ( ptMsgQ->uxMaxNum * ptMsgQ->uxItemSize );
		ptMsgQ->uxCurNum = ( uOSBase_t ) 0U;
		ptMsgQ->pcWriteTo = ptMsgQ->pcHead;
		ptMsgQ->pcReadFrom = ptMsgQ->pcHead + ( ( ptMsgQ->uxMaxNum - ( uOSBase_t ) 1U ) * ptMsgQ->uxItemSize );
		ptMsgQ->xMsgQPLock = OSMSGQ_UNLOCKED;
		ptMsgQ->xMsgQVLock = OSMSGQ_UNLOCKED;

		if( bNewQueue == OS_FALSE )
		{
			if( OSListIsEmpty( &( ptMsgQ->tMsgQVTaskList ) ) == OS_FALSE )
			{
				if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQVTaskList ) ) != OS_FALSE )
				{
					OSSchedule();
				}
			}
		}
		else
		{
			OSListInitialise( &( ptMsgQ->tMsgQVTaskList ) );
			OSListInitialise( &( ptMsgQ->tMsgQPTaskList ) );
		}
	}
	OSIntUnlock();

	return OS_TRUE;
}

OSMsgQHandle_t OSMsgQCreate( const uOSBase_t uxQueueLength, const uOSBase_t uxItemSize )
{
	tOSMsgQ_t *ptNewMsgQ;
	size_t xQSizeInBytes;
	OSMsgQHandle_t xReturn = OS_NULL;

	if( uxItemSize == ( uOSBase_t ) 0 )
	{
		return xReturn;
	}
	else
	{
		xQSizeInBytes = ( size_t ) ( uxQueueLength * uxItemSize ) + ( size_t ) 1;
	}

	ptNewMsgQ = ( tOSMsgQ_t * ) OSMemMalloc( sizeof( tOSMsgQ_t ) + xQSizeInBytes );

	if( ptNewMsgQ != OS_NULL )
	{
		ptNewMsgQ->pcHead = ( ( sOS8_t * ) ptNewMsgQ ) + sizeof( tOSMsgQ_t );

		ptNewMsgQ->uxMaxNum = uxQueueLength;
		ptNewMsgQ->uxItemSize = uxItemSize;
		( void ) OSMsgQReset( ptNewMsgQ, OS_TRUE );

		xReturn = ptNewMsgQ;
	}

	return xReturn;
}

void OSMsgQDelete( OSMsgQHandle_t MsgQHandle )
{
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	OSMemFree( ptMsgQ );
}

sOSBase_t OSMsgQSetID(OSMsgQHandle_t const MsgQHandle, sOSBase_t xID)
{
	if(MsgQHandle == OS_NULL)
	{
		return 1;
	}
	OSIntLock();
	{
		MsgQHandle->xID = xID;
	}
	OSIntUnlock();

	return 0;
}

sOSBase_t OSMsgQGetID(OSMsgQHandle_t const MsgQHandle)
{
	sOSBase_t xID = 0;
	
	OSIntLock();
	if(MsgQHandle != OS_NULL)
	{
		xID = MsgQHandle->xID;
	}
	OSIntUnlock();

	return xID;	
}

static uOSBool_t OSMsgQSendGeneral( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue, uOSTick_t uxTicksToWait, const sOSBase_t xCopyPosition )
{
	uOSBool_t bEntryTimeSet = OS_FALSE, bNeedSchedule;
	tOSTimeOut_t tTimeOut;
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	for( ;; )
	{
		OSIntLock();
		{
			if( ( ptMsgQ->uxCurNum < ptMsgQ->uxMaxNum ) || ( xCopyPosition == OSMSGQ_SEND_OVERWRITE ) )
			{
				bNeedSchedule = OSMsgQCopyDataIn( ptMsgQ, pvItemToQueue, xCopyPosition );

				if( OSListIsEmpty( &( ptMsgQ->tMsgQPTaskList ) ) == OS_FALSE )
				{
					if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQPTaskList ) ) != OS_FALSE )
					{
						OSSchedule();
					}
				}
				else if( bNeedSchedule != OS_FALSE )
				{
					OSSchedule();
				}

				OSIntUnlock();
				return OS_TRUE;
			}
			else
			{
				if( uxTicksToWait == ( uOSTick_t ) 0 )
				{
					OSIntUnlock();
					//the MsgQ is full
					return OS_FALSE;
				}
				else if( bEntryTimeSet == OS_FALSE )
				{
					OSTaskSetTimeOutState( &tTimeOut );
					bEntryTimeSet = OS_TRUE;
				}
			}
		}
		OSIntUnlock();

		/* Interrupts and other tasks can send to or receive from the MsgQ
		To avoid confusion, we lock the scheduler and the MsgQ. */
		OSScheduleLock();
		OSMsgQLock( ptMsgQ );

		if( OSTaskGetTimeOutState( &tTimeOut, &uxTicksToWait ) == OS_FALSE )
		{
			if( OSMsgQIsFull( ptMsgQ ) != OS_FALSE )
			{
				OSTaskListOfEventAdd( &( ptMsgQ->tMsgQVTaskList ), uxTicksToWait );

				OSMsgQUnlock( ptMsgQ );

				if( OSScheduleUnlock() == OS_FALSE )
				{
					OSSchedule();
				}
			}
			else
			{
				/* Try again. */
				OSMsgQUnlock( ptMsgQ );
				( void ) OSScheduleUnlock();
			}
		}
		else
		{
			/* The timeout has expired. */
			OSMsgQUnlock( ptMsgQ );
			( void ) OSScheduleUnlock();
			//the MsgQ is full
			return OS_FALSE;
		}
	}
}

uOSBool_t OSMsgQSend( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue, uOSTick_t uxTicksToWait)
{
	return OSMsgQSendGeneral(MsgQHandle, pvItemToQueue, uxTicksToWait, OSMSGQ_SEND_TO_BACK);
}

uOSBool_t OSMsgQOverwrite( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue)
{
	return OSMsgQSendGeneral(MsgQHandle, pvItemToQueue, ( uOSTick_t )0U, OSMSGQ_SEND_OVERWRITE);
}

uOSBool_t OSMsgQSendToHead( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue, uOSTick_t uxTicksToWait)
{
	return OSMsgQSendGeneral(MsgQHandle, pvItemToQueue, uxTicksToWait, OSMSGQ_SEND_TO_FRONT);
}

static uOSBool_t OSMsgQSendGeneralFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue, uOSBool_t * const pbNeedSchedule, const sOSBase_t xCopyPosition )
{
	uOSBool_t bReturn;
	uOSBase_t uxIntSave;
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	uxIntSave = OSIntMaskFromISR();
	{
		if( ( ptMsgQ->uxCurNum < ptMsgQ->uxMaxNum ) || ( xCopyPosition == OSMSGQ_SEND_OVERWRITE ) )
		{
			const sOSBase_t xMsgQVLock = ptMsgQ->xMsgQVLock;
	
			( void ) OSMsgQCopyDataIn( ptMsgQ, pvItemToQueue, xCopyPosition );

			if( xMsgQVLock == OSMSGQ_UNLOCKED )
			{
				if( OSListIsEmpty( &( ptMsgQ->tMsgQPTaskList ) ) == OS_FALSE )
				{
					if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQPTaskList ) ) != OS_FALSE )
					{
						if( pbNeedSchedule != OS_NULL )
						{
							*pbNeedSchedule = OS_TRUE;
						}
					}
				}
			}
			else
			{
				ptMsgQ->xMsgQVLock = ( sOSBase_t )(xMsgQVLock + 1);
			}

			bReturn = OS_TRUE;
		}
		else
		{
			//the MsgQ is full
			bReturn = OS_FALSE;
		}
	}
	OSIntUnmaskFromISR( uxIntSave );

	return bReturn;
}

uOSBool_t OSMsgQSendFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue)
{
	uOSBool_t bReturn = OS_FALSE;
	uOSBool_t bNeedSchedule = OS_FALSE;
	
	bReturn = OSMsgQSendGeneralFromISR( ( OSMsgQHandle_t )MsgQHandle, pvItemToQueue, &bNeedSchedule, OSMSGQ_SEND_TO_BACK );
	if(SCHEDULER_RUNNING == OSTaskGetSchedulerState())
	{
		OSScheduleFromISR( bNeedSchedule );
	}

	return bReturn;
}

uOSBool_t OSMsgQOverwriteFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue)
{
	uOSBool_t bRet = OS_FALSE;
	uOSBool_t bNeedSchedule = OS_FALSE;
	
	bRet = OSMsgQSendGeneralFromISR( ( OSMsgQHandle_t )MsgQHandle, pvItemToQueue, &bNeedSchedule, OSMSGQ_SEND_OVERWRITE );
	if(SCHEDULER_RUNNING == OSTaskGetSchedulerState())
	{
		OSScheduleFromISR( bNeedSchedule );
	}

	return bRet;
}

uOSBool_t OSMsgQSendToHeadFromISR( OSMsgQHandle_t MsgQHandle, const void * const pvItemToQueue)
{
	uOSBool_t bReturn = OS_FALSE;
	uOSBool_t bNeedSchedule = OS_FALSE;
	
	bReturn = OSMsgQSendGeneralFromISR( ( OSMsgQHandle_t )MsgQHandle, pvItemToQueue, &bNeedSchedule, OSMSGQ_SEND_TO_FRONT );
	if(SCHEDULER_RUNNING == OSTaskGetSchedulerState())
	{
		OSScheduleFromISR( bNeedSchedule );
	}

	return bReturn;
}

static uOSBool_t OSMsgQReceiveGeneral( OSMsgQHandle_t MsgQHandle, void * const pvBuffer, uOSTick_t uxTicksToWait, const uOSBool_t bJustPeeking )
{
	uOSBool_t bEntryTimeSet = OS_FALSE;
	tOSTimeOut_t tTimeOut;
	sOS8_t *pcOriginalReadPosition;
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	for( ;; )
	{
		OSIntLock();
		{
			const uOSBase_t uxCurNum = ptMsgQ->uxCurNum;
			
			if( uxCurNum > ( uOSBase_t ) 0 )
			{
				pcOriginalReadPosition = ptMsgQ->pcReadFrom;

				OSMsgQCopyDataOut( ptMsgQ, pvBuffer );

				if( bJustPeeking == OS_FALSE )
				{
					ptMsgQ->uxCurNum = uxCurNum - 1;

					if( OSListIsEmpty( &( ptMsgQ->tMsgQVTaskList ) ) == OS_FALSE )
					{
						if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQVTaskList ) ) != OS_FALSE )
						{
							OSSchedule();
						}
					}
				}
				else
				{
					ptMsgQ->pcReadFrom = pcOriginalReadPosition;

					if( OSListIsEmpty( &( ptMsgQ->tMsgQPTaskList ) ) == OS_FALSE )
					{
						if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQPTaskList ) ) != OS_FALSE )
						{
							OSSchedule();
						}
					}
				}

				OSIntUnlock();
				return OS_TRUE;
			}
			else
			{
				if( uxTicksToWait == ( uOSTick_t ) 0 )
				{
					OSIntUnlock();
					//the MsgQ is empty
					return OS_FALSE;
				}
				else if( bEntryTimeSet == OS_FALSE )
				{
					OSTaskSetTimeOutState( &tTimeOut );
					bEntryTimeSet = OS_TRUE;
				}
			}
		}
		OSIntUnlock();

		/* Interrupts and other tasks can send to or receive from the MsgQ
		To avoid confusion, we lock the scheduler and the MsgQ. */
		OSScheduleLock();
		OSMsgQLock( ptMsgQ );

		if( OSTaskGetTimeOutState( &tTimeOut, &uxTicksToWait ) == OS_FALSE )
		{
			if( OSMsgQIsEmpty( ptMsgQ ) != OS_FALSE )
			{
				OSTaskListOfEventAdd( &( ptMsgQ->tMsgQPTaskList ), uxTicksToWait );
				OSMsgQUnlock( ptMsgQ );
				if( OSScheduleUnlock() == OS_FALSE )
				{
					OSSchedule();
				}
			}
			else
			{
				/* Try again. */
				OSMsgQUnlock( ptMsgQ );
				( void ) OSScheduleUnlock();
			}
		}
		else
		{
			OSMsgQUnlock( ptMsgQ );
			( void ) OSScheduleUnlock();
			
			if( OSMsgQIsEmpty( ptMsgQ ) != OS_FALSE )
			{
				//the MsgQ is empty
				return OS_FALSE;
			}
		}
	}
}

uOSBool_t OSMsgQPeek( OSMsgQHandle_t MsgQHandle, void * const pvBuffer, uOSTick_t uxTicksToWait)
{
	return OSMsgQReceiveGeneral( ( OSMsgQHandle_t )MsgQHandle, pvBuffer, uxTicksToWait, OS_TRUE );
}
uOSBool_t OSMsgQReceive( OSMsgQHandle_t MsgQHandle, void * const pvBuffer, uOSTick_t uxTicksToWait)
{
	return OSMsgQReceiveGeneral( ( OSMsgQHandle_t )MsgQHandle, pvBuffer, uxTicksToWait, OS_FALSE );
}

uOSBool_t OSMsgQReceiveFromISR( OSMsgQHandle_t MsgQHandle, void * const pvBuffer )
{
	uOSBool_t bReturn;
	uOSBase_t uxIntSave;
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;
	uOSBool_t bNeedSchedule = OS_FALSE;
	
	uxIntSave = OSIntMaskFromISR();
	{
		const uOSBase_t uxCurNum = ptMsgQ->uxCurNum;
		
		if( uxCurNum > ( uOSBase_t ) 0 )
		{
			const sOSBase_t xMsgQPLock = ptMsgQ->xMsgQPLock;
			
			OSMsgQCopyDataOut( ptMsgQ, pvBuffer );
			ptMsgQ->uxCurNum = uxCurNum - 1;

			if( xMsgQPLock == OSMSGQ_UNLOCKED )
			{
				if( OSListIsEmpty( &( ptMsgQ->tMsgQVTaskList ) ) == OS_FALSE )
				{
					if( OSTaskListOfEventRemove( &( ptMsgQ->tMsgQVTaskList ) ) != OS_FALSE )
					{
						bNeedSchedule = OS_TRUE;
					}
				}
			}
			else
			{
				ptMsgQ->xMsgQPLock = ( sOSBase_t )(xMsgQPLock + 1);
			}
			bReturn = OS_TRUE;
		}
		else
		{
			bReturn = OS_FALSE;
		}
	}
	OSIntUnmaskFromISR( uxIntSave );

	if(SCHEDULER_RUNNING == OSTaskGetSchedulerState())
	{
		OSScheduleFromISR( bNeedSchedule );
	}
	
	return bReturn;
}

uOSBool_t OSMsgQPeekFromISR( OSMsgQHandle_t MsgQHandle,  void * const pvBuffer )
{
	uOSBool_t bReturn;
	uOSBase_t uxIntSave;
	sOS8_t *pcOriginalReadPosition;
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	uxIntSave = OSIntMaskFromISR();
	{
		if( ptMsgQ->uxCurNum > ( uOSBase_t ) 0 )
		{
			pcOriginalReadPosition = ptMsgQ->pcReadFrom;
			OSMsgQCopyDataOut( ptMsgQ, pvBuffer );
			ptMsgQ->pcReadFrom = pcOriginalReadPosition;

			bReturn = OS_TRUE;
		}
		else
		{
			bReturn = OS_FALSE;
		}
	}
	OSIntUnmaskFromISR( uxIntSave );

	return bReturn;
}

#if (OS_TIMER_ON==1)
void OSMsgQWait( OSMsgQHandle_t MsgQHandle, uOSTick_t uxTicksToWait, uOSBool_t bNeedSuspend )
{
	tOSMsgQ_t * const ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;
	if( bNeedSuspend != OS_FALSE )
	{
		uxTicksToWait = OSPEND_FOREVER_VALUE;
	}
	
	OSMsgQLock( ptMsgQ );
	if( ptMsgQ->uxCurNum == ( uOSBase_t ) 0U )
	{
		/* There is nothing in the MsgQ, block for the specified period. */
		OSTaskBlockAndDelay( &( ptMsgQ->tMsgQPTaskList ), uxTicksToWait, bNeedSuspend );
	}
	OSMsgQUnlock( ptMsgQ );
}
#endif /* (OS_TIMER_ON==1) */

uOSBase_t OSMsgQGetSpaceNum( const OSMsgQHandle_t MsgQHandle )
{
	uOSBase_t uxReturn;
	tOSMsgQ_t *ptMsgQ;

	ptMsgQ = ( tOSMsgQ_t * ) MsgQHandle;

	OSIntLock();
	{
		uxReturn = ptMsgQ->uxMaxNum - ptMsgQ->uxCurNum;
	}
	OSIntUnlock();

	return uxReturn;
}

uOSBase_t OSMsgQGetMsgNum( const OSMsgQHandle_t MsgQHandle )
{
	uOSBase_t uxReturn;

	OSIntLock();
	{
		uxReturn = ( ( tOSMsgQ_t * ) MsgQHandle )->uxCurNum;
	}
	OSIntUnlock();

	return uxReturn;
}
