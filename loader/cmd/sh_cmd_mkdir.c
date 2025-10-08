/** ========================================================================= *
*
 * @file sh_cmd_mkdir.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'mkdir' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "time/sleep.h"
#include "log/log.h"
#include "sh_cmd.h"
#include "project.h"

/* Defines ================================================================== */
#define LOG_TAG SHELL

/* Macros =================================================================== */
/* Exposed macros =========================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Private functions ======================================================== */
/* Shared functions ========================================================= */
static int8_t cmd_mkdir(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 2) {
    log_error("Usage: mkdir PATH");
    return SHELL_FAIL;
  }

  SH_ERR_REPORT_RETURN(vfs_mkdir(&vfs, argv[1]), "vfs_mkdir");

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(mkdir, cmd_mkdir, "Creates directories");
