

#include <stdio.h>

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
#ifdef DEBUG
	printf("******* intArrayLength=%d   reminder=%d\n", intArrayLength , reminder);
#endif
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
			return &userStoreArray[i];
		}
	}
#ifdef DEBUG
	//printf("Not find this user ID=%d\n",userID);
#endif
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
#ifdef DEBUG
	printf("Not find this user FgNum=%d\n",FgNum);
#endif
	return 0;
}






/*查找并清除botmap里面的相应位*/
user_Info_t* getuserInfoClear(char *userID)
{
	int i=0,count=0;
	for(i=0; i < MAX_USER_LENGTH ; i++)
	{
		if( userStoreArray[i].uid[0] == userID[0] && userStoreArray[i].uid[1] == userID[1]
				&& userStoreArray[i].uid[2] == userID[2] && userStoreArray[i].uid[3] == userID[3]){
			clearBit(count);
			return &userStoreArray[i];
		}
		count++;
	}
#ifdef DEBUG
	//printf("Not find this user ID=%d\n",userID);
#endif
	return 0;
}


int addUserInfo(char *userID, char *pswd)
{
	short firstUnused = -1,k=0;
	user_Info_t usi,*search;
#ifdef DEBUG	
	//printf("Add User: id=%d,pswd=%d\n",userID,password);
#endif
	search = getuserInfo(userID);
	if(!search)
	{
		firstUnused = getFirstUnusedBit();
#ifdef DEBUG
		//printf("firstUnUsedBit=%d\n",firstUnused);
#endif
		if(firstUnused < 0)
		{
#ifdef DEBUG
			printf("Store Error: Array is FULL\n");
#endif
		}
		
		for(k=0; k<4; k++)
			userStoreArray[ firstUnused ].uid[k] = userID[k];
		for(k=0; k<6; k++)
			userStoreArray[ firstUnused ].passwd[k] = pswd[k];
		return 0;
	}
	else // this userID already exit
	{
#ifdef DEBUG
		printf("Store Error: You want to add a user whose userID has exist in this array\n");
#endif
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
					printf("%d",userStoreArray[count].uid[k]);
				printf(" ,  Passwd = ");
				for(k=0;k<6;k++)
					printf("%d",userStoreArray[count].passwd[k]);
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
				printf("%d",userStoreArray[count].uid[k]);
			printf(" ,  Passwd = ");
			for(k=0;k<6;k++)
				printf("%d",userStoreArray[count].passwd[k]);
		}
		count++;
		curInt>>=1;
	}

}




/*
int main()
{
	init_bitmap();
	addUserInfo(1,111);
	addUserInfo(2,222);
	addUserInfo(3,333);
	addUserInfo(4,444);
	deleteUser(2);
	showAllUser();
	return 0;
}
*/
