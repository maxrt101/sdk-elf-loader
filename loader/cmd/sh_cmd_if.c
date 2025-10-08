/** ========================================================================= *
 *
 * @file sh_cmd_if.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'if' CLI Command implementation
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
static int8_t cmd_if(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc < 3) {
    log_error("Usage: if \"CONDITION\" \"THEN\" [\"ELSE\"]");
    return SHELL_FAIL;
  }

  char condition[SHELL_MAX_LINE_SIZE];
  char then_branch[SHELL_MAX_LINE_SIZE];
  char else_branch[SHELL_MAX_LINE_SIZE];

  strcpy(condition, argv[1]);
  strcpy(then_branch, argv[2]);

  if (argc == 4) {
    strcpy(else_branch, argv[3]);
  }

  if (shell_execute(sh, condition) == SHELL_OK) {
    return shell_execute(sh, then_branch);
  } else if (argc == 4) {
    return shell_execute(sh, else_branch);
  }

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(if, cmd_if, "Conditional execution");
