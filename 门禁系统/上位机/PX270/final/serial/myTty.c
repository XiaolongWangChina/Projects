#ifndef __MYTTY_C_
#define __MYTTY_C_


#include <unistd.h>
#include <stdlib.h> 
//#include <linux/spinlock.h>
#include "myTty.h"

#define BAUDRATE B230400

#define COM2_485 "/dev/ttyS2" 


static int fd_485;
struct termios oldtio_485,newtio_485,oldstdtio_485,newstdtio_485;

int my_tty_init()
{

   fd_485 = open(COM2_485, O_RDWR| O_NONBLOCK );//读写，非阻塞的方式打开文设备

	if (fd_485 <0) {
		perror(COM2_485);
		exit(-1);
  	}
	tcgetattr(0,&oldstdtio_485);/* 将标准输出的终端参数保存到 oldstdtio_485 */
  	tcgetattr(fd_485,&oldtio_485); /*将当前的参数保存到 oldtio_485 */
  	tcgetattr(fd_485,&newstdtio_485); /* get working stdtio */
	newtio_485.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
	newtio_485.c_iflag = IGNPAR; /*忽略帧错误和奇偶校验错*/
	newtio_485.c_oflag = 0;		/*output flag*/
 	newtio_485.c_lflag = 0;
 	newtio_485.c_cc[VMIN]=1; /* 非标准模式读的最小字符数。 */
	newtio_485.c_cc[VTIME]=0;
 /* now clean the modem line and activate the settings for modem */
 	tcflush(fd_485, TCIFLUSH);//TCIFLUSH 刷新收到的数据但是不读 
	tcsetattr(fd_485,TCSANOW,&newtio_485);/*set attrib	  */
	return 0;
}

int my_tty_read( char *buf,int nbytes)
{
	int tmp = -2;
	//unsigned char ch;
	//spinlock_t lock;
	//spin_lock_init(&lock);
	usleep(1000);
	//spin_lock(&lock);
	tmp = read(fd_485,buf,nbytes);
	//spin_unlock(&lock);
	//ch = buf[0];

	if(tmp > 0)
		printf("+++TTY_READ=0x%x\n",buf[0]);

	return tmp;
}

int my_tty_write(char *buf,int nbytes)
{
	int writeRes = 9999;


	int j=0;
	printf("+++TTY_WRITE=");
	for(j=0;j<nbytes;j++)
		printf(" 0x%x,",(unsigned char)buf[j]);
	printf("\n");


	int i;
	unsigned char tempChar;
	//spinlock_t lock;
	//spin_lock_init(&lock);
	//spin_lock(&lock);
	for(i=0; i<nbytes; i++) {
		tempChar = (unsigned char)buf[i];
		writeRes = write(fd_485,&tempChar,1);
		#ifdef DEBUG
			
		#endif
		usleep(100);
	}
	usleep(1000);
	//spin_unlock(&lock);
	return tcdrain(fd_485);
}

int my_tty_writecmd(char *buf,int nbytes)
{

	int j=0;
	printf("+++TTY_WRITECMD:");
	for(j=0;j<nbytes;j++)
		printf(" 0x%x",(unsigned char)buf[j]);
	printf("\n");


	int i;
	unsigned char tempChar;
	//spinlock_t lock;
    //spin_lock_init(&lock);
	//spin_lock(&lock);
	for(i=0; i<nbytes; i++) {
		tempChar = (unsigned char)buf[i];
		write(fd_485,&tempChar,1);
		usleep(100);
	}
	write(fd_485,"\r",1);
	usleep(300000);
	//spin_unlock(&lock);
	return tcdrain(fd_485);
}

int my_tty_end()
{
  	tcsetattr(fd_485,TCSANOW,&oldtio_485);	 	/* restore old modem setings */
  	close(fd_485);
	return 0;
}

void my_tty_fflush(){
	tcflush(fd_485,TCIFLUSH);//刷新输入输出缓冲
}

#endif
