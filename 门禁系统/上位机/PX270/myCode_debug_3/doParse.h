




/*
将拆分后的整数拼成原来的数。
例如，0x4d2 -> 1234
< need to be tested !!!!!!!!!!! >
*/
#define MAKE_USER_ID(ch1,ch2)	 ((((short)ch1)<<8) + ((short)ch2))

#define MAKE_USER_PASSWORD(ch1,ch2,ch3)  	((((int)ch1)<<16) +(((int)ch2)<<8) +(((int)ch3)))


/*
	对下位机发来的消息的解析中用到的处理函数
	在此声明。
*/


void showDoorStateOpened();//通知主界面 门的状态是开着的。

void showDoorStateClosed();//通知主界面 门的状态是关着的。

void showDoorStateUnknown();//通知主界面 门的状态是未知的。（异常情况）

void showLockOpenSuccess();//通知主界面 开锁成功

void showLockOpenFail();//通知主界面 开锁失败

void showLockOpenUnknown();//通知主界面开锁结果未知的。（异常情况）



/*
	上位机向下位机发送密码验证结果的命令。（即上位机回应下位机的密码验证请求）
	参数： 
		@res - 上位机验证后产生的验证结果
				（0-验证通过。 1 - 用户ID存在但密码不正确。	2 - 用户ID不存在。）		
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
void RepalyPswdAuth(int userID , int password);


/*
	指纹验证失败，下位机给上位机通知，这里处理该通知
	报警。(主控界面 报警)
*/
void AlertFGFail();




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
int ReplyFGAuthed(int FgNum);


/*
用户修改密码：
参数：
@userID:用户号
@newPasswd:新密码。
*/
void ModifyPasswd(int userID , int newPasswd);


/*
从485接收的指令含有未知命令。
(主控界面 报警)
*/

void AlertDecodeFail();

/*add new user*/
void AddNewUser(char uid[],char pswd[]);


