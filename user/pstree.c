#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

struct pstat uproc[NPROC];
int nprocs;

// helper function to output the process tree rooted at pid
// calls itself recursively on any children of pid
void mktree(int indent, int pid)
{
    struct pstat *p; 

    int i ;  

    for(p = uproc; p < &uproc[NPROC]-pid; p++){ 

        i = indent; 

        if (indent==0 && p->pid == pid){  

            printf("(%d)%s\n",p->pid,p->name); 

            mktree(indent+1,pid); 

            break;

        } 

        if (p->ppid == pid){ 

          while (i>0){ 

            printf("%s","  "); 

            i--; 

            } 

            printf("(%d)%s\n",p->pid,p->name); 

            mktree(indent+1,p->pid); 

        } 

    } 

    return; 
}

int
main(int argc, char **argv)
{
  int pid = 1;
  if (argc == 2)
    pid = atoi(argv[1]);
  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  // You can remove the following print statement
  printf("%d processes\n", nprocs);

  mktree(0, pid);
  exit(0);
}