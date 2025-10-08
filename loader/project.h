/** ========================================================================= *
 *
 * @file project.h
 * @date 27-02-2025
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 *  ========================================================================= */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ================================================================= */
#include "util/compiler.h"
#include "storage/storage.h"
#include "vfs/vfs.h"

/* Defines ================================================================== */
/* Macros =================================================================== */
#define GET_STORAGE_PTR() ((const storage_t *) &__storage)

/* Enums ==================================================================== */
/* Types ==================================================================== */
/**
 * Device context
 *
 * Contains more-or-less generic application handle, and device-specific
 * driver/protocol handles
 */
typedef struct {

} device_t;

/* Variables ================================================================ */
extern device_t device;
extern vfs_t vfs;
extern storage_t * __storage;

/* Shared functions ========================================================= */
/**
 *
 */
void project_main(void);

#ifdef __cplusplus
}
#endif