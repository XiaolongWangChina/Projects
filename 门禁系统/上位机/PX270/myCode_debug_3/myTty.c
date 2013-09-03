#include <unistd.h>
#include <stdlib.h> 
#include <linux/spinlock.h>
#include "myTty.h"

#define BAUDRATE B230400

#define COM2 "/dev/ttyS2" 

static int fd;
struct termios oldtio,newtio,oldstdtio,newstdtio;

int tty_init()
{

   fd = open(COM2, O_RDWR| O_NONBLOCK );//读写，非阻塞的方式打开文设备

	if (fd <0) {
		perror(COM2);
		exit(-1);
  	}
	tcgetattr(0,&oldstdtio);/* 将标准输出的终端参数保存到 oldstdtio */
  	tcgetattr(fd,&oldtio); /*将当前的参数保存到 oldtio */
  	tcgetattr(fd,&newstdtio); /* get working stdtio */
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
	newtio.c_iflag = IGNPAR; /*忽略帧错误和奇偶校验错*/
	newtio.c_oflag = 0;		/*output flag*/
 	newtio.c_lflag = 0;
 	newtio.c_cc[VMIN]=1; /* 非标准模式读的最小字符数。 */
	newtio.c_cc[VTIME]=0;
 /* now clean the modem line and activate the settings for modem */
 	tcflush(fd, TCIFLUSH);//TCIFLUSH 刷新收到的数据但是不读 
	tcsetattr(fd,TCSANOW,&newtio);/*set attrib	  */
	return 0;
}

int tty_read(unsigned char *buf,int nbytes)
{
	int tmp = -2;
	//unsigned char ch;
	spinlock_t lock;
	spin_lock_init(&lock);
	usleep(1000);
	spin_lock(&lock);
	tmp = read(fd,buf,nbytes);
	spin_unlock(&lock);
	//ch = buf[0];

#ifdef DEBUG
	if(tmp > 0)
		printf("+++TTY_READ=0x%x\n",buf[0]);
#endif
	
	return tmp;
}

int tty_write(char *buf,int nbytes)
{
	int writeRes = 9999;

#ifdef DEBUG
	int j=0;
	printf("+++TTY_WRITE=");
	for(j=0;j<nbytes;j++)
		printf(" 0x%x,",(unsigned char)buf[j]);
	printf("\n");
#endif

	int i;
	unsigned char tempChar;
	spinlock_t lock;
	spin_lock_init(&lock);
	spin_lock(&lock);
	for(i=0; i<nbytes; i++) {
		tempChar = (unsigned char)buf[i];
		writeRes = write(fd,&tempChar,1);
		#ifdef DEBUG
			
		#endif
		usleep(100);
	}
	usleep(1000);
	spin_unlock(&lock);
	return tcdrain(fd);
}

int tty_writecmd(char *buf,int nbytes)
{



#ifdef DEBUG

	int j=0;
	printf("+++TTY_WRITECMD:");
	for(j=0;j<nbytes;j++)
		printf(" 0x%x",(unsigned char)buf[j]);
	printf("\n");
#endif

	int i;
	unsigned char tempChar;
	spinlock_t lock;
   spin_lock_init(&lock);
	spin_lock(&lock);
	for(i=0; i<nbytes; i++) {
		tempChar = (unsigned char)buf[i];
		write(fd,&tempChar,1);
		usleep(100);
	}
	write(fd,"\r",1);
	usleep(300000);
	spin_unlock(&lock);
	return tcdrain(fd);
}

int tty_end()
{
  	tcsetattr(fd,TCSANOW,&oldtio);	 	/* restore old modem setings */
  	close(fd);
	return 0;
}

void tty_fflush(){
	tcflush(fd,TCIFLUSH);//刷新输入输出缓冲
}


