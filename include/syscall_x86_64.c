asm(".text");
asm(".global _start");
asm("_start:");
asm("mov (%rsp),%rdi");
asm("lea 8(%rsp),%rsi");
asm("call main");
asm("mov %rax,%rdi");
asm("mov $231,%eax");
asm("syscall");
long __syscall(long n,long a1,long a2,long a3,long a4,long a5,long a6);
asm("__syscall:");
asm("push %rdi");
asm("push %rsi");
asm("push %rdx");
asm("push %rcx");
asm("push %r8");
asm("push %r9");
asm("mov %rdi,%rax");
asm("mov %rsi,%rdi");
asm("mov %rdx,%rsi");
asm("mov %rcx,%rdx");
asm("mov %r8,%r10");
asm("mov %r9,%r8");
asm("mov 56(%rsp),%r9");
asm("syscall");
asm("pop %r9");
asm("pop %r8");
asm("pop %rcx");
asm("pop %rdx");
asm("pop %rsi");
asm("pop %rdi");
asm("ret");
#define syscall(ax,di,si,dx,r10,r8,r9) __syscall((ax),(long)(di),(long)(si),(long)(dx),(long)(r10),(long)(r8),(long)(r9))
#define valid(x) ((unsigned long int)(x)>0xfffffffffffff000?0:1)
int open(char *name,int flags,int mode)
{
	return syscall(2,name,flags,mode,0,0,0);
}
int close(int fd)
{
	return syscall(3,fd,0,0,0,0,0);
}
void *mmap(void *start,long size,int prot,int flags,int fd,long offset)
{
	return (void *)syscall(9,start,size,prot,flags,fd,offset);
}
long read(int fd,void *buf,int size)
{
	return syscall(0,fd,buf,size,0,0,0);
}
long write(int fd,void *buf,int size)
{
	return syscall(1,fd,buf,size,0,0,0);
}
long lseek(int fd,int pos,int w)
{
	return syscall(8,fd,pos,w,0,0,0);
}
long getrandom(void *buf,int size,int flags)
{
	return syscall(318,buf,size,flags,0,0,0);
}
void exit(int code)
{
	syscall(231,code,0,0,0,0,0);
}
