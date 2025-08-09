// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/spinlock.h"
#include "user/user.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid;
  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }
    // We have not supported wait in this version,
    // so we just wait indefinitely.
    while(1);
  }
}
