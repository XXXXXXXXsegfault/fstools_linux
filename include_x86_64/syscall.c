asm ".entry"
asm "lea 8(%rsp),%rax"
asm "push %rax"
asm "pushq 8(%rsp)"
asm "call @main"
asm "mov %rax,%rdi"
asm "mov $231,%eax"
asm "syscall"
#define valid(s) ((unsigned long)((long)(s))<=0xfffffffffffff000)
int open(char *name,int flags,int mode)
{
	asm "push %rdi"
	asm "push %rsi"
	asm "push %rdx"
	asm "push %r10"
	asm "push %r11"
	asm "mov 16(%rbp),%rdi"
	asm "mov 24(%rbp),%rsi"
	asm "mov 32(%rbp),%rdx"
	asm "mov $2,%eax"
	asm "syscall"
	asm "pop %r11"
	asm "pop %r10"
	asm "pop %rdx"
	asm "pop %rsi"
	asm "pop %rdi"
}
int write(int fd,void *buf,int size)
{
	asm "push %rdi"
	asm "push %rsi"
	asm "push %rdx"
	asm "push %r10"
	asm "push %r11"
	asm "mov 16(%rbp),%rdi"
	asm "mov 24(%rbp),%rsi"
	asm "mov 32(%rbp),%rdx"
	asm "mov $1,%eax"
	asm "syscall"
	asm "pop %r11"
	asm "pop %r10"
	asm "pop %rdx"
	asm "pop %rsi"
	asm "pop %rdi"
}
long lseek(int fd,long pos,int type)
{
	asm "push %rdi"
	asm "push %rsi"
	asm "push %rdx"
	asm "push %r10"
	asm "push %r11"
	asm "mov 16(%rbp),%rdi"
	asm "mov 24(%rbp),%rsi"
	asm "mov 32(%rbp),%rdx"
	asm "mov $8,%eax"
	asm "syscall"
	asm "pop %r11"
	asm "pop %r10"
	asm "pop %rdx"
	asm "pop %rsi"
	asm "pop %rdi"
}
void *mmap(void *start,unsigned long int size,int prot,int flags,int fd,unsigned long int off)
{
	asm "push %rdi"
	asm "push %rsi"
	asm "push %rdx"
	asm "push %r10"
	asm "push %r11"
	asm "push %r8"
	asm "push %r9"
	asm "mov 16(%rbp),%rdi"
	asm "mov 24(%rbp),%rsi"
	asm "mov 32(%rbp),%rdx"
	asm "mov 40(%rbp),%r10"
	asm "mov 48(%rbp),%r8"
	asm "mov 56(%rbp),%r9"
	asm "mov $9,%eax"
	asm "syscall"
	asm "pop %r9"
	asm "pop %r8"
	asm "pop %r11"
	asm "pop %r10"
	asm "pop %rdx"
	asm "pop %rsi"
	asm "pop %rdi"
}
void exit(int code)
{
	asm "mov 16(%rbp),%rdi"
	asm "mov $231,%eax"
	asm "syscall"
}
int getrandom(void *buf,long size,int flags)
{
	asm "push %rdi"
	asm "push %rsi"
	asm "push %rdx"
	asm "push %r10"
	asm "push %r11"
	asm "mov 16(%rbp),%rdi"
	asm "mov 24(%rbp),%rsi"
	asm "mov 32(%rbp),%rdx"
	asm "mov $318,%eax"
	asm "syscall"
	asm "pop %r11"
	asm "pop %r10"
	asm "pop %rdx"
	asm "pop %rsi"
	asm "pop %rdi"
}
