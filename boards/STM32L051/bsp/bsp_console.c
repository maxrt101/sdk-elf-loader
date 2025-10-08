/** ========================================================================= *
 *
 * @file bsp_console.c
 * @date 30-08-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include <string.h>
#include "lib/log/log.h"
#include "lib/vfs/vfs.h"
#include "lib/error/assertion.h"
#include "hal/uart/uart.h"
#include "project.h"

/* Defines ================================================================== */
#define LOG_TAG                 CONSOLE_PORT
#define CONSOLE_READ_TIMEOUT_MS 100

/* Macros =================================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
typedef struct {
  uart_t *  uart;               /** Console UART handle */
  bool      write_flag;         /** Indicates that a write happened */
  bool      read_timeout_flag;  /** Indicates that a read timeout should be used */
  bool      reset_on_next_flag; /** Should reset device on next occasion */
  timeout_t read_timeout;       /** Timeout for read operation */
} console_ctx_t;

static console_ctx_t console_ctx = {NULL, false, false};

/* Private functions ======================================================== */
error_t console_open(void * ctx, vfs_file_t * file) {
  ASSERT_RETURN(ctx, E_NULL);
  console_ctx_t * console = (console_ctx_t *) ctx;

  if (console->uart == NULL) {
    uart_cfg_t cfg = {.uart_no = CONSOLE_UART_INDEX};
    return uart_init(&console->uart, &cfg);
  }

  return E_OK;
}

error_t console_read(void * ctx, vfs_file_t * file, uint8_t * buffer, size_t size) {
  ASSERT_RETURN(ctx, E_NULL);
  console_ctx_t * console = (console_ctx_t *) ctx;

  if (console->reset_on_next_flag) {
    uart_reset(console->uart);
    console->reset_on_next_flag = false;
  }

  if (console->read_timeout_flag) {
    timeout_start(&console->read_timeout, CONSOLE_READ_TIMEOUT_MS);
  }

  return uart_recv(
      console->uart, buffer, size,
      console->read_timeout_flag ? &console->read_timeout : NULL
  );
}

error_t console_write(void * ctx, vfs_file_t * file, const uint8_t * buffer, size_t size) {
  ASSERT_RETURN(ctx, E_NULL);

  console_ctx_t * console = (console_ctx_t *) ctx;

  if (console->reset_on_next_flag) {
    uart_reset(console->uart);
    console->reset_on_next_flag = false;
  }

  console->write_flag = true;

  error_t err = E_OK;

//  ATOMIC_BLOCK() {
    err = uart_send(console->uart, buffer, size);
//  }

  return err;
}

size_t console_tell(void * ctx, vfs_file_t * file) {
  ASSERT_RETURN(ctx, E_NULL);
  console_ctx_t * console = (console_ctx_t *) ctx;
  return uart_available(console->uart);
}

error_t console_ioctl(void * ctx, vfs_file_t * file, int cmd, va_list args) {
  ASSERT_RETURN(ctx, E_NULL);
  console_ctx_t * console = (console_ctx_t *) ctx;

  switch (cmd) {
    case VFS_IOCTL_SEEK: {
      break;
    }

    case VFS_IOCTL_TELL: {
      size_t * result = va_arg(args, size_t *);
      ASSERT_RETURN(result, E_INVAL);
      *result = uart_available(console->uart);
      break;
    }

    case VFS_IOCTL_RESET_DEVICE: {
      uart_reset(console->uart);
      break;
    }

    case VFS_IOCTL_RESET_DEVICE_DEFERRED: {
      console->reset_on_next_flag = true;
      break;
    }

    case VFS_IOCTL_WRITE_DETECTED: {
      bool * result = va_arg(args, bool*);
      *result = console->write_flag;
      break;
    }

    case VFS_IOCTL_WRITE_DETECTED_CLEAR: {
      console->write_flag = false;
      break;
    }

    case VFS_IOCTL_READ_TIMEOUT_ENABLE: {
      console->read_timeout_flag = va_arg(args, int);
      break;
    }

    default:
      return E_NOTIMPL;
  }

  return E_OK;
}

/* Shared functions ========================================================= */
error_t bsp_console_init(device_t * dev) {
  ASSERT_RETURN(dev, E_NULL);

  return vfs_create_block(&vfs, CONSOLE_FILE, &(vfs_block_data_t){
    &console_ctx,
    console_open,
    NULL,
    console_read,
    console_write,
    console_ioctl,
  });
}
