
typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;

/*��ѯ�ŵ�״̬*/
void getStateOfDoor();

/*������Ϊ��*/
int setLockOpen();

/*����485�ӿڵ�����---�߳�*/
void* ListenSerial485(void* data);

void* hostDoto485(void* data);
