
/**
��485����ͨ�š�
��λ����������������
		1.��ѯ�ŵ�״̬������λ���лظ���
		2.���ÿ���������λ���лظ���
�������յ����󣨴�������Ҫ�ظ���
		1.��λ������������֤����λ��Ҫ�ظ���
		2.��λ������ָ�ƺţ���λ��Ҫ�ظ���
		3.��λ����֪����֤ʧ�ܵ��������λ���޻ظ���
		4.ָ����֤ͨ������λ����֪��λ��������λ��Ҫ�ظ���
		5.��λ�������û��ź������룬Ҫ���޸����롣����λ���ظ��޸Ľ����
*/

#include "myTty.h"
#include "Serial485.h"
#include "doParse.h"
#include "pthread.h"

#define BytesPerFrame  11  //һ֡��11�ֽ�

#define DECODE_ERROR_HEAD_UNEXPECT 1			//b[0] != 0xBB
#define DECODE_ERROR_SECONDHEAD_UNEXPECT 2	//b[1] != 0xFF
#define DECODE_ERROR_LENGTH_ERROR 3				//b[2] != 0x07
#define DECODE_ERROR_NOT_HOST_TARGET 4			//b[3] != 0X03
#define DECODE_ERROR_TAIL_UNEXPECT 5			//b[10] != 0XFF

/*��ѯ�ŵ�״̬*/
void getStateOfDoor()
{

#ifdef DEBUG
printf("Host want to get the status of door.\nNow send command\n");
#endif

	char getStateCmd[11];
	
	getStateCmd[0]  = 0xAA;
	getStateCmd[1]  = 0xFF;
	getStateCmd[2]  = 0x07;
	getStateCmd[3]  = 0x03;
	getStateCmd[4]  = 0xA7;
	getStateCmd[5]  = 0x00;
	getStateCmd[6]  = 0x00;
	getStateCmd[7]  = 0x00;
	getStateCmd[8]  = 0x00;
	getStateCmd[9]  = 0x00;
	getStateCmd[10] = 0xFF;
	
	tty_write(getStateCmd,11);
}

/*������Ϊ��*/
int setLockOpen()
{
#ifdef DEBUG
printf("Host want to set the lock open.\n");
#endif

	char openLockCmd[11];
	
	openLockCmd[0]  = 0xAA;
	openLockCmd[1]  = 0xFF;
	openLockCmd[2]  = 0x07;
	openLockCmd[3]  = 0x03;
	openLockCmd[4]  = 0xB7;
	openLockCmd[5]  = 0x00;
	openLockCmd[6]  = 0x00;
	openLockCmd[7]  = 0x00;
	openLockCmd[8]  = 0x00;
	openLockCmd[9]  = 0x00;
	openLockCmd[10] = 0xFF;
	
	tty_write(openLockCmd,11);
}

/*
	������һ֡
	Э����Լ������Э���ĵ���Э��-final.doc��
*/

int parseFrame(unsigned char *buffer)
{
	unsigned char cmd,param;
	int userID,password;
	int FgNum;
	
#ifdef DEBUG
	int i=0;
	printf("+++Now Parsing Frame:\n");
	for(i=0;i<11;i++)
		printf("[%d]=0x%x",i,buffer[i]);
	printf("\n");
#endif

	if((unsigned char)buffer[0] != 0xBB)
		return DECODE_ERROR_HEAD_UNEXPECT;
	if((unsigned char)buffer[1] != 0XFF)
		return DECODE_ERROR_SECONDHEAD_UNEXPECT;
	if((unsigned char)buffer[2] != 0X07)
		return DECODE_ERROR_LENGTH_ERROR;
	if((unsigned char)buffer[3] != 0x00)//0x00��ʾ�Ƿ������صġ������صĻ�����Ϊ00��
		return DECODE_ERROR_NOT_HOST_TARGET;
	if((unsigned char)buffer[10] != 0xFF)
		return DECODE_ERROR_TAIL_UNEXPECT;
	
	//here the head is right & the tail is OK! --- get cmd 
	cmd = buffer[4];

#ifdef DEBUG
	printf("Parse cmd=0x%x\n",cmd);
#endif

	switch(cmd)
	{
	/*	�����ŵ�״̬
		BB FF 07 00 A7 00 00 00 00 00 FF(��)
	   BB FF 07 00 A7 01 00 00 00 00 FF(��)*/
	case 0xA7:
		if(buffer[5] == 0x00 )
		{
			showDoorStateOpened();//֪ͨ���� �ŵ�״̬�ǿ��ŵġ�			
		}
		else if(buffer[5] == 0x01 )
		{
			showDoorStateClosed();//֪ͨ���� �ŵ�״̬�ǹ��ŵġ�
		}
		else
		{
			showDoorStateUnknown();//֪ͨ���� �ŵ�״̬��δ֪�ġ����쳣�����
		}
		break;

	case 0xB7:
		/*	���ؿ����Ľ��
			BB FF 07 00 B7 00 00 00 00 00 FF  (�����ɹ�)
			BB FF 07 00 B7 01 00 00 00 00 FF ������ʧ�ܣ�*/
		if(buffer[5] == 0x00 )
		{
			showLockOpenSuccess();//֪ͨ���濪���ɹ�			
		}
		else if(buffer[5] == 0x01 )
		{
			showLockOpenFail();//֪ͨ���濪��ʧ��
		}
		else
		{
			showLockOpenUnknown();//֪ͨ���濪�����δ֪�ġ����쳣�����
		}
		break;

	case 0xCC:
		/*	������֤����
			BB FF 07 00 CC   00 80    06 25 37  FF(5��6�û�����7��8��9����)*/
		userID = MAKE_USER_ID(buffer[5] , buffer[6]);
		password = MAKE_USER_PASSWORD(buffer[7] , buffer[8] , buffer[9]);
		//RepalyPswdAuth(checkPassword(userID,password));
		RepalyPswdAuth(userID,password);
		break;

/**********************  ??????  ***********************************/
	/*
		ָ���޸ģ�����ָ�ƺţ�
		BB FF 07 00 CE 00 23 09 00 00 FF��5��6Ϊ�û�����7Ϊָ����ţ�
		���أ�
		AA FF 07 03 CE 00 00 00 00 00 FF��5��ʾ�ɹ���
		AA FF 07 03 CE 01 00 00 00 00 FF��5��ʾʧ�ܣ�
	*/
	case 0xCE:
		/*
		Some Code Here!
		*/
		break;
/*******************************************************************/
	/*	ָ����֤ʧ�ܶ�Σ���λ�������¼���֪��λ��������λ������
		BB FF 07 00 CF 00 00 00 00 00 FF*/
	case 0xCF:
		AlertFGFail();
		break;

	/*	ָ����֤ͨ������λ����֪��λ���������ڱ����¼�ȣ�
		BB FF 07 00 DA 08 00 00 00 00 FF      ( 5��ʾָ�ƺ�)
		AA FF 07 03 DA 00 23 00 00 00 FF     ��5��6 �û��ţ� */
	case 0xDA:
		FgNum = buffer[5];
		ReplyFGAuthed(FgNum);//��Ӧ��ָ����֤ͨ���¼���
		break;
	
	/*	�����޸ģ�
		BB FF 07 00 DB 00 23 12 34 56 FF��0023���û��� ��  12 34 56�������룩
		AA FF 07 03 DB 00 00 00 00 FF���ɹ���
		AA FF 07 03 DB 01 00 00 00 FF��ʧ�ܣ�  */
	case 0xDB:
		userID = MAKE_USER_ID(buffer[5] , buffer[6] );
		password = MAKE_USER_PASSWORD(buffer[5] , buffer[6] , buffer[7]);
		ModifyPasswd(userID , password);
		break;
	default:
		#ifdef DEBUG
			printf("Unknown Command cmd = %d\n",cmd);
		#endif
	/*
		Do nothing .
		Alert to Qt.
	*/
	break;
	}

	return 0;

}


