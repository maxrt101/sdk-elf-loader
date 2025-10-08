/** ========================================================================= *
*
 * @file sh_cmd_touch.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'touch' CLI Command implementation
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
static int8_t cmd_touch(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 3) {
    log_error("Usage: touch PATH SIZE");
    return SHELL_FAIL;
  }

  SH_ERR_REPORT_RETURN(
    vfs_create_file(&vfs, argv[1], &(vfs_file_data_t){.buffer = NULL, .capacity = atoi(argv[2])}),
    "vfs_crate_file");

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(touch, cmd_touch, "Creates file");
