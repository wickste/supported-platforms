/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   STM32 Controller Driver                                             */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE
#define UX_HCD_STM32_SOURCE_CODE

#include "ux_api.h"
#include "ux_hcd_stm32.h"
#include "ux_host_stack.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_hcd_stm32_request_trans_finish                  PORTABLE C      */
/*                                                           6.1.12       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*     Finish current on going data transactions.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    hcd_stm32                             Pointer to STM32 controller   */
/*    ed                                    Pointer to STM32 ED           */
/*    transfer                              Pointer to transfer request   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_utility_memory_free               Free memory                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    STM32 Controller Driver                                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  07-29-2022     Chaoqiong Xiao           Initial Version 6.1.12        */
/*                                                                        */
/**************************************************************************/
VOID  _ux_hcd_stm32_request_trans_finish(UX_HCD_STM32 *hcd_stm32, UX_HCD_STM32_ED *ed)
{
UX_TRANSFER *transfer = ed -> ux_stm32_ed_transfer_request;

    /* If there is no transfer, it's OK.  */
    if (transfer == UX_NULL)
        return;

    /* If there is no data, it's OK.  */
    if (ed -> ux_stm32_ed_data == UX_NULL)
        return;

    /* If the data is aligned, it's OK.  */
    if (ed -> ux_stm32_ed_data == transfer -> ux_transfer_request_data_pointer)
        return;

    /* If the data is IN, copy it.  */
    if (ed -> ux_stm32_ed_dir)
    {
        _ux_utility_memory_copy(transfer -> ux_transfer_request_data_pointer,
                                ed -> ux_stm32_ed_data,
                                transfer -> ux_transfer_request_actual_length);
    }

    /* Free the aligned memory.  */
    if (ed -> ux_stm32_ed_data != ed -> ux_stm32_ed_setup)
    {
      _ux_utility_memory_free(ed -> ux_stm32_ed_data);
      ed -> ux_stm32_ed_data = UX_NULL;
    }
}
