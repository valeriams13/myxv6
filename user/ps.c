#include "kernel/param.h"
#include "kernel/types.h"
#include "user/uproc.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  struct uproc uproc[NPROC];
  int nprocs;

  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  // You can remove the following print statement
  printf("%d processes\n", nprocs);

  /*********************************
   * Add your code here to output information returned by getprocs()
   * Hint: see procdump() code in kernel/console.c, except that here
   * you will be outputting the first nprocs elements of the uproc array
   * and all will have active states.
   *********************************/ 

  static char *states[] = {
    [UNUSED]    "unused",
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running",
    [ZOMBIE]    "zombie"
  };

  struct uproc *p;
  char *state;

  printf("\n");
  for(p = uproc; p < &uproc[NPROC]; p++){
    if(p->state ==  UNUSED)
    continue;
    if(p->state >= 0 && p-> state < (sizeof(states)/sizeof(states[0])));
    state = states[p->state];
    else
    state = "??";
    printf("%d %s %s", p->pid, state, p->name);
  }
  exit(0);
}