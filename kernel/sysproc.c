#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"
#include <stdlib.h>


uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_wait2(void)
{
  uint64 p1;
  uint64 p2;
  if (argaddr(0, &p1) < 0 || argaddr(1, &p2) < 0)
    return -1;
  
  return wait2(p1, p2);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_getprocs(uint64 addr)
{

  if (argaddr(0, &addr) < 0)
    return -1;

  return procinfo(addr);
}

int sys_freepmem(void){
  return kfreepagecount()*PGSIZE;
}

uint64 sys_sem_init(void){
  uint64 usem_addr;

  int value;
  int pshared;


  // Get address
  if(argaddr(0, &usem_addr) < 0)
    return -1;
  
  if(argint(1, &pshared) < 0)
    return -1;
  
  if(argint(2, &value)<0)
    return -1;
  
  if (pshared != 1){
    return -1;
  }

  // Allocate a semaphore and get its index
  int sem_index = semalloc();

  // copy the sem_index value into the user's sem_t variable
  struct proc *p = myproc();
  semtable.sem[sem_index].count = value;

  if (copyout(p->pagetable, usem_addr, (char*)&sem_index, sizeof(int))<0){
    semdealloc(sem_index);
    return -1;
  }
  

  return 0;
  
}

uint64 sys_sem_destroy(void){
  uint64 usem_addr;

  // Get address
  if(argaddr(0, &usem_addr) < 0)
    return -1;
  
  int sem_index;


  // copy the sem_index value into the user's sem_t variable
  struct proc *p = myproc();

  if (copyin(p->pagetable, (char*)&sem_index, usem_addr, sizeof(int)) < 0){
    return -1;
  }
  acquire(&semtable.sem[sem_index].lock);
  if (semtable.sem[sem_index].valid != 1){
    release(&semtable.sem[sem_index].lock);
    return -1;
  }
  semdealloc(sem_index);

  release(&semtable.sem[sem_index].lock);

  return 0;
}

uint64 sys_sem_wait(void){
  uint64 usem_addr;

  // Get address
  if(argaddr(0, &usem_addr) < 0)
    return -1;
  


  // copy the sem_index value into the user's sem_t variable
  struct proc *p = myproc();

  int sem_index;

  if (copyin(p->pagetable, (char*)&sem_index, usem_addr, sizeof(int)) < 0){
    return -1;
  }
  acquire(&semtable.sem[sem_index].lock);
  if(semtable.sem[sem_index].valid != 1){
    release(&semtable.sem[sem_index].lock);
    //printf("invalid semaphore");
    return -1;
  }

  
  while(semtable.sem[sem_index].count == 0){
    //printf("sleep");
    sleep((void *)&semtable.sem[sem_index], &semtable.sem[sem_index].lock);
    
  }
  //printf("woken");
  semtable.sem[sem_index].count -= 1;
  release(&semtable.sem[sem_index].lock);
  return 0;
}

uint64 sys_sem_post(void){
  uint64 usem_addr;

  // Get address
  if(argaddr(0, &usem_addr) < 0)
    return -1;
  


  //copy the sem_index value into the user's sem_t variable
  struct proc *p = myproc();
  
  int sem_index;

  if (copyin(p->pagetable, (char*)&sem_index, usem_addr, sizeof(int)) < 0){
    return -1;
  }

  acquire(&semtable.sem[sem_index].lock);
  if (semtable.sem[sem_index].valid != 1){
    release(&semtable.sem[sem_index].lock);
    return -1;
  }
  else{
    semtable.sem[sem_index].count += 1;
    wakeup((void *)&semtable.sem[sem_index]);
    release(&semtable.sem[sem_index].lock);
  }
  return 0;
}