/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuTrigger.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : Trigger
**     Version     : Component 01.067, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-08-13, 18:42, # CodeGen: 675
**     Abstract    :
**
This component implements triggers.
Triggers are callbacks with a time when they should be executed.
**     Settings    :
**          Component name                                 : McuTrigger
**          Trigger Events                                 : (string list)
**          Low Power                                      : Disabled
**          TickPeriodMs                                   : 10
**          RTOS                                           : Disabled
**          Initialize on Init                             : yes
**     Contents    :
**         AddTrigger        - void McuTrigger_AddTrigger(uint8_t trigger, uint16_t incTicks, void...
**         AddTick           - void McuTrigger_AddTick(void);
**         AnyTriggerPending - bool McuTrigger_AnyTriggerPending(void);
**         TriggerPending    - bool McuTrigger_TriggerPending(uint8_t trigger);
**
** * Copyright (c) 2013-2020, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuTrigger.h
** @version 01.00
** @brief
**
This component implements triggers.
Triggers are callbacks with a time when they should be executed.
*/         
/*!
**  @addtogroup McuTrigger_module McuTrigger module documentation
**  @{
*/         

/* MODULE McuTrigger. */

#include "McuTrigger.h"
#include <stddef.h> /* for NULL */
#include "FreeRTOS.h"
#include "task.h"

/*!
  \brief Descriptor defining a trigger. Triggers are used set as 'reminders' for the future.
*/
typedef struct TriggerDesc {
  uint16_t triggerTime;                      /*!< trigger 'time' in ticks */
  void (*callback)(void);                    /*!< callback function */
} TriggerDesc;

static TriggerDesc TriggerList[2];           /*!< Array of triggers */

#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif
/* Internal method prototypes */
static bool CheckCallbacks(void);

/*
** ===================================================================
**     Method      :  AddTrigger (component Trigger)
**
**     Description :
**         Adds a trigger
**     Parameters  :
**         NAME            - DESCRIPTION
**         trigger         - The trigger to be added
**         incTicks        - Trigger time, in trigger ticks.
**                           The time will is relative to the current
**                           tick time
**         callback        - Callback to be called when the
**                           trigger fires
**     Returns     : Nothing
** ===================================================================
*/
void McuTrigger_AddTrigger(uint8_t trigger, uint16_t incTicks, void (*callback)(void))
{
  McuCriticalSection_CriticalVariable();

  /* method can be called from an interrupt service routine! */
  McuCriticalSection_EnterCritical();
  TriggerList[trigger].triggerTime = incTicks;
  TriggerList[trigger].callback = callback;
  McuCriticalSection_ExitCritical();
}

/*
** ===================================================================
**     Method      :  CheckCallbacks (component Trigger)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static bool CheckCallbacks(void)
{
  /* This method is called from McuTrigger_AddTick() which is called from a timer interrupt! */
  uint8_t i; /* counter through all triggers */
  void (*callback)(void); /* variable to temporarily store the callback pointer */
  bool calledCallBack = FALSE; /* flag to indicate if the callback has been called */
  McuCriticalSection_CriticalVariable();

  for(i=0;i<sizeof(TriggerList)/sizeof(TriggerDesc);i++) {
    McuCriticalSection_EnterCritical();
    if (TriggerList[i].callback!=NULL && TriggerList[i].triggerTime==0) { /* we have a trigger to check */
      callback = TriggerList[i].callback;
      TriggerList[i].callback = NULL;        /* do not trigger again */
      McuCriticalSection_ExitCritical();
      callback();
      calledCallBack = TRUE;                 /* callback may have set a trigger at the current time: rescan trigger list */
    } else {
      McuCriticalSection_ExitCritical();
    }
  } /* for */
  return calledCallBack;
}

/*
** ===================================================================
**     Method      :  AddTick (component Trigger)
**
**     Description :
**         Called by a timer function to add a tick.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuTrigger_AddTick(void)
{
  /* This method is usually called from a periodic timer interrupt! */
  uint8_t i;
  McuCriticalSection_CriticalVariable();

  for(i=0;i<sizeof(TriggerList)/sizeof(TriggerDesc);i++) {
    McuCriticalSection_EnterCritical();
    if (TriggerList[i].triggerTime!=0) { /* prevent underflow */
      TriggerList[i].triggerTime--;
    }
    McuCriticalSection_ExitCritical();
  } /* for */
  while(CheckCallbacks()) {
    /* while we have callbacks, re-iterate the list as this may have added new triggers at the current time */
  }
}

/*
** ===================================================================
**     Method      :  McuTrigger_Init (component Trigger)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void McuTrigger_Init(void)
{
  uint8_t i;

  for(i=0;i<sizeof(TriggerList)/sizeof(TriggerDesc);i++) {
    TriggerList[i].triggerTime = 0;
    TriggerList[i].callback = NULL;
  }
}

/*
** ===================================================================
**     Method      :  AnyTriggerPending (component Trigger)
**
**     Description :
**         Return true if there are any triggers pending.
**     Parameters  : None
**     Returns     :
**         ---             - true in case any triggers are pending
** ===================================================================
*/
bool McuTrigger_AnyTriggerPending(void)
{
  uint8_t i;
  bool res = FALSE;
  McuCriticalSection_CriticalVariable();

  McuCriticalSection_EnterCritical();
  for(i=0;i<sizeof(TriggerList)/sizeof(TriggerDesc);i++) {
    if (TriggerList[i].callback != NULL) {
      res = TRUE;
      break;                           /* leave the for loop */
    }
  }
  McuCriticalSection_ExitCritical();
  return res;
}

/*
** ===================================================================
**     Method      :  TriggerPending (component Trigger)
**
**     Description :
**         Determines if the given trigger is set and pending
**     Parameters  :
**         NAME            - DESCRIPTION
**         trigger         - The trigger to check
**     Returns     :
**         ---             - True if this trigger is pending, false
**                           otherwise.
** ===================================================================
*/
bool McuTrigger_TriggerPending(uint8_t trigger)
{
  return (bool)(TriggerList[trigger].callback != NULL);
}

/* END McuTrigger. */

/*!
** @}
*/