#ifdef DEBUG
int k=0,j=0,f=0;
#endif

/*
	����485�ӿڵ�����
	�������ݣ�����������ա�
	Ӧ��Ϊһ�������߳�
*/
void* ListenSerial485(void* data)
{

	tty_init();

	unsigned char  frameBuffer[12];	
	unsigned char  tempBuffer[12];
	unsigned char  receiveFrame = 0;	//�Ƿ��յ�һ֡
	unsigned char  count = 0;			//���յ���ǰ֡�ļ����ֽ�
	unsigned char  parseRes = -1;
	while(1){
		printf("Try reading:\n");
		while(tty_read( &frameBuffer[count] , 1 ) < 0)
		{
			#ifdef DEBUG
			
			#endif
		}

		#ifdef DEBUG
			printf("\nGet 0x%x from tty ,Now count=%d\n",frameBuffer[count],count+1);
		#endif

		count++;

		if(count >= BytesPerFrame)
		{
			#ifdef DEBUG
			printf("< Frame_%d >Receive A Frame!!\n",k++);
			#endif
			//���յ�һ֡��
			count = 0;
			receiveFrame = 1;
			parseRes = parseFrame(frameBuffer);
			printf("> > > > > parse return : %d\n",parseRes);
			//�Ƿ�Ҫ˯��һ��,�Եȴ�������ɡ����ʡ�Ϊ��ֹ���������ǣ�����Ҫ���Ƶ���һ��������
			receiveFrame = 0;
		}
	}
	
	tty_end();
	return NULL;

}




void* hostDoto485(void* data)
{
	int cmd;
	int inputCnt=0;
	int sid,pswd;

	while(1){

		printf("[Cmd]Please select one command:\n");
		printf("<1>Get the state of Door.\n");
		printf("<2>Unlock the lock.\n");	
		printf("<3>Add new user.\n");	
		printf("<4>Delete user.\n");	
		printf("<5>Show All users.\n");	
		printf("<6>Print Parse result table.\n");	
		
		scanf("%d",&cmd);

	#ifdef DEBUG
		printf("Your input cmd is %d\n",cmd);
	#endif

		switch(cmd)
		{	
			case 1:
				getStateOfDoor();	
			break;
			
			case 2:
				setLockOpen();
			break;
			
			case 3:
				scanf("%d",&sid);
				scanf("%d",&pswd);
				addUserInfo(sid,pswd);
			break;
	
			case 4:
				scanf("%d",&sid);
				deleteUser(sid);
			break;

			case 5:
				showAllUser();
			break;

			case 6:
				printf("Parse result table :\n");
				printf("  0 - parse OK !\n");
				printf("  1 - unexpected head(0xBB).\n");
				printf("  2 - unexpected second head(0xFF).\n");
				printf("  3 - length error(0x07 , Not fetal).\n");
				printf("  4 - not from target(0x03 , door expected).\n");
				printf("  5 - unexpected tail(not 0xFF).\n");
			break;
		}
	}
	return NULL;
}




int main(int argc,char** argv)
{

  	pthread_t listen485Thread, keyThread;
	void * retval;

#ifdef DEBUG
printf("Threads create ...\n");
#endif

 	pthread_create(&listen485Thread, NULL, ListenSerial485, 0);
  	pthread_create(&keyThread, NULL, hostDoto485, 0);
  
  	pthread_join(listen485Thread, &retval);
  	pthread_join(keyThread, &retval);

	return 0;
}







