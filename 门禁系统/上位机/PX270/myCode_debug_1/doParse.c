#include <stdio.h>
#include "doParse.h"
#include "userStore.h"

/*
	上位机向下位机发送密码验证结果的命令。（即上位机回应下位机的密码验证请求）
	//参数： 
		//@res - 上位机验证后产生的验证结果
		//		（0-验证通过。 1 - 用户ID存在但密码不正确。	2 - 用户ID不存在。）		
	实现：
		首先去密码存储中查找用户名，若找到，则取出密码和指纹号（可能要用）。
		若密码验证正确。回复下位机：AA FF 07 03 CD 00 00 00 00 00 FF(正确，6为指纹序号)
		否则回复：AA FF 07 03 CD 01 00 00 00 00 FF(错误)
-------------------------------------------------------------------	
参照协议
	密码验证结果：
	AA FF 07 03 CD 00 00 00 00 00 FF(正确，6为指纹序号)
	AA FF 07 03 CD 01 00 00 00 00 FF(错误)
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
	if(user)//已找到这个用户信息
	{
		if( user->passwd == password )//密码验证通过
		{
			char fgNum = user->fgNum;
			char replyPswdOk[11]={0xAA,0xFF,0x07,0x03,0xCD,0x00,fgNum,0x00,0x00,0x00,0xFF};
			tty_write(replyPswdOk,11);
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
	char replyPswdFail[11]={0xAA,0xFF,0x07,0x03,0xCD,0x01,0x00,0x00,0x00,0x00,0xFF};
	tty_write(replyPswdFail,11);
	return;

}


/*
密码修改。
*/
void ModifyPasswd(int userID , int newPasswd)
{



	#ifdef DEBUG
		printf("------------------------\n");
		printf("<*** in doParse ****>UserID=%D change password. New = %d\n",userID, newPasswd);
		printf("------------------------\n");
	#endif

	user_Info_Store_t *user = getuserInfo(userID);
	if(user)//已找到这个用户信息
	{
		user->passwd = newPasswd;
	}
	else
	{
	// alert modify paswprd failed.
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
int ReplyFGAuthed(unsigned char FgNum)
{
/*
有待完善。
*/
	#ifdef DEBUG
	printf("------------------------\n");
	printf("<*** in doParse ****>ReplyFGAuthed Linux Recceive a message: There is a fg authed event.fg=%d\n",FgNum);
	printf("------------------------\n");
	#endif

}




/*
-下位机的用户录入指纹或修改指纹。
	下位机发来用户号和指纹号。

-上位机操作：
	查询用户信息数组中的该对应用户号，将其指纹号改为新指纹号。
	返回：
	AA FF 07 03 CE 00 00 00 00 00 FF（5表示成功）
	AA FF 07 03 CE 01 00 00 00 00 FF（5表示失败）

*/
void ModifyFgNum(int userID, unsigned char fgNum)
{
#ifdef DEBUG 
	printf("------------------------\n");
	printf("<*** in doParse ****>ModifyFgNum(id = %d  fgNum=%d)\n",userID , fgNum);
	printf("------------------------\n");
#endif

	user_Info_Store_t *user = getuserInfo(userID);
	if(user)//已找到这个用户信息
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
	user_Info_t newUser;
	user_Info_Store_t ustore;
	
	sprintf(newUser.uid,"%s",uid);
	sprintf(newUser.passwd,"%s",pswd);
	newUser.fgNum=0x00;
	
	ustore = showStyleToStoreStyle(newUser);
	
	
	addUserInfo(ustore.uid , ustore.passwd);

}



