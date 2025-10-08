/** ========================================================================= *
 *
 * @file storage.h
 * @date 13-03-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief
 *
 *  ========================================================================= */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ================================================================= */
#include "error/error.h"
#include "vfs/vfs.h"

/* Defines ================================================================== */
#ifndef STORAGE_MAX_FILES
#define STORAGE_MAX_FILES   4
#endif

#ifndef STORAGE_BLOCK_SIZE
#define STORAGE_BLOCK_SIZE  128
#endif

#ifndef STORAGE_SIZE
#define STORAGE_SIZE        4096
#endif

// #ifndef STORAGE_ADDR
// #define STORAGE_ADDR        0x0
// #endif

#define STORAGE_MAGIC      0xFC0A
#define STORAGE_FILE_MAGIC 0xF11E

/* Macros =================================================================== */
/* Enums ==================================================================== */
typedef enum {
  STORAGE_FLAG_NONE = 0,
  STORAGE_FLAG_USED = 1 << 0,
} storage_flags_t;

/* Types ==================================================================== */
typedef __PACKED_STRUCT {
  uint16_t magic;
  uint8_t flags;
  char path[VFS_MAX_PATH];
  size_t size;
  uint8_t bitmap[STORAGE_SIZE / STORAGE_BLOCK_SIZE / 8];
  // uint8_t * data;
} storage_file_t;

typedef __PACKED_STRUCT {
  uint16_t magic;
  storage_file_t files[STORAGE_MAX_FILES];
  uint8_t bitmap[STORAGE_SIZE / STORAGE_BLOCK_SIZE / 8];
  uint8_t data[0];
} storage_t;

/* Variables ================================================================ */
/* Shared functions ========================================================= */
error_t storage_init(const storage_t * storage);
error_t storage_check(const storage_t * storage);
error_t storage_dump(const storage_t * storage);
error_t storage_load_all(vfs_t * vfs, const storage_t * storage);
error_t storage_save_file(vfs_t * vfs, const storage_t * storage, const char * path);


#ifdef __cplusplus
}
#endif