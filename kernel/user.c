#include "types.h"
#include "riscv.h"
#include "defs.h"

// a very rough implementaion, just to consume the cpu
static void task_delay(volatile int count)
{
  count *= 50000;
  while (count--);
}

#define DELAY 1000

void user_task0(void)
{
  while (1) {
    task_delay(DELAY);
  }
}

void user_task1(void)
{
  while (1) {
    task_delay(DELAY);
  }
}

extern int consoleread(uint64 dst, int n);
extern int consolewrite(uint64 src, int n);

int read(int fd, void* buf, int count)
{
  return consoleread((uint64)buf, count);
}

int write(int fd, const void* buf, int count)
{
  return consolewrite((uint64)buf, count);
}

void
runcmd(char *cmd)
{
  printf("Running command: \'%s\', unsupported!\n", cmd);
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
getcmd(char *buf, int nbuf)
{
  write(2, "$ ", 2);
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

void
user_shell(void)
{
  static char buf[100];

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){
    char *cmd = buf;
    while (*cmd == ' ' || *cmd == '\t')
      cmd++;
    if (*cmd == '\n') // is a blank command
      continue;
    cmd[strlen(cmd) - 1] = '\0';
    runcmd(cmd);
  }
}
