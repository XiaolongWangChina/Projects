#include <stdio.h>
#include "doParse.h"
#include "userStore.h"

/*
	��λ������λ������������֤��������������λ����Ӧ��λ����������֤����
	//������ 
		//@res - ��λ����֤���������֤���
		//		��0-��֤ͨ���� 1 - �û�ID���ڵ����벻��ȷ��	2 - �û�ID�����ڡ���		
	ʵ�֣�
		����ȥ����洢�в����û��������ҵ�����ȡ�������ָ�ƺţ�����Ҫ�ã���
		��������֤��ȷ���ظ���λ����AA FF 07 03 CD 00 00 00 00 00 FF(��ȷ��6Ϊָ�����)
		����ظ���AA FF 07 03 CD 01 00 00 00 00 FF(����)
-------------------------------------------------------------------	
����Э��
	������֤�����
	AA FF 07 03 CD 00 00 00 00 00 FF(��ȷ��6Ϊָ�����)
	AA FF 07 03 CD 01 00 00 00 00 FF(����)
-------------------------------------------------------------------	
*/
void RepalyPswdAuth(int userID , int password)
{


#ifdef DEBUG 
	printf("------------------------\n");
	printf("<*** in doParse ****>ReplyPswdAuth(id = %d  pswd=%d)\n",userID , password);
	printf("------------------------\n");
#endif


	user_Info_Store_t *user = getuserInfo(userID);
	if(user)//���ҵ�����û���Ϣ
	{
		if( user->passwd == password )//������֤ͨ��
		{
			char fgNum = user->fgNum;
			char replyPswdOk[11]={0xAA,0xFF,0x07,0x03,0xCD,0x00,fgNum,0x00,0x00,0x00,0xFF};
			tty_write(replyPswdOk,11);
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
	char replyPswdFail[11]={0xAA,0xFF,0x07,0x03,0xCD,0x01,0x00,0x00,0x00,0x00,0xFF};
	tty_write(replyPswdFail,11);
	return;

}


/*
�����޸ġ�
*/
void ModifyPasswd(int userID , int newPasswd)
{



	#ifdef DEBUG
		printf("------------------------\n");
		printf("<*** in doParse ****>UserID=%D change password. New = %d\n",userID, newPasswd);
		printf("------------------------\n");
	#endif

	user_Info_Store_t *user = getuserInfo(userID);
	if(user)//���ҵ�����û���Ϣ
	{
		user->passwd = newPasswd;
	}
	else
	{
	// alert modify paswprd failed.
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
int ReplyFGAuthed(unsigned char FgNum)
{
/*
�д����ơ�
*/
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>ReplyFGAuthed Linux Recceive a message: There is a fg authed event.fg=%d\n",FgNum);
	printf("------------------------\n");
	#endif

}




/*
-��λ�����û�¼��ָ�ƻ��޸�ָ�ơ�
	��λ�������û��ź�ָ�ƺš�

-��λ��������
	��ѯ�û���Ϣ�����еĸö�Ӧ�û��ţ�����ָ�ƺŸ�Ϊ��ָ�ƺš�
	���أ�
	AA FF 07 03 CE 00 00 00 00 00 FF��5��ʾ�ɹ���
	AA FF 07 03 CE 01 00 00 00 00 FF��5��ʾʧ�ܣ�

*/
void ModifyFgNum(int userID, unsigned char fgNum)
{
#ifdef DEBUG 
	printf("------------------------\n");
	printf("<*** in doParse ****>ModifyFgNum(id = %d  fgNum=%d)\n",userID , fgNum);
	printf("------------------------\n");
#endif

	user_Info_Store_t *user = getuserInfo(userID);
	if(user)//���ҵ�����û���Ϣ
	{
		user->fgNum = fgNum;
		char replyModifyFgOk[11]={0xAA,0xFF,0x07,0x03,0xCE,0x00,0X00,0x00,0x00,0x00,0xFF};
		tty_write(replyModifyFgOk,11);
		return;
	}
	char replyModifyFgFail[11]={0xAA,0xFF,0x07,0x03,0xCE,0x01,0x00,0x00,0x00,0x00,0xFF};
	tty_write(replyModifyFgFail,11);
	return;
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
	user_Info_t newUser;
	user_Info_Store_t ustore;
	
	sprintf(newUser.uid,"%s",uid);
	sprintf(newUser.passwd,"%s",pswd);
	newUser.fgNum=0x00;
	
	ustore = showStyleToStoreStyle(newUser);
	
	
	addUserInfo(ustore.uid , ustore.passwd);

}



