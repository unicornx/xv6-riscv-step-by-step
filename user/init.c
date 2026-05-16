#include "user/user.h"

int
main(void)
{
  char c;

  for(;;) {
    // read will block until there is a enter,
    // or EOF or console buffer is full.
    read(0, &c, 1);
    printf("->");
    printf("%c", c);
  }
}
