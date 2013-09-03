#include <stdio.h>
#include "doParse.h"
#include "userStore.h"



//�ж����������Ƿ���ͬ������ͬ����1����ͬ����0
unsigned char passwdEqual(char *pswd1,char *pswd2)
{
	int i=0;
	for(i=0;i<6;i++)
		if(pswd1[i] != pswd2[i])
			return 0;
	return 1;
}

//�ж������û����Ƿ���ͬ������ͬ����1����ͬ����0
unsigned char uidEqual(char *uid1,char *uid2)
{
	int i=0;
	for(i=0;i<4;i++)
		if(uid1[i] != uid2[i])
			return 0;
	return 1;
}



/*
	��λ������λ������������֤��������������λ����Ӧ��λ����������֤����
	//������ 
		//@res - ��λ����֤���������֤���
		//		��0-��֤ͨ���� 1 - �û�ID���ڵ����벻��ȷ��	2 - �û�ID�����ڡ���		
	ʵ�֣�
		����ȥ����洢�в����û��������ҵ�����ȡ�������ָ�ƺţ�����Ҫ�ã���
-------------------------------------------------------------------	
����Э��
	������֤����
		BB FF 07 00 CC 00 00 00 00 00 00 00 00 00 00 FF(5-8�û�����9-14����)
	������֤�����
		AA FF 07 03 CD 00 00 00 00 00 00 00 00 00 00 FF(��ȷ��6Ϊָ�����)
		AA FF 07 03 CD 01 00 00 00 00 00 00 00 00 00 FF(����)	
-------------------------------------------------------------------	
*/
void RepalyPswdAuth(char* userID , char* password)
{

#ifdef DEBUG 
	printf("------------------------\n");
	printf("<*** in doParse ****>ReplyPswdAuth(id = %d%d%d%d  pswd=%d%d%d%d%d%d)\n",userID[0],userID[1],userID[2],userID[3], password[0], password[1], password[2], password[3], password[4], password[5]);
	printf("------------------------\n");
#endif

	user_Info_t *user = getuserInfo(userID);
	if(user)//���ҵ�����û���Ϣ
	{
		if(passwdEqual(user->passwd,password))//������֤ͨ��
		{
			char fgNum = user->fgNum;
			char replyPswdOk[16]={0xAA,0xFF,0x07,0x03,0xCD,0x00,fgNum,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
			tty_write(replyPswdOk,16);
			return;
		}
		//else//�û��Ŵ��ڣ������벻�ԡ�
		//{
			//do nothing.
		//}
	}
	//else//No such user.
	//{
	//}
	char replyPswdFail[16]={0xAA,0xFF,0x07,0x03,0xCD,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	tty_write(replyPswdFail,16);
	return;

}


/*
�����޸ġ�
*/
void ModifyPasswd(char* userID , char* newPasswd)
{

	int k=0;
	#ifdef DEBUG
		printf("------------------------\n");
		printf("<*** in doParse ****>UserID=%d%d%d%d change password. new pswd=%d%d%d%d%d%d\n",userID[0],userID[1],userID[2],userID[3], newPasswd[0], newPasswd[1], newPasswd[2], newPasswd[3], newPasswd[4], newPasswd[5]);
		printf("------------------------\n");
	#endif

	user_Info_t *user = getuserInfo(userID);
	if(user)//���ҵ�����û���Ϣ
	{
		for(k=0;k<6;k++)
			user->passwd[k] = newPasswd[k];
		char modifyPswdOK[16]={0xAA,0xFF,0x07,0x03,0xDB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
		tty_write(modifyPswdOK,16);
	}
	else
	{
	// alert modify paswprd failed.
		char modifyPswdFail[16]={0xAA,0xFF,0x07,0x03,0xDB,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
		tty_write(modifyPswdFail,16);
	}
}





/*
	��¼��ͨ��ָ����֤���¼���
	��λ��������ͨ��ָ����֤�󣬽���ָ�Ƶ�ָ�ƺŷ�����λ����
	��λ�����յ�ָ�ƺţ����Խ��б��������������
	������
		@FgNum ��λ��������ָ�ƺ�
	����ֵ��
		��Ӧ�ڸ�ָ�ƺŵ��û��š�
		-1 ��ʾû�ж�Ӧ���û��ţ��쳣����
	ʵ�֣�
		�ڴ洢�и���ָ�ƺŲ����û��ţ�������ָ��ͨ����֤ʱ�������ʷ��¼��---optional��
		�ظ���λ���û��š�AA FF 07 03 DA 00 23 00 00 00 FF     ��5��6 �û��ţ�
*/
int ReplyFGAuthed(char FgNum)
{
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>ReplyFGAuthed Linux Recceive a message: There is a fg authed event.fg=%d\n",FgNum);
	printf("------------------------\n");
	#endif
	
	user_Info_t *user = getuserIDByFgNum(FgNum);
	
	if(!user){
		//û�����ָ�ƺ�
	
		return ;
	}

	char replyFgAuthed[16]={0xAA,0xFF,0x07,0x03,0xDA,user->uid[0],user->uid[1],user->uid[2],user->uid[3],0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	tty_write(replyFgAuthed,16);

}




/*
-��λ�����û�¼��ָ�ƻ��޸�ָ�ơ�
	��λ�������û��ź�ָ�ƺš�
	
-��λ��������
	��ѯ�û���Ϣ�����еĸö�Ӧ�û��ţ�����ָ�ƺŸ�Ϊ��ָ�ƺš�
	���ظ���λ����
*/
void ModifyFgNum(char* userID, char fgNum)
{
#ifdef DEBUG 
	printf("------------------------\n");
	printf("<*** in doParse ****>ModifyFgNum(id = %d%d%d%d  fgNum=%d)\n",userID[0],userID[1],userID[2],userID[3] , fgNum);
	printf("------------------------\n");
#endif

	user_Info_t *user = getuserInfo(userID);
	if(user)//���ҵ�����û���Ϣ
	{
		user->fgNum = fgNum;
	}else{
		//û�ҵ�����û��š�
	}
	
}


void showDoorStateOpened()
{
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Door State Opened! This will ALERT in QT MainGUIApp\n");
	printf("------------------------\n");
	#endif
}//֪ͨ������ �ŵ�״̬�ǿ��ŵġ�

void showDoorStateClosed(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Door State Closed!\n");
	printf("------------------------\n");
	#endif

}//֪ͨ������ �ŵ�״̬�ǹ��ŵġ�

void showDoorStateUnknown(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Door State Unknown!\n");
	printf("------------------------\n");
	#endif

}//֪ͨ������ �ŵ�״̬��δ֪�ġ����쳣�����

void showLockOpenSuccess(){

	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Lock Open Success\n");
	printf("------------------------\n");
	#endif

}//֪ͨ������ �����ɹ�

void showLockOpenFail(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>showLockOpenFail\n");
	printf("------------------------\n");
	#endif
}//֪ͨ������ ����ʧ��

void showLockOpenUnknown(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>showLockOpenUnknown\n");
	printf("------------------------\n");
	#endif
}//֪ͨ�����濪�����δ֪�ġ����쳣�����


void AlertFGFail(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>AlertFGFail\n");
	printf("------------------------\n");
	#endif
}




/*add new user*/
void AddNewUser(char uid[],char pswd[])
{
	addUserInfo(uid ,pswd);
}



