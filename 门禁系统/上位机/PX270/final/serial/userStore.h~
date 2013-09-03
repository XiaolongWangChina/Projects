#ifndef __USERSTORE_H_
#define  __USERSTORE_H_


#include <stdio.h>

extern int totalUser;

/********************************************************************************************/
/*
Here is the bitmap operations
提供bitmap操作。
*/
/*
(________)    (________)       (________)
IntAry[2]  IntAry[1]  IntAry[0]
 _32bit_

*/

#define MAX_USER_LENGTH 64					//bitmap总长度。
//计算需要几个int(32位，严格来说，要检查sizeof(int)的值)。
#define reminder  (MAX_USER_LENGTH % 32)	//不足一个int的零头
#define reminder_addtional  reminder?1:0	//是否正好没有零头，是=0 否=1
#define intArrayLength  (((MAX_USER_LENGTH)/32)+(reminder_addtional))	//需要的int个数
int bitmapIntArray[ intArrayLength ];		//bitmap

//初始化bitmap
void init_bitmap()
{
	short i=0;

	printf("******* intArrayLength=%d   reminder=%d\n", intArrayLength , reminder);

	for(i=0; i < intArrayLength ;  i++)
		bitmapIntArray[i]=0;

}

//返回第一个空位的位置。0 ~ MAX_USER_LENGTH-1 , 没有空位返回-1
int getFirstUnusedBit()
{
	short i=0,j=0;
	int count = 0,curInt;
	for(i=0;i < MAX_USER_LENGTH/32 ; i++)
	{
		curInt = bitmapIntArray[i];
		for(j=0;j<32;j++)
		{
			if(curInt & 0x00000001)
			{
				count++;	
				curInt>>=1;
			}
			else
			{	
				bitmapIntArray[i] = bitmapIntArray[i] | (1<<count%32);
				return count;
			}
		}
	}

	curInt = bitmapIntArray[i];

	for( j=0 ; j < reminder ; j++)
	{
		if(curInt & 0x1)
		{
			count++;	
			curInt>>=1;
		}
		else
		{
			bitmapIntArray[i] = bitmapIntArray[i] | (1<<count%32);
			return count;
		}
	}
	return -1;
}

//清除某一位
//@index :  0~MAX_USER_LENGTH-1 ,越界则返回-1
int clearBit(int index)
{
	short addtion  = 0;
	int curInt;
	if( index >=  MAX_USER_LENGTH )
		return -1;
	bitmapIntArray[ index/32 ] &= ~(1<<(index % 32)); 
	return 0;
}
/*****************************************************************************************/



/*存储格式，该类型的数组*/
typedef struct userInfoInNum{
	char passwd[6];
	char uid[4];
	char fgNum;
} user_Info_t;


user_Info_t userStoreArray[ MAX_USER_LENGTH ];


user_Info_t* getuserInfo( char* userID)
{
	int i=0;
	for(i=0; i < MAX_USER_LENGTH ; i++)
	{
		if( userStoreArray[i].uid[0] == userID[0] && userStoreArray[i].uid[1] == userID[1] 
			&& userStoreArray[i].uid[2] == userID[2] && userStoreArray[i].uid[3] == userID[3])
		{
			printf("Search user OK!:id=%d%d%d%d\n",userID[0],userID[1],userID[2],userID[3]);
			return &userStoreArray[i];
		}
	}

	printf("Search user Fail. id=%d%d%d%d\n",userID[0],userID[1],userID[2],userID[3]);

	return 0;
}



user_Info_t* getuserIDByFgNum(char FgNum)
{
	int i=0;
	for(i=0; i < MAX_USER_LENGTH ; i++)
	{
		if( userStoreArray[i].fgNum == FgNum ){
			return &userStoreArray[i];
		}
	}

	printf("Not find this user FgNum=%d\n",FgNum);

	return 0;
}






