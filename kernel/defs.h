struct context;
struct proc;

// kalloc.c
void*           kalloc(void);
void            kfree(void *);
void            kinit(void);

// printf.c
int             printf(char*, ...) __attribute__ ((format (printf, 1, 2)));
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);

// proc.c
int             cpuid(void);
struct cpu*     mycpu(void);
void            procinit(void);
void            scheduler(void) __attribute__((noreturn));
void            userinit(void);

// swtch.S
void            swtch(struct context*, struct context*);

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);

// uart.c
void            uartinit(void);
void            uartputc_sync(int);

// user.c
void            user_task0(void);
