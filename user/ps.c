#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
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
  //struct uproc p;
  //char *state;
  
  printf("\n");


  static char *states[] = {
  [UNUSED]    "unused",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  struct pstat *p;
  char *state;

  printf("\n");
  printf("ppid pid state name size    cputime\n");
  for(p = uproc; p < &uproc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < ((sizeof(states)/sizeof((states)[0]))) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    printf("%d    %d   %s %s  %d      %d", p->ppid, p->pid, state, p->name, p->size, p->cputime);
    printf("\n");
  }






  exit(0);
}