/*查找并清除botmap里面的相应位*/
user_Info_t* getuserInfoClear(char *userID)
{
	short i=0,j=0,k=0;
	int count = 0,curInt;
	for(i=0;i < MAX_USER_LENGTH/32 ; i++)
	{
		curInt = bitmapIntArray[i];
		for(j=0;j<32;j++)
		{
			if(curInt & 0x00000001)
			{
				if( userStoreArray[count].uid[0] == userID[0] && userStoreArray[count].uid[1] == userID[1] && 						userStoreArray[count].uid[2] == userID[2] && userStoreArray[count].uid[3] == userID[3])
				{
					clearBit(count);
                                        totalUser--;
					return &userStoreArray[i];
				}
			}
			curInt>>=1;
			count++;
		}
	}
	curInt = bitmapIntArray[i];
	for( j=0 ; j < reminder ; j++)
	{
		if(curInt & 0x1)
		{
			if( userStoreArray[count].uid[0] == userID[0] && userStoreArray[count].uid[1] == userID[1] && 						userStoreArray[count].uid[2] == userID[2] && userStoreArray[count].uid[3] == userID[3])
			{
					clearBit(count);
                                        totalUser--;
					return &userStoreArray[i];
			}
		}
		count++;
		curInt>>=1;
	}
}


int addUserInfo(char *userID, char *pswd)
{
	short firstUnused = -1,k=0;
	user_Info_t usi,*search;
	
	printf("******Add User: id=%d%d%d%d,pswd=%d%d%d%d%d%d\n",userID[0],userID[1],userID[2],userID[3],pswd[0],pswd[1],pswd[2],pswd[3],pswd[4],pswd[5]);

	search = getuserInfo(userID);
	if(!search)
	{
		firstUnused = getFirstUnusedBit();

		printf("firstUnUsedBit=%d\n",firstUnused);

		if(firstUnused < 0)
		{
			printf("Store Error: Array is FULL\n");
			return -1;
		}

		for(k=0; k<4; k++)
			userStoreArray[ firstUnused ].uid[k] = userID[k];
		for(k=0; k<6; k++)
			userStoreArray[ firstUnused ].passwd[k] = pswd[k];
		userStoreArray[ firstUnused ].fgNum = 0xFF;
		totalUser++;		
		return 0;
	}
	else // this userID already exit
	{

		printf("Store Error: You want to add a user whose userID has exist in this array\n");

		return -1;
	}
	
}

int deleteUser(char *userID)
{
	getuserInfoClear(userID);
	
}


void showAllUser()
{
	short i=0,j=0,k=0;
	int count = 0,curInt;
	for(i=0;i < MAX_USER_LENGTH/32 ; i++)
	{
		curInt = bitmapIntArray[i];
		for(j=0;j<32;j++)
		{
			if(curInt & 0x00000001)
			{
				printf("<All Users:>Uid = ");
				for(k=0;k<4;k++)
					printf("%c",userStoreArray[count].uid[k]);
				printf(" ,  Passwd = ");
				for(k=0;k<6;k++)
					printf("%c",userStoreArray[count].passwd[k]);
				printf(", fgNum=%d\n",userStoreArray[count].fgNum);
			}
			curInt>>=1;
			count++;
		}
	}

	curInt = bitmapIntArray[i];

	for( j=0 ; j < reminder ; j++)
	{
		if(curInt & 0x1)
		{
			printf("<All Users:>Uid = ");
			for(k=0;k<4;k++)
				printf("%c",userStoreArray[count].uid[k]);
			printf(" ,  Passwd = ");
			for(k=0;k<6;k++)
				printf("%c",userStoreArray[count].passwd[k]);
			printf(", fgNum=%d\n",userStoreArray[count].fgNum);
		}
		count++;
		curInt>>=1;
	}

}



/*pageNum 页号  0 ~ N  */
int getAPageOfUser(int pageNum,user_Info_t loadUserArray[])
{
	int start = pageNum*5;//从start开始加载
	int loadUserCount=0,userCount = 0;
	short i=0,j=0,k=0;
	int count = 0,curInt;
	for(i=0;i < MAX_USER_LENGTH/32 ; i++)
	{
		curInt = bitmapIntArray[i];
		for(j=0;j<32;j++)
		{
			if(curInt & 0x00000001)
			{
				if(userCount >= start && userCount < start+5)
				{
					loadUserArray[ loadUserCount ] = userStoreArray[count];
					loadUserCount++;
					if( loadUserCount >= 5 )
						return loadUserCount;
				}
            userCount++;
			}
			curInt>>=1;
			count++;
		}
	}

	curInt = bitmapIntArray[i];

	for( j=0 ; j < reminder ; j++)
	{
		if(curInt & 0x1)
		{
			if(userCount >= start && userCount < start+5)
			{
					loadUserArray[ loadUserCount ] = userStoreArray[count];
					loadUserCount++;
					if( loadUserCount >= 5 )
						return loadUserCount;
			}
                        userCount++;
		}
		count++;
		curInt>>=1;
	}
	
	return loadUserCount;

}






