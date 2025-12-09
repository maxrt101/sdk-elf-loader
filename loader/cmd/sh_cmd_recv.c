/** ========================================================================= *
*
 * @file sh_cmd_recv.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief 'recv' CLI Command implementation
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "shell/shell.h"
#include "shell/shell_util.h"
#include "time/sleep.h"
#include "log/log.h"
#include "hal/uart/uart.h"
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
uart_t * console_get_uart(void);

/* Shared functions ========================================================= */
static int8_t cmd_recv(shell_t * sh, uint8_t argc, const char ** argv) {
  if (argc != 3) {
    log_error("Usage: recv FILE SIZE");
    return SHELL_FAIL;
  }

  size_t size = atoi(argv[2]);

  vfs_file_t * file = vfs_open(&vfs, argv[1]);

  if (!file) {
    log_error("Can't open file '%s'", argv[1]);
    return SHELL_FAIL;
  }

  for (uint32_t i = 0; i < size; ++i) {
    uint8_t buffer;

    SHELL_ERR_REPORT_RETURN(uart_recv(console_get_uart(), &buffer, 1, NULL), "uart_recv");

    SHELL_ERR_REPORT_RETURN(vfs_write(file, &buffer, 1), "vfs_write");
  }

  vfs_close(file);

  return SHELL_OK;
}

SHELL_DECLARE_COMMAND(recv, cmd_recv, "Receive data from uart to file");
