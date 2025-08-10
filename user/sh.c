// Shell.

#include "kernel/types.h"
#include "user/user.h"

void cmd_zombie(void)
{
  printf("zombie: starting\n");
  int pid = fork();
  if(pid < 0) {
    printf("zombie: fork failed\n");
    exit(1);
  }

  // Parent exits first, leaving child as a zombie.
  // The child will be reparented to init.
  if (pid > 0) {
    pause(50);
    printf("zombie: parent[%d] exiting, child[%d] was adopted by init\n", getpid(), pid);
  } else {
    pause(100);
    printf("zombie: child[%d] exiting\n", getpid());
  }
  exit(0);
}

void cmd_memtest(void)
{
  char *buf = malloc(100);
  if(buf == 0) {
    printf("Failed to allocate memory for command buffer\n");
    exit(1);
  }
  // Not must-haveto, memory will be freed when the process is destroyed.
  // But it's a good practice to free allocated memory.
  free(buf);
  printf("memtest: PASS!\n");
}

void
runcmd(char *cmd)
{
  if(strcmp(cmd, "exit") == 0) {
    printf("Exiting shell...\n");
    exit(0);
  } else if(strcmp(cmd, "zombie") == 0){
    cmd_zombie();
  } else if(strcmp(cmd, "kill") == 0) {
    printf("Oops, myself is killed...\n");
    kill(getpid());
  } else if(strcmp(cmd, "memtest") == 0) {
    cmd_memtest();
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
  exit(0);
}
