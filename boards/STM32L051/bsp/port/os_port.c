/** ========================================================================= *
 *
 * @file os_port.c
 * @date 19-08-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 * @copyright GrainMole
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
#include "stm32l051xx.h"
#include "stm32l0xx.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_adc.h"
#include "project.h"

/* Defines ================================================================== */
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
      UTIL_IF_1(USE_DEBUG, __BKPT(0));
      // TODO: implement hal/wdg
      break;

    case OS_RESET_HARD:
    case OS_RESET_SOFT:
    default:
      NVIC_SystemReset();
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

error_t os_power_mode_change_port(os_power_mode_t prev_mode, os_power_mode_t mode) {
  switch (mode) {
    case OS_POWER_MODE_NORMAL:
#if USE_ULTRALOWPOWER
      if (prev_mode == OS_POWER_MODE_DEEP_SLEEP) {
        // TODO: Should call LL_ADC_Enable?
        LL_PWR_DisableUltraLowPower();
      }
#endif
      if (prev_mode == OS_POWER_MODE_DEEP_SLEEP) {
        // Reinit UART after deep sleep as it breaks
        // FIXME: UART breaks after deep sleep
        // FIXME: vfs_open can work a bit longer to be called from IRQ, or maybe not?
        vfs_ioctl(vfs_open(&vfs, CONSOLE_FILE), VFS_IOCTL_RESET_DEVICE_DEFERRED);
      }

      break;

    case OS_POWER_MODE_FAST_SLEEP:
      HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;

    case OS_POWER_MODE_DEEP_SLEEP:
#if USE_ULTRALOWPOWER
      if (LL_ADC_IsEnabled(ADC1))
      {
        // TODO: Should save ADC state?
        LL_ADC_Disable(ADC1);

        /* TODO: Check this on L051
         * ADC regulator consumes power in STOP mode, so we need to disable it
         * before entering STOP mode.
         */
        LL_ADC_DisableInternalRegulator(ADC1);
        LL_PWR_EnableUltraLowPower();
      }
#endif
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;

    default:
      return E_INVAL;
  }

  return E_OK;
}

void os_prepare_scheduler_stack_port(void) {
  extern uint32_t _estack;

  // Switch MSP & PSP
  __set_PSP(__get_MSP());

  // Use PSP as SP
  __set_CONTROL(1 << CONTROL_SPSEL_Pos);

  // MSP shouldn't be used by anything, but if it is used it will overwrite PSP
  // This is expected, because if MSP ever got used, things have gone bad...
  __set_MSP((uint32_t) &_estack);

  // Flush pipeline
  __ISB();
}

void os_set_stack_port(void * stack) {
  // Set PSP to new value
  __set_PSP((uint32_t) stack);

  // Flush pipeline
  __ISB();
}
