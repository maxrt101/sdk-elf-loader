/** ========================================================================= *
*
 * @file sh_cmd_run.c
 * @date 09-12-2025
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'run' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include <loader/loader.h>

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
static int8_t cmd_run(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 2) {
    log_error("Usage: run ADDR");
    return SHELL_FAIL;
  }

  void * module_data = (void *) shell_parse_int(argv[1]);

  log_info("module_data=%p", module_data);

  module_t mod;

  SHELL_ERR_REPORT_RETURN(module_load(&mod, module_data), "module_load");

  SHELL_ERR_REPORT_RETURN(elf_dump(&mod.elf), "elf_dump");

  int (*mod_main)() = NULL;

  SHELL_ERR_REPORT_RETURN(module_get_symbol(&mod, (void **) &mod_main, "main"), "elf_get_symbol");

  log_info("main=%p", mod_main);
  log_info("res=%d", mod_main());

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(run, cmd_run, "Run ELF file by address");
