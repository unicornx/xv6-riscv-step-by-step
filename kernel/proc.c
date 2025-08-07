#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

struct cpu cpus[NCPU];

struct proc proc[NPROC];

int nextpid = 1;

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
  p->context.ra = (uint64)start_routin;
  p->context.sp = p->kstack + PGSIZE;

  return p;
}

// Set up first user process.
void
userinit(void)
{
  struct proc *p;

  p = allocproc(user_task0);
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
