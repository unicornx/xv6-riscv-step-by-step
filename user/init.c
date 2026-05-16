#include "user/user.h"

int
main(void)
{
  int ticks1, ticks2;
  int pid = getpid();
  printf("hello world! my pid is %d\n", pid);

  printf("--> Sleeping 2s ......\n");
  ticks1 = uptime();
  // sleep is a system call that sleeps for a specified number of ticks
  // every tick is 1000000, about a tenth of a second(0.1s).
  pause(20); // 2 seconds
  ticks2 = uptime();
  printf("<-- Woke-up! Elasped ticks = %d.\n", ticks2 - ticks1);

  return 0;
}
