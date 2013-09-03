#include <qthread.h>

class QLabel;

class myThread : public QThread
{
    public:
        myThread(QLabel * label);
        ~myThread(); 
        virtual void run();
    
    private: 
	QLabel *mylabel;
    
};


    
    
