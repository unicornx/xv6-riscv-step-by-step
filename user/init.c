// Shell.

#include "kernel/types.h"
#include "user/user.h"

void
runcmd(char *cmd)
{
  if(strcmp(cmd, "exit") == 0) {
    printf("Exiting shell...\n");
    exit(0);
  } else
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

int
main(int argc, char *argv[])
{
  static char buf[100];
  int i;

  printf("init: argc = %d\n", argc);
  for(i = 0; i < argc; i++){
    printf("init: argv[%d] is: %s\n", i, argv[i]);
  }

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

  return 0;
}