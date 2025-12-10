#include <FreeRTOS.h>
#include <task.h>

#include "lib/os/abort/abort.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName) {
  os_abort("Task %s stack overflowed!", pcTaskName);
}
