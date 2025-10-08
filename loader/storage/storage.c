/** ========================================================================= *
*
 * @file storage.c
 * @date 13-11-2024
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 * @brief
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "storage/storage.h"
#include "error/assertion.h"
#include "log/log.h"
#include "hal/nvm/nvm.h"

/* Defines ================================================================== */
#define LOG_TAG STORAGE

/* Macros =================================================================== */
/* Exposed macros =========================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */
/* Variables ================================================================ */
/* Private functions ======================================================== */
__STATIC_INLINE error_t storage_allocate_blocks(const storage_t * storage, uint8_t * bitmap, uint32_t blocks) {
  storage_t storage_copy;

  memcpy(&storage_copy, storage, sizeof(storage_t));

  uint32_t last_block_index = 0;
  for (uint32_t i = 0; i < blocks; ++i) {
    while (UTIL_BIT_GET(storage_copy.bitmap[last_block_index / 8], last_block_index % 8)) {
      ASSERT_RETURN(last_block_index < STORAGE_SIZE / STORAGE_BLOCK_SIZE, E_NOMEM);

      last_block_index++;
    }

    UTIL_BIT_SET(storage_copy.bitmap[last_block_index / 8], last_block_index % 8);
    UTIL_BIT_SET(bitmap[last_block_index / 8], last_block_index % 8);
  }

  return nvm_write((uint32_t) storage, (uint8_t *) &storage_copy, sizeof(storage_t));
}

__STATIC_INLINE void storage_dump_bitmap(const uint8_t * bitmap, size_t size, size_t bits_in_line) {
  static char buf[128];
  size_t buf_pos = 0;

  for (uint32_t i = 0; i < size; ++i) {
    buf[buf_pos++] = UTIL_BIT_GET(bitmap[i / 8], i % 8) ? '1' : '0';

    if (i && i % bits_in_line == 0) {
      buf[buf_pos] = 0;
      buf_pos = 0;
      log_info("%s", buf);
    }
  }
}

#if 0
__STATIC_INLINE error_t storage_save_folder(vfs_t * vfs, const storage_t * storage, vfs_file_t * folder) {
  for (size_t i = 0; i < folder->folder.children->capacity; ++i) {
    if (!folder->folder.children->nodes[i].used) {
      continue;
    }

    vfs_file_t * iter = (vfs_file_t *) folder->folder.children->nodes[i].value;

    log_printf("'%s' %s 0x%x\r\n",
      vfs_get_file_name(iter),
      vfs_node_type_to_string(iter->head.type),
      iter->head.flags);

    if (iter->head.type == VFS_FOLDER) {
      ERROR_CHECK_RETURN(storage_save_file(vfs, storage, iter));
    }

    if (iter->head.type == VFS_FOLDER) {
      ERROR_CHECK_RETURN(storage_save_folder(vfs, storage, iter));
    }
  }

  return E_OK;
}
#endif

/* Shared functions ========================================================= */
error_t storage_init(const storage_t * storage) {
  ASSERT_RETURN(storage, E_NULL);

  storage_t init = {0};

  init.magic = STORAGE_MAGIC;

  return nvm_write((uint32_t) storage, (uint8_t *) &init, sizeof(storage_t));
}

error_t storage_check(const storage_t * storage) {
  ASSERT_RETURN(storage, E_NULL);

  return storage->magic == STORAGE_MAGIC ? E_OK : E_CORRUPT;
}

error_t storage_dump(const storage_t * storage) {
  ASSERT_RETURN(storage, E_NULL);

  log_info("Storage %p (magic=0x%x data=%p size=%u block=%u)", storage, storage->magic, storage->data, STORAGE_SIZE, STORAGE_BLOCK_SIZE);
  log_info("Global block usage:");
  storage_dump_bitmap(storage->bitmap, STORAGE_SIZE / STORAGE_BLOCK_SIZE / 8, 16);

  for (uint32_t i = 0; i < STORAGE_MAX_FILES; ++i) {
    if (storage->files[i].magic == STORAGE_FILE_MAGIC && storage->files[i].flags & STORAGE_FLAG_USED) {
      log_info("File '%s' (flags=0x%x size=%u)", storage->files[i].path, storage->files[i].flags, storage->files[i].size);
      log_info("Block usage:");
      storage_dump_bitmap(storage->files[i].bitmap, STORAGE_SIZE / STORAGE_BLOCK_SIZE / 8, 16);
    }
  }

  return E_OK;
}

error_t storage_load_all(vfs_t * vfs, const storage_t * storage) {
  ASSERT_RETURN(vfs && storage, E_NULL);

  return E_OK;
}

error_t storage_save_file(vfs_t * vfs, const storage_t * storage, const char * path) {
  ASSERT_RETURN(vfs && storage, E_NULL);

  vfs_file_t * file = vfs_open(vfs, path);

  for (size_t i = 0; i < STORAGE_MAX_FILES; ++i) {
    if (!(storage->files[i].flags & STORAGE_FLAG_USED)) {
      storage_file_t storage_file = {0};
      UTIL_STR_COPY(storage_file.path, path, VFS_MAX_PATH);
      storage_file.magic = STORAGE_FILE_MAGIC;
      storage_file.flags = STORAGE_FLAG_USED;
      storage_file.size = file->file.data.size;

      size_t blocks = file->file.data.size / STORAGE_BLOCK_SIZE + (file->file.data.size % STORAGE_BLOCK_SIZE ? 1 : 0);

      ERROR_CHECK_RETURN(storage_allocate_blocks(storage, storage_file.bitmap, blocks));

      uint32_t last_block_index = 0;

      for (size_t b = 0; b < blocks; ++b) {
        while (UTIL_BIT_GET(storage_file.bitmap[last_block_index / 8], last_block_index % 8)) {
          last_block_index++;
        }

        ERROR_CHECK_RETURN(nvm_write(
          (uint32_t) storage->data + last_block_index * STORAGE_BLOCK_SIZE,
          file->file.data.buffer + b * STORAGE_BLOCK_SIZE,
          STORAGE_BLOCK_SIZE
        ));
      }

      return nvm_write((uint32_t) &storage->files[i], (uint8_t *) &storage_file, sizeof(storage_file_t));
    }
  }

  return E_NOMEM;
}
