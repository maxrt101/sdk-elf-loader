/** ========================================================================= *
 *
 * @file sh_cmd_log.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'log' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "log/log.h"

/* Defines ================================================================== */
#define LOG_TAG SHELL

/* Macros =================================================================== */
/* Exposed macros =========================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Private functions ======================================================== */
/* Shared functions ========================================================= */
static int8_t cmd_log(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc < 3) {
    log_error("Usage: log LEVEL ...");
    return SHELL_FAIL;
  }

  log_level_t level = log_level_from_str(argv[1]);

  for (uint8_t i = 2; i < argc; ++i) {
    log_fmt(level, "%s", argv[i]);
  }

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(log, cmd_log, "Log");
