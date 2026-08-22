#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAXBUF 100

void usage() {
  fprintf(2, "Usage: echo2file <file> <text>\n");
}

int
main(int argc, char *argv[])
{
  int fd, len;

  if(argc != 3){
    usage();
    exit(1);
  }

  fd = open(argv[1], O_WRONLY|O_CREATE|O_TRUNC);
  if(fd < 0){
    fprintf(2, "echo2file: cannot open %s\n", argv[1]);
    exit(1);
  }

  len = strlen(argv[2]);
  if(write(fd, argv[2], len) != len){
    fprintf(2, "echo2file: write error\n");
    exit(1);
  }

  printf("echo2file: wrote %d bytes to %s\n", len, argv[1]);

  close(fd);
  exit(0);
}
