// Create a zombie process that
// must be reparented at exit.

#include "kernel/types.h"
#include "user/user.h"

int
main(void)
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
    printf("zombie: parent exiting\n");
  } else {
    pause(100);
    printf("zombie: child exiting\n");
  }
  exit(0);
}
