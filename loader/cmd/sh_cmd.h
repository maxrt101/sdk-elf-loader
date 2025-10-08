/** ========================================================================= *
 *
 * @file sh_cmd.h
 * @date 27-08-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief CLI Command handlers definitions
 *
 *  ========================================================================= */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ================================================================= */
#include "shell/shell.h"

/* Defines ================================================================== */
/* Macros =================================================================== */
/**
 * Converts boolean to SHELL_OK/SHELL_FAIL
 */
#define SHELL_BOOL_TO_RES(val) ((val) ? SHELL_OK : SHELL_FAIL)

/**
 * Checks error_t, reports if happened, return SHELL_FAIL
 */
#define SH_ERR_REPORT_RETURN(expr, log_str)                               \
  do {                                                                    \
    error_t err = (expr);                                                 \
    if (err != E_OK) {                                                    \
      log_error(log_str ": %s", error2str(err));                          \
      return SHELL_FAIL;                                                  \
    }                                                                     \
  } while (0)


/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Shared functions ========================================================= */

#ifdef __cplusplus
}
#endif