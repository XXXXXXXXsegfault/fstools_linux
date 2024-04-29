void *memcpy(void *dst,void *src,unsigned long int size)
{
	char *p1,*p2;
	p1=dst;
	p2=src;
	while(size)
	{
		*p1=*p2;
		++p1;
		++p2;
		--size;
	}
	return dst;
}
void *memset(void *mem,int val,unsigned long int size)
{
	char *p;
	p=mem;
	while(size)
	{
		*p=val;
		++p;
		--size;
	}
	return mem;
}
/*
int memcmp(void *mem1,void *mem2,unsigned long int size)
{
	int val;
	while(size)
	{
		if(val=*(char *)mem1-*(char *)mem2)
		{
			return val;
		}
		mem1=(char *)mem1+1;
		mem2=(char *)mem2+1;
		--size;
	}
	return 0;
}
unsigned long int strlen(char *str)
{
	unsigned long n;
	n=0;
	while(*str)
	{
		++n;
		++str;
	}
	return n;
}
unsigned long int strnlen(char *str,unsigned long int max)
{
	unsigned long int l;
	if(max==0)
	{
		return 0;
	}
	--max;
	l=0;
	while(*str&&l<max)
	{
		++l;
		++str;
	}
	return l;
}
int strcmp(char *str1,char *str2)
{
	unsigned long int l1,l2;
	l1=strlen(str1);
	l2=strlen(str2);
	if(l1>l2)
	{
		l1=l2;
	}
	return memcmp(str1,str2,l1+1);
}
int strncmp(char *str1,char *str2,unsigned long int max)
{
	unsigned long int l1,l2;
	l1=strnlen(str1,max);
	l2=strnlen(str2,max);
	if(l1>l2)
	{
		l1=l2;
	}
	return memcmp(str1,str2,l1+1);
}
char *strcpy(char *str1,char *str2)
{
	unsigned long int l;
	l=strlen(str2);
	return memcpy(str1,str2,l+1);
}
char *strcat(char *dst,char *src)
{
	unsigned long int l;
	l=strlen(dst);
	strcpy(dst+l,src);
	return dst;
}
*/
