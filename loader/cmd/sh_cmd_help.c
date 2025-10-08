/** ========================================================================= *
 *
 * @file sh_cmd_help.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'help' CLI Command implementation
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
static int8_t cmd_help(shell_t * sh, uint8_t argc, const char ** argv) {
  log_printf("Available commands:\r\n");

  SHELL_ITER_COMMANDS(cmd) {
    log_printf("%s - %s\r\n", cmd->name, cmd->help);
  }

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(help, cmd_help, "Shows all commands");
