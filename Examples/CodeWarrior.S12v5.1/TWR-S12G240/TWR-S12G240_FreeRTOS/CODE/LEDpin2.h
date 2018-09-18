/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : LEDpin2.h
**     Project     : TWR_S12G240_FreeRTOS
**     Processor   : MC9S12G240CLL
**     Component   : BitIO
**     Version     : Component 02.075, Driver 03.16, CPU db: 3.00.010
**     Compiler    : CodeWarrior HC12 C Compiler
**     Date/Time   : 04.09.2018, 20:29
**     Abstract    :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings    :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       41            |  PT5_IOC5
**             ----------------------------------------------------
**
**         Port name                   : T
**
**         Bit number (in port)        : 5
**         Bit mask of the port        : $0020
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTT       [$0240]
**         Port control register       : DDRT      [$0242]
**
**         Optimization for            : speed
**     Contents    :
**         GetVal - bool LEDpin2_GetVal(void);
**         ClrVal - void LEDpin2_ClrVal(void);
**         SetVal - void LEDpin2_SetVal(void);
**         NegVal - void LEDpin2_NegVal(void);
**
**Copyright : 1997 - 2011 Freescale Semiconductor, Inc. All Rights Reserved.
**
**http      : www.freescale.com
**mail      : support@freescale.com
** ###################################################################*/
/*!
** @file LEDpin2.h
** @version 03.16
** @brief
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
*/         
/*!
**  @addtogroup LEDpin2_module LEDpin2 module documentation
**  @{
*/         

#ifndef LEDpin2_H_
#define LEDpin2_H_

/* MODULE LEDpin2. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma CODE_SEG LEDpin2_CODE
/*
** ===================================================================
**     Method      :  LEDpin2_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
#define LEDpin2_GetVal() ( \
    (bool)((getReg8(PTT) & 0x20U))     /* Return port data */ \
  )

/*
** ===================================================================
**     Method      :  LEDpin2_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define LEDpin2_ClrVal() ( \
    (void)clrReg8Bits(PTT, 0x20U)      /* PTT5=0x00U */ \
  )

/*
** ===================================================================
**     Method      :  LEDpin2_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define LEDpin2_SetVal() ( \
    (void)setReg8Bits(PTT, 0x20U)      /* PTT5=0x01U */ \
  )

/*
** ===================================================================
**     Method      :  LEDpin2_NegVal (component BitIO)
**
**     Description :
**         This method negates (inverts) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define LEDpin2_NegVal() ( \
    (void)invertReg8Bits(PTT, 0x20U)   /* PTT5=invert */ \
  )

#pragma CODE_SEG DEFAULT

/* END LEDpin2. */
#endif /* #ifndef __LEDpin2_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.03 [04.46]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/