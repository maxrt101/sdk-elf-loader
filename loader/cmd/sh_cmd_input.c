/** ========================================================================= *
 *
 * @file sh_cmd_input.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'input' CLI Command implementation
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
static int8_t cmd_input(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc < 2) {
    log_error("Usage: input VAR [PROMPT]");
    return SHELL_FAIL;
  }

  const char * var = argv[1];
  const char * prompt = argc == 3 ? argv[2] : "> ";

  log_printf("%s", prompt);

  tty_line_t line;
  tty_read_line(&sh->tty, &line);

  shell_env_set(sh, var, line.buf);

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(input, cmd_input, "User input into variable");
