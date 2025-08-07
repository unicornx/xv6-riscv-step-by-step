#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  uartinit();
  printfinit();
  printf("\n");
  printf("xv6 kernel is booting\n");
  printf("\n");
  kinit();         // physical page allocator
  procinit();      // process table
  trapinit();      // trap vectors
  trapinithart();  // install kernel trap vector
  userinit();      // first user process

  scheduler();        
}
