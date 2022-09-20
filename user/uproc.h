enum uprocstate {UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE};

struct uproc{
    int pid;
    enum uprocstate state;
    uint64 size;
    int ppid;
    char name [16];
};