/** ========================================================================= *
*
 * @file sh_cmd_rm.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'rm' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "time/sleep.h"
#include "error/assertion.h"
#include "log/log.h"
#include "project.h"
#include "sh_cmd.h"

/* Defines ================================================================== */
#define LOG_TAG SHELL

/* Macros =================================================================== */
/* Exposed macros =========================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Private functions ======================================================== */
/* Shared functions ========================================================= */
static int8_t cmd_rm(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 2) {
    log_error("Usage: rm FILE");
    return SHELL_FAIL;
  }

  SH_ERR_REPORT_RETURN(vfs_remove(&vfs, argv[1]), "vfs_remove");

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(rm, cmd_rm, "Remove file");
