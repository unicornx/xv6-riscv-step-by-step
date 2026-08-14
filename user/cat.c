#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
cat(int fd)
{
  // To be implemented when read/write are available.
  printf("cat: opened fd = %d\n", fd);
  printf("cat: have NOT implemented read/write yet!\n");
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    cat(0);
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    }
    cat(fd);
    close(fd);
  }
  exit(0);
}
