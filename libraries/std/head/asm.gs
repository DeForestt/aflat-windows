adr brk(adr size);
int sys_write(int file, adr text, int bytes);
int sys_create(adr path, int mode);
int sys_open(adr path, int flags, int mode);
char sys_read(int file, adr to, int count);
int sys_close(int file);
int sys_fstat(int file, adr stat);
int sys_fork();
int sys_pause();