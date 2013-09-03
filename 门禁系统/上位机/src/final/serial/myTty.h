
#ifndef __MYTTY_H_
#define __MYTTY_H_

#include <sys/types.h>                                                  
#include <sys/stat.h>                                                      
#include <fcntl.h>                                                       
#include <termios.h>                                                    
#include <stdio.h>  

int my_tty_init();
int my_tty_end();
int my_tty_read(char *buf,int nbytes);
int my_tty_write(char *buf,int nbytes);
int my_tty_writecmd(char *buf,int nbytes);
void my_tty_fflush();


#endif
