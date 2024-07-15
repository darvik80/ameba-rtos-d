/**
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EXAMPLE_USBD_COMPOSITE_H
#define EXAMPLE_USBD_COMPOSITE_H

#include <platform_opts.h>

#if defined(CONFIG_EXAMPLE_USBD_HID) && CONFIG_EXAMPLE_USBD_HID

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

void example_usbd_composite(void);

#endif

#endif /* EXAMPLE_USBD_COMPOSITE_H */

