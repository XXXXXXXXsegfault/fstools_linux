asm(".text");
asm(".global _start");
asm("_start:");
asm("ldr x0,[sp]");
asm("add x1,sp,#8");
asm("bl main");
asm("mov w8,#94");
asm("svc #0");
long __syscall(long num,long a1,long a2,long a3,long a4,long a5,long a6);
asm("__syscall:");
asm("str x8,[sp,#-16]!");
asm("mov x8,x0");
asm("mov x0,x1");
asm("mov x1,x2");
asm("mov x2,x3");
asm("mov x3,x4");
asm("mov x4,x5");
asm("mov x5,x6");
asm("svc #0");
asm("ldr x8,[sp],#16");
asm("ret");
#define syscall(num,a1,a2,a3,a4,a5,a6) __syscall(num,(long)(a1),(long)(a2),(long)(a3),(long)(a4),(long)(a5),(long)(a6))
#define valid(a) ((unsigned long)(a)<0xfffffffffffff000)
int open(char *name,int flags,int mode)
{
	return syscall(56,-100,name,flags,mode,0,0);
}
int close(int fd)
{
	return syscall(57,fd,0,0,0,0,0);
}
void *mmap(void *start,long size,int prot,int flags,int fd,long offset)
{
	return (void *)syscall(222,start,size,prot,flags,fd,offset);
}
long read(int fd,void *buf,int size)
{
	return syscall(63,fd,buf,size,0,0,0);
}
long write(int fd,void *buf,int size)
{
	return syscall(64,fd,buf,size,0,0,0);
}
long lseek(int fd,long int pos,int w)
{
	return syscall(62,fd,pos,w,0,0,0);
}
long getrandom(void *buf,int size,int flags)
{
	return syscall(278,buf,size,flags,0,0,0);
}
void exit(int code)
{
	syscall(94,code,0,0,0,0,0);
}