/*

int main()
{
	init_bitmap();
	int i=0,res=0;
	char* uid1 = "1234";
	char* uid2 = "1235";
	char* uid3 = "1236";
	char* uid4 = "1237";
	char* uid5 = "1238";
	char* uid6 = "1239";
	char* uid7 = "1240";
	char* uid8 = "1241";
	char* uid9 = "1242";
	char* uid10= "1243";
	char* uid11= "1244";
	char* uid12= "1245";
	char* uid13= "1246";
	char* uid14= "1247";
	char* uid15= "1248";
	char* uid16= "1249";
	

	char* pswd1 = "987654";
	char* pswd2 = "987653";
	char* pswd3 = "987652";
	char* pswd4 = "987651";
	char* pswd5 = "987650";
	char* pswd6 = "987649";
	char* pswd7 = "987648";
	char* pswd8 = "987647";
	char* pswd9 = "987646";
	char* pswd10= "987645";
	char* pswd11= "987644";
	char* pswd12= "987643";
	char* pswd13= "987642";
	char* pswd14= "987641";
	char* pswd15= "987640";
	char* pswd16= "987639";
	
	addUserInfo(uid1,pswd1);
	addUserInfo(uid2,pswd2);
	addUserInfo(uid3,pswd3);
	addUserInfo(uid4,pswd4);
	addUserInfo(uid5,pswd5);
	addUserInfo(uid6,pswd6);
	addUserInfo(uid7,pswd7);
	addUserInfo(uid8,pswd8);
	addUserInfo(uid9,pswd9);
	addUserInfo(uid10,pswd10);
	addUserInfo(uid11,pswd11);
	addUserInfo(uid12,pswd12);
	addUserInfo(uid13,pswd13);
	addUserInfo(uid14,pswd14);
	addUserInfo(uid15,pswd15);
	addUserInfo(uid16,pswd16);
	

	deleteUser("1244");

	user_Info_t page[5];

	res = getAPageOfUser(2,page);
	printf("page_2 = %d ______________________________\n",res);
	for(i=0;i<5;i++)
		printf("id=%c%c%c%c pswd=%c%c%c%c%c%c fg=%d\n",page[i].uid[0],page[i].uid[1],page[i].uid[2],page[i].uid[3],page	[i].passwd[0],page[i].passwd[1],page[i].passwd[2],page[i].passwd[3],page[i].passwd[4],page[i].passwd[5],page[i].fgNum);



	printf("page_1  ______________________________\n");
	getAPageOfUser(1,page);
	for(i=0;i<5;i++)
		printf("id=%c%c%c%c pswd=%c%c%c%c%c%c fg=%d\n",page[i].uid[0],page[i].uid[1],page[i].uid[2],page[i].uid[3],page[i].passwd[0],page[i].passwd[1],page[i].passwd[2],page[i].passwd[3],page[i].passwd[4],page[i].passwd[5],page[i].fgNum);


	printf("page_2  ______________________________\n");
	getAPageOfUser(2,page);
	for(i=0;i<5;i++)
		printf("id=%c%c%c%c pswd=%c%c%c%c%c%c fg=%d\n",page[i].uid[0],page[i].uid[1],page[i].uid[2],page[i].uid[3],page[i].passwd[0],page[i].passwd[1],page[i].passwd[2],page[i].passwd[3],page[i].passwd[4],page[i].passwd[5],page[i].fgNum);


	printf("page_3  ______________________________\n");
	getAPageOfUser(3,page);
	for(i=0;i<5;i++)
		printf("id=%c%c%c%c pswd=%c%c%c%c%c%c fg=%d\n",page[i].uid[0],page[i].uid[1],page[i].uid[2],page[i].uid[3],page[i].passwd[0],page[i].passwd[1],page[i].passwd[2],page[i].passwd[3],page[i].passwd[4],page[i].passwd[5],page[i].fgNum);
	

	printf("page_4  ______________________________\n");
	getAPageOfUser(4,page);
	for(i=0;i<5;i++)
		printf("id=%c%c%c%c pswd=%c%c%c%c%c%c fg=%d\n",page[i].uid[0],page[i].uid[1],page[i].uid[2],page[i].uid[3],page[i].passwd[0],page[i].passwd[1],page[i].passwd[2],page[i].passwd[3],page[i].passwd[4],page[i].passwd[5],page[i].fgNum);

	//deleteUser("1235");

	showAllUser();
	return 0;
}
*/


#endif
