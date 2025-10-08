/** ========================================================================= *
 *
 * @file sh_cmd_exec.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'exec' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
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
static int8_t cmd_exec(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 2) {
    log_error("Usage: exec \"COMMAND\"");
    return SHELL_FAIL;
  }

  char command[SHELL_MAX_LINE_SIZE];
  strcpy(command, argv[1]);

  return shell_execute(sh, command);
}

SHELL_DECLARE_COMMAND(exec, cmd_exec, "Execute subcommand");
