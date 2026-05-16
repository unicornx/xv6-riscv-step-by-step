// system calls
int write(int, const void*, int);
int getpid(void);
int pause(int);
int uptime(void);

// printf.c
void fprintf(int, const char*, ...) __attribute__ ((format (printf, 2, 3)));
void printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));
