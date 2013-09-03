
typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;

/*查询门的状态*/
void getStateOfDoor();

/*将锁设为打开*/
int setLockOpen();

/*监听485接口的数据---线程*/
void* ListenSerial485(void* data);

void* hostDoto485(void* data);
