#define SBRK_ERROR ((char *)-1)

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int write(int, const void*, int);
int read(int, void*, int);
int kill(int);
int exec(const char*, char**);
int getpid(void);
char* sys_sbrk(int,int);
int pause(int);
int uptime(void);

// ulib.c
int strcmp(const char*, const char*);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
char* sbrk(int);
char* sbrklazy(int);

// printf.c
void fprintf(int, const char*, ...) __attribute__ ((format (printf, 2, 3)));
void printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));

// umalloc.c
void* malloc(uint);
void free(void*);
