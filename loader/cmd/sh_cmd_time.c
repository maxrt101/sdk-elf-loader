/** ========================================================================= *
 *
 * @file sh_cmd_time.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'time' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "time/time.h"
#include "log/log.h"
#include <string.h>

/* Defines ================================================================== */
#define LOG_TAG SHELL

/* Macros =================================================================== */
/* Exposed macros =========================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Private functions ======================================================== */
/* Shared functions ========================================================= */
static int8_t cmd_time(shell_t * sh, uint8_t argc, const char ** argv) {
  log_printf("%u\r\n", runtime_get());

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(time, cmd_time, "Reports system time (ms tick)");
