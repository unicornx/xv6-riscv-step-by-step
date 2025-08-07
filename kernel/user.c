#include "types.h"
#include "riscv.h"
#include "defs.h"

// a very rough implementaion, just to consume the cpu
static void task_delay(volatile int count)
{
  count *= 50000;
  while (count--);
}

#define DELAY 1000

void user_task0(void)
{
  printf("Task 0: Created!\n");
  while (1) {
    printf("Task 0: Running...\n");
    task_delay(DELAY);
    yield();
  }
}

void user_task1(void)
{
  printf("Task 1: Created!\n");
  while (1) {
    printf("Task 1: Running...\n");
    task_delay(DELAY);
    yield();
  }
}

