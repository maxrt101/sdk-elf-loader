/** ========================================================================= *
*
 * @file sh_cmd_sync.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'sync' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "shell/shell_util.h"
#include "time/sleep.h"
#include "log/log.h"
#include "storage/storage.h"
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
static int8_t cmd_sync(shell_t * sh, uint8_t argc, const char ** argv) {
#if 0
  if (argc > 1) {
    if (!strcmp(argv[1], "--dump")) {
      SHELL_ERR_REPORT_RETURN(storage_dump(GET_STORAGE_PTR()), "storage_dump");
    } else {
      SHELL_ERR_REPORT_RETURN(storage_save_file(&vfs, GET_STORAGE_PTR(), argv[1]), "storage_save_file");
    }
  } else {
    SHELL_ERR_REPORT_RETURN(storage_load_all(&vfs, GET_STORAGE_PTR()), "storage_load_all");
  }
#endif

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(sync, cmd_sync, "Synchronizes files from VFS to storage");
