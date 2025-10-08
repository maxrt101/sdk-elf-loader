/** ========================================================================= *
 *
 * @file sh_cmd_check.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'check' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "log/log.h"
#include "sh_cmd.h"
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
static int8_t cmd_check(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc < 2) {
    log_error("Usage: check VAL1 [!=|==|>|<|<=|>=] [VAL2] ");
    return SHELL_FAIL;
  }

  const char * val1 = argv[1];

  if (argc == 4) {
    const char * operation = argv[2];
    const char * val2 = argv[3];

    if (!strcmp(operation, "==")) {
      return SHELL_BOOL_TO_RES(!strcmp(val1, val2));
    } else if (!strcmp(operation, "!=")) {
      return SHELL_BOOL_TO_RES(strcmp(val1, val2));
    } else if (!strcmp(operation, ">")) {
      return SHELL_BOOL_TO_RES(atoi(val1) > atoi(val2));
    } else if (!strcmp(operation, "<")) {
      return SHELL_BOOL_TO_RES(atoi(val1) > atoi(val2));
    } else if (!strcmp(operation, ">=")) {
      return SHELL_BOOL_TO_RES(atoi(val1) >= atoi(val2));
    } else if (!strcmp(operation, "<=")) {
      return SHELL_BOOL_TO_RES(atoi(val1) <= atoi(val2));
    } else {
      return SHELL_FAIL;
    }
  } else {
    char * value;
    return SHELL_BOOL_TO_RES(shell_env_find(sh, val1, &value) == E_OK);
  }
}

SHELL_DECLARE_COMMAND(check, cmd_check, "Comparison between 2 ints");
