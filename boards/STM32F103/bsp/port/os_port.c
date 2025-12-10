/** ========================================================================= *
 *
 * @file os_port.c
 * @date 19-08-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief Port implementation of SDK sys library
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "os/irq/irq.h"
#include "os/power/power.h"
#include "os/reset/reset.h"
#include "util/util.h"
#include "log/log.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_adc.h"

/* Defines ================================================================== */
#define LOOP_ON_RESET 1

/* Macros =================================================================== */
/* Exposed macros =========================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Private functions ======================================================== */
/* Shared functions ========================================================= */
void os_irq_enable_port(uint8_t irq) {
  if (irq == OS_IRQ_ALL) {
    __enable_irq();
  } else {
    HAL_NVIC_EnableIRQ(irq);
  }
}

void os_irq_disable_port(uint8_t irq) {
  if (irq == OS_IRQ_ALL) {
    __disable_irq();
  } else {
    HAL_NVIC_DisableIRQ(irq);
  }
}

__NO_RETURN void os_reset_port(os_reset_method_t method) {
  switch (method) {
    case OS_RESET_WDG:
    case OS_RESET_HARD:
    case OS_RESET_SOFT:
    default:
#if !LOOP_ON_RESET
      UTIL_IF_1(USE_DEBUG, __BKPT(0), NVIC_SystemReset());
#endif
      break;
  }

  while (1) {
    /* Wait for reset */
  }
}

os_reset_reason_t os_get_reset_reason_port(void) {
  os_reset_reason_t reason = OS_RESET_REASON_UNK;

  if (LL_RCC_IsActiveFlag_IWDGRST()) {
    reason = OS_RESET_REASON_WDG;
  } else if (LL_RCC_IsActiveFlag_WWDGRST()) {
    reason = OS_RESET_REASON_WWDG;
  } else if (LL_RCC_IsActiveFlag_SFTRST()) {
    reason = OS_RESET_REASON_SW_RST;
  } else if (LL_RCC_IsActiveFlag_PINRST()) {
    reason = OS_RESET_REASON_HW_RST;
  } else if (LL_RCC_IsActiveFlag_PORRST()) {
    reason = OS_RESET_REASON_POR;
  }

  return reason;
}

void os_abort_dump_ctx(void) {
  register volatile uint32_t sp asm("sp");

  log_fatal("SP      %p", sp);
  log_fatal("MSP     %p", __get_MSP());
  log_fatal("PSP     %p", __get_PSP());
  log_fatal("xPSR    %p", __get_xPSR());
  log_fatal("CONTROL %p", __get_CONTROL());
}
