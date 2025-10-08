/** ========================================================================= *
 *
 * @file sh_cmd_repeat.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'repeat' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "error/assertion.h"
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
static int8_t cmd_repeat(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 3) {
    log_error("Usage: repeat N \"COMMAND\"");
    return SHELL_FAIL;
  }

  int times = atoi(argv[1]);

  char command[SHELL_MAX_LINE_SIZE];
  strcpy(command, argv[2]);

  while (times--) {
    int8_t result = shell_execute(sh, command);
    ASSERT_RETURN(result == 0, result);
  }

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(repeat, cmd_repeat, "Repeat subcommand");
