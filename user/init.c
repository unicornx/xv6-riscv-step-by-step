// Shell.

#include "kernel/types.h"
#include "user/user.h"

void
runcmd(char *cmd)
{
  printf("Running command: \'%s\', unsupported!\n", cmd);
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
main(void)
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