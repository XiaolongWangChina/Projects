
#include <sys/types.h>                                                  
#include <sys/stat.h>                                                      
#include <fcntl.h>                                                       
#include <termios.h>                                                    
#include <stdio.h>  

int tty_init();
int tty_end();
int tty_read(unsigned char *buf,int nbytes);
int tty_write(char *buf,int nbytes);
int tty_writecmd(char *buf,int nbytes);
void tty_fflush();


