#include <stdio.h>
#include "doParse.h"
#include "userStore.h"



//判断两个密码是否相同，若相同返回1，不同返回0
unsigned char passwdEqual(char *pswd1,char *pswd2)
{
	int i=0;
	for(i=0;i<6;i++)
		if(pswd1[i] != pswd2[i])
			return 0;
	return 1;
}

//判断两个用户名是否相同，若相同返回1，不同返回0
unsigned char uidEqual(char *uid1,char *uid2)
{
	int i=0;
	for(i=0;i<4;i++)
		if(uid1[i] != uid2[i])
			return 0;
	return 1;
}



/*
	上位机向下位机发送密码验证结果的命令。（即上位机回应下位机的密码验证请求）
	//参数： 
		//@res - 上位机验证后产生的验证结果
		//		（0-验证通过。 1 - 用户ID存在但密码不正确。	2 - 用户ID不存在。）		
	实现：
		首先去密码存储中查找用户名，若找到，则取出密码和指纹号（可能要用）。
-------------------------------------------------------------------	
参照协议
	密码验证请求：
		BB FF 07 00 CC 00 00 00 00 00 00 00 00 00 00 FF(5-8用户名，9-14密码)
	密码验证结果：
		AA FF 07 03 CD 00 00 00 00 00 00 00 00 00 00 FF(正确，6为指纹序号)
		AA FF 07 03 CD 01 00 00 00 00 00 00 00 00 00 FF(错误)	
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
	if(user)//已找到这个用户信息
	{
		if(passwdEqual(user->passwd,password))//密码验证通过
		{
			char fgNum = user->fgNum;
			char replyPswdOk[16]={0xAA,0xFF,0x07,0x03,0xCD,0x00,fgNum,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
			tty_write(replyPswdOk,16);
			return;
		}
		//else//用户号存在，但密码不对。
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
密码修改。
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
	if(user)//已找到这个用户信息
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
	记录已通过指纹验证的事件。
	下位机在有人通过指纹验证后，将该指纹的指纹号发给上位机。
	上位机接收到指纹号，可以进行保存和其他操作。
	参数：
		@FgNum 下位机发来的指纹号
	返回值：
		对应于该指纹号的用户号。
		-1 表示没有对应的用户号（异常）。
	实现：
		在存储中根据指纹号查找用户号，（将该指纹通过验证时间存至历史记录中---optional）
		回复下位机用户号。AA FF 07 03 DA 00 23 00 00 00 FF     （5、6 用户号）
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
		//没有这个指纹号
	
		return ;
	}

	char replyFgAuthed[16]={0xAA,0xFF,0x07,0x03,0xDA,user->uid[0],user->uid[1],user->uid[2],user->uid[3],0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	tty_write(replyFgAuthed,16);

}




/*
-下位机的用户录入指纹或修改指纹。
	下位机发来用户号和指纹号。
	
-上位机操作：
	查询用户信息数组中的该对应用户号，将其指纹号改为新指纹号。
	不回复下位机。
*/
void ModifyFgNum(char* userID, char fgNum)
{
#ifdef DEBUG 
	printf("------------------------\n");
	printf("<*** in doParse ****>ModifyFgNum(id = %d%d%d%d  fgNum=%d)\n",userID[0],userID[1],userID[2],userID[3] , fgNum);
	printf("------------------------\n");
#endif

	user_Info_t *user = getuserInfo(userID);
	if(user)//已找到这个用户信息
	{
		user->fgNum = fgNum;
	}else{
		//没找到这个用户号。
	}
	
}


void showDoorStateOpened()
{
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Door State Opened! This will ALERT in QT MainGUIApp\n");
	printf("------------------------\n");
	#endif
}//通知主界面 门的状态是开着的。

void showDoorStateClosed(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Door State Closed!\n");
	printf("------------------------\n");
	#endif

}//通知主界面 门的状态是关着的。

void showDoorStateUnknown(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Door State Unknown!\n");
	printf("------------------------\n");
	#endif

}//通知主界面 门的状态是未知的。（异常情况）

void showLockOpenSuccess(){

	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>show Lock Open Success\n");
	printf("------------------------\n");
	#endif

}//通知主界面 开锁成功

void showLockOpenFail(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>showLockOpenFail\n");
	printf("------------------------\n");
	#endif
}//通知主界面 开锁失败

void showLockOpenUnknown(){
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>showLockOpenUnknown\n");
	printf("------------------------\n");
	#endif
}//通知主界面开锁结果未知的。（异常情况）


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



