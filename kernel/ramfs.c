#include "types.h"
#include "riscv.h"
#include "defs.h"

#include "_init.c"
#include "_sh.c"
#include "_kill.c"
#include "_echo.c"
#include "_zombie.c"

struct ram_file {
  char *name;
  uchar len_name;
  uchar *program;
};

static const struct ram_file ramfs[] = {
  {"init", 5, program__init},
  {"sh", 2, program__sh},
  {"kill", 4, program__kill},
  {"echo", 4, program__echo},
  {"zombie", 6, program__zombie},
};

uchar * find_program(char *name)
{
  for (int i = 0; i < sizeof(ramfs) / sizeof(struct ram_file); i++) {
    if (strncmp(name, ramfs[i].name, ramfs[i].len_name) == 0) {
      return ramfs[i].program;
    }
  }
  return 0; // Not found
}
