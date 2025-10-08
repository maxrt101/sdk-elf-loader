/** ========================================================================= *
 *
 * @file project.c
 * @date 27-02-2025
 * @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
 *
 *  ========================================================================= */

/* Includes ================================================================= */
#include "project.h"
#include "bsp.h"
#include "error/assertion.h"
#include "os/abort/abort.h"
#include "os/alloc/alloc.h"
#include "shell/shell.h"
#include "time/time.h"
#include "log/log.h"

#include "loader/loader.h"
#include "elf/elf.h"

#include "module.h"

/* Defines ================================================================== */
#define LOG_TAG MAIN

#define HEAP_SIZE 2048

#define USE_EMBEDDED_ELF 1

/* Macros =================================================================== */
/* Enums ==================================================================== */
/* Types ==================================================================== */

/* Variables ================================================================ */
device_t device;
vfs_t vfs;
VFS_DECLARE_NODE_POOL(vfs_node_pool, 8);
VFS_DECLARE_TABLE_POOL(vfs_table_pool, 8);

static os_heap_t heap;
static uint8_t heap_buffer[HEAP_SIZE];

/* Private functions ======================================================== */
void error_handler_port(error_t error, int line, const char * file) {
  os_abort("%s at %s:%d", error2str(error), file, line);
}

// TODO: ELF utils. This + elf_dump_* + maybe general ELF APIs (get_section, get_symbol, etc)
//       implement elf dump APIs (phdrs sections symbols relocations GOT unresolved symbols general info (ehdr), etc.)
//       implement convenience APIs get_section_multiple(elf, section_list, size). section list = (name, addr[out])[]
//       add types (elf_section_t {header, data}, elf_symbol_t {name, addr, size, ...})
//       implement elf_dump_*
//       implement example (main fw with shell (implement elf dump run + vfs ls cd etc (save files in flash, implement simple system with max file size & max files) + maybe some kind of uploader (maybe using uart)) and some loadable modules (cmake subprojects + meta project))
//

MODULE_EXPORT_API(runtime_get);
MODULE_EXPORT_API(log_fmt);

/* Shared functions ========================================================= */
void project_main(void) {
  bsp_init(&device);

  vfs_init(&vfs, &vfs_node_pool, &vfs_table_pool);
  vfs_mkdir(&vfs, "/dev");
  bsp_console_init(&device);
  log_init(vfs_open(&vfs, CONSOLE_FILE));

  log_info("Startup");

  if (storage_check(GET_STORAGE_PTR()) == E_CORRUPT) {
    log_warn("Storage is corrupt, erasing...");
    storage_init(GET_STORAGE_PTR());
  }

  os_heap_create(&heap, heap_buffer, HEAP_SIZE);
  os_use_heap(&heap);

#if USE_EMBEDDED_ELF
  log_info("__module_data=%p", __module_data);

  module_t mod;

  ERROR_CHECK(module_load(&mod, __module_data));

  ERROR_CHECK(elf_dump(&mod.elf));

  int (*mod_main)() = NULL;

  ERROR_CHECK(module_get_symbol(&mod, (void **) &mod_main, "main"));

  log_info("main=%p", mod_main);

  log_info("runtime=%d", runtime_get());
  log_info("res=%d", mod_main());
#endif

  shell_t shell;
  shell_init(&shell, vfs_open(&vfs, CONSOLE_FILE), NULL);

  shell_start(&shell);

  while (1) {
    shell_process(&shell);
  }
}
