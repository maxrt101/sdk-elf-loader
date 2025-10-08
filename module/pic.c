#include <stdint.h>
#include "time/time.h"
#include "log/log.h"

uint32_t test(void) {
  return runtime_get();
}

int main() {
  log_info("Hello, World from PIC module!");
  return test();
}

