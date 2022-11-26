// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

// Counting semaphore
struct semaphore {
  struct spinlock lock; // semaphore lock
  int    count; // semaphore value
  int    valid; // 1 if this entry is in use
  };
  
// OS semaphore table type
struct semtab {
  struct spinlock lock;
  struct semaphore sem[100];
};

extern struct semtab semtable;
