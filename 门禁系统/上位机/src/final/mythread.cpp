#include <qlabel.h>
#include "mythread.h"

extern "C"
{
#include "gprs/gprs.h"
}

myThread::myThread(QLabel * label)
{
    mylabel = label;
}

myThread::~myThread() 
{
}

void myThread::run()
{
    while(true)
    {
	const char *temp = alert;
       QString alert_temp = temp;
       mylabel->setText(alert_temp);
       if(verifyFailed == 1){
	   gprs_msg("15866698861", "Verify failed.");
	   verifyFailed = 0;
       }
       sleep(1);
    }
}
