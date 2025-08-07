#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

struct cpu cpus[NCPU];

struct proc proc[NPROC];

int nextpid = 1;

extern void kerneltrapret(void);

// initialize the proc table.
void
procinit(void)
{
  struct proc *p;
  
  for(p = proc; p < &proc[NPROC]; p++) {
      p->state = UNUSED;
      p->kstack = (uint64)kalloc();
      if(p->kstack == 0)
          panic("procinit: kalloc");
  }
}

int
cpuid()
{
  return 0;
}

// Return this CPU's cpu struct.
struct cpu*
mycpu(void)
{
  int id = cpuid();
  struct cpu *c = &cpus[id];
  return c;
}

// Return the current struct proc *, or zero if none.
struct proc*
myproc(void)
{
  struct cpu *c = mycpu();
  struct proc *p = c->proc;
  return p;
}

int
allocpid()
{
  int pid;
  
  pid = nextpid;
  nextpid = nextpid + 1;

  return pid;
}

// Look in the process table for an UNUSED proc.
// If found, initialize state required to run in the kernel,
// and return with p->lock held.
// If there are no free procs, or a memory allocation fails, return 0.
static struct proc*
allocproc(void (*start_routin)(void))
{
  struct proc *p;

  for(p = proc; p < &proc[NPROC]; p++) {
    if(p->state == UNUSED) {
      goto found;
    }
  }
  return 0;

found:
  p->pid = allocpid();
  p->state = USED;

  // Set up new context to start executing
  memset(&p->context, 0, sizeof(p->context));
  p->context.ra = (uint64)kerneltrapret;
  p->context.sp = p->kstack + PGSIZE;

  p->start = start_routin;

  return p;
}

// Set up first user process.
void
userinit(void)
{
  struct proc *p;

  p = allocproc(user_task0);
  if (p == 0)
    panic("userinit: allocproc task0 failed");
  p->state = RUNNABLE;

  p = allocproc(user_task1);
  if (p == 0)
    panic("userinit: allocproc task1 failed");
  p->state = RUNNABLE;
}

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run.
//  - swtch to start running that process.
//  - eventually that process transfers control
//    via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();

  c->proc = 0;
  for(;;){
    for(p = proc; p < &proc[NPROC]; p++) {
      if(p->state == RUNNABLE) {
        // Switch to chosen process.
        p->state = RUNNING;
        c->proc = p;
        swtch(&c->context, &p->context);

        // Process is done running for now.
        // It should have changed its p->state before coming back.
        c->proc = 0;
      }
    }
  }
}

// Switch to scheduler.
void
sched(void)
{
  struct proc *p = myproc();

  if(p->state == RUNNING)
    panic("sched RUNNING");
  if(intr_get())
    panic("sched interruptible");

  swtch(&p->context, &mycpu()->context);
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  struct proc *p = myproc();
  p->state = RUNNABLE;
  sched();
}

// A user task's very first scheduling by scheduler()
// will swtch to kerneltrapret.
void
kerneltrapret(void)
{
  struct proc *p = myproc();

  unsigned long x = r_sstatus();
  x |= SSTATUS_SPP;  // set S Previous Privilege mode to Supervisor.
  x |= SSTATUS_SPIE; // enable interrupts in S mode.
  w_sstatus(x);

  w_sepc((uint64)p->start);

  asm volatile("sret");
}
