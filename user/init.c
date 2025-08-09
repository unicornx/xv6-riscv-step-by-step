#include "user/user.h"

int
main(void)
{
  int pid = getpid();
  printf("hello world! my pid is %d\n", pid);
}
