//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

extern int consoleread(int user_dst, uint64 dst, int n);
extern int consolewrite(int user_src, uint64 src, int n);

uint64
sys_read(void)
{
  int n;
  uint64 p;

  // ignore arg 0 (fd)
  argaddr(1, &p);
  argint(2, &n);

  return consoleread(1, p, n);
}

uint64
sys_write(void)
{
  int n;
  uint64 p;

  // ignore arg 0 (fd)
  argaddr(1, &p);
  argint(2, &n);

  return consolewrite(1, p, n);
}
