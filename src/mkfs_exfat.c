#include "../include/syscall.c"
#include "../include/mem.c"
#include "../include/exfat.c"
int fd;
long int size,sectors,clusters;
struct exfat_bs bsec;
int init_fs_info(void)
{
	unsigned long int fat_size;
	int x;
	size=lseek(fd,0,2);
	if(!valid(size))
	{
		write(1,"Cannot determine device size\n",29);
		return 1;
	}
	sectors=size>>9;
	if(sectors<=8192)
	{
		write(1,"Device is too small\n",20);
		return 1;
	}
	memcpy(bsec.jump,"\xeb\x76\x90",3);
	memcpy(bsec.name,"EXFAT   ",8);

	bsec.sector_size=9;
	x=3;
	while(x<27)
	{
		if(sectors>>x<0xfffffff0)
		{
			goto Out;
		}
		++x;
	}
	write(1,"Device is too large\n",20);
	return 1;
Out:
	bsec.cluster_size=x;
	bsec.fatoff=2048;
	clusters=(sectors-2048<<9)/(4+(1<<bsec.cluster_size+9));
	clusters-=(clusters*4+511+8>>9)*2+2048+(1<<bsec.cluster_size)-1>>bsec.cluster_size;

	bsec.sectors=sectors;
	bsec.clusters=clusters;
	bsec.fatlen=clusters*4+511>>9;
	bsec.clusteroff=bsec.fatlen+2048+(1<<bsec.cluster_size)-1>>bsec.cluster_size<<bsec.cluster_size;
	bsec.drive=0x80;
	bsec.rev=0x0100;
	bsec.fats=1;
	bsec.clusterroot=2+(clusters-1>>bsec.cluster_size+12)+1+1;
	memcpy(bsec.rsv+7,"\xeb\xfe",2);
	if(x==3)
	{
		bsec.clusterroot+=1;
	}
	bsec.bootflag=0xaa55;
	return 0;
}
unsigned char buf[131072];
unsigned int buf_x;
void buf_write(void *ptr,unsigned long int size)
{
	while(size)
	{
		if(size>=131072-buf_x)
		{
			if(ptr)
			{
				memcpy(buf+buf_x,ptr,131072-buf_x);
				ptr=(char *)ptr+131072-buf_x;
			}
			else
			{
				memset(buf+buf_x,0,131072-buf_x);
			}
			size-=131072-buf_x;
			buf_x=0;
			if(write(fd,buf,131072)!=131072)
			{
				write(1,"Failed to write data\n",21);
				exit(2);
			}
		}
		else
		{
			if(ptr)
			{
				memcpy(buf+buf_x,ptr,size);
				ptr=(char *)ptr+size;
			}
			else
			{
				memset(buf+buf_x,0,size);
			}
			buf_x+=size;
			size=0;
		}
	}
}
void buf_write_zero(unsigned long int size)
{
	buf_write(0,size);
}
void buf_flush(void)
{
	if(buf_x)
	{
		if(write(fd,buf,buf_x)!=buf_x)
		{
			write(1,"Failed to write data\n",21);
			exit(2);
		}
		buf_x=0;
	}
}
void write_fat(void)
{
	unsigned int val,off,x;
	buf_write("\xf8\xff\xff\xff\xff\xff\xff\xff",8);
	off=8;
	val=3;
	x=(clusters-1>>bsec.cluster_size+12)+1;
	while(x>1)
	{
		buf_write(&val,4);
		--x;
		++val;
		off+=4;
	}
	buf_write("\xff\xff\xff\xff",4);
	val+=1;
	buf_write(&val,4);
	buf_write("\xff\xff\xff\xff\xff\xff\xff\xff",8);
	buf_write_zero(((unsigned long long)bsec.fatlen<<9)-(off+12));
	buf_flush();
}
void write_bs(void)
{
	unsigned int checksum;
	int i;
	checksum=0;
	i=0;
	while(i<512)
	{
		if(i!=106&&i!=107&&i!=112)
		{
			checksum=(checksum>>1|checksum<<31)+(unsigned int)((unsigned char *)&bsec)[i];
		}
		++i;
	}
	checksum=(checksum<<1|checksum>>31)+0x55;
	checksum=(checksum>>1|checksum<<31)+0xaa;
	buf_write(&bsec,512);
	buf_write_zero(510);
	buf_write("\x55\xaa",2);
	buf_write_zero(512*9);
	i=0;
	while(i<128)
	{
		buf_write(&checksum,4);
		++i;
	}
	buf_flush();
}
void write_bitmap(void)
{
	unsigned long long int x,i,rest_len;
	unsigned char val;
	x=bsec.clusterroot-1;
	i=0;
	while(i+7<x)
	{
		buf_write("\xff",1);
		i+=8;
	}
	val=(1<<x-i)-1;
	buf_write(&val,1);
	rest_len=(clusters-1>>bsec.cluster_size+12)+1<<bsec.cluster_size+12;
	rest_len=(rest_len-x)>>3;
	buf_write_zero(rest_len);
	buf_flush();
}
void write_upcase(void)
{
	const unsigned short *p;
	unsigned short val;
	int x;
	p=exfat_upcase;
	val=0;
	buf_write(&val,2);
	while(p<exfat_upcase_end)
	{
		if(*p==0)
		{
			x=p[1];
			if(x)
			{
				do
				{
					val+=1;
					buf_write(&val,2);
					--x;
				}
				while(x);
			}
			else
			{
				buf_write(&val,2);
			}
			++p;
		}
		else if(*p==1)
		{
			x=p[1];
			while(x)
			{
				buf_write(&val,2);
				val+=2;
				buf_write(&val,2);
				--x;
			}
			++p;
		}
		else
		{
			val+=*p;
			buf_write(&val,2);
		}
		++p;
	}
	buf_write_zero((1<<bsec.cluster_size+9)-0x6cc);
}
void write_rootdir(void)
{
	static struct exfat_bitmap_dirent bdir;
	static struct exfat_upcase_dirent udir;
	static struct exfat_label_dirent ldir;
	bdir.type=0x81;
	bdir.cluster=2;
	bdir.size=(clusters+7>>3);
	udir.type=0x82;
	udir.checksum=0xe619d30d;
	udir.cluster=2+(clusters-1>>bsec.cluster_size+12)+1;
	udir.size=0x16cc;
	ldir.type=0x83;
	buf_write(&bdir,32);
	buf_write(&udir,32);
	buf_write(&ldir,32);
	buf_write_zero((1<<bsec.cluster_size+9)-96);
	buf_flush();
}
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		write(1,"Usage: mkfs.exfat <PARTITION>\n",30);
		return 1;
	}
	fd=open(argv[1],2,0);
	if(fd<0)
	{
		write(1,"Cannot open device\n",19);
		return 1;
	}
	if(init_fs_info())
	{
		return 1;
	}
	lseek(fd,0,0);
	write_bs();
	write_bs();
	lseek(fd,(unsigned long long)bsec.fatoff<<9,0);
	write_fat();
	lseek(fd,(unsigned long long)bsec.clusteroff<<9,0);
	write_bitmap();
	write_upcase();
	write_rootdir();
	return 0;
}
