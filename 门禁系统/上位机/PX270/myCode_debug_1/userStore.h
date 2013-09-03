

#include <stdio.h>

/*****************************************************************************************/
/*Here is the bitmap operations*/

/*

(________)    (________)       (________)
IntAry[2]  IntAry[1]  IntAry[0]
 _32bit_


*/


#define MAX_USER_LENGTH 64

#define reminder  (MAX_USER_LENGTH % 32)
#define reminder_addtional  reminder?1:0
#define intArrayLength  (((MAX_USER_LENGTH)/32)+(reminder_addtional))
int bitmapIntArray[ intArrayLength ];

void init_bitmap()
{
	short i=0;

	printf("******* intArrayLength=%d   reminder=%d\n", intArrayLength , reminder);

	for(i=0; i < intArrayLength ;  i++)
		bitmapIntArray[i]=0;

}

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

int clearBit(int index)
{
	short addtion  = 0;
	int curInt;
	if( index >=  MAX_USER_LENGTH )
		return -1;
	//if(index % 32 == 0)
	//	addtion = 1;
	//else
	//	addtion = 0;
	bitmapIntArray[ index/32 ] &= ~(1<<(index % 32)); 
}
/*****************************************************************************************/





/*显示格式*/
typedef struct userInfo{
	char passwd[6];
	char uid[4];
	char fgNum;
} user_Info_t;


/*存储格式，该类型的数组*/
typedef struct userInfoInNum{
	int 	passwd;
	short 	uid;
	char 	fgNum;
} user_Info_Store_t;


user_Info_Store_t userStoreArray[ MAX_USER_LENGTH ];

/*可以考虑使用直接映射，暂时先扫描数组查找*/
user_Info_Store_t* getuserInfo(int userID)
{
	int i=0;
	for(i=0; i < MAX_USER_LENGTH ; i++)
	{
		if( userStoreArray[i].uid == userID ){
			return &userStoreArray[i];
		}
	}
	printf("Not find this user ID=%d\n",userID);
	return 0;
}

/*查找并清除botmap里面的相应位*/
user_Info_Store_t* getuserInfoClear(int userID)
{
	int i=0,count=0;
	for(i=0; i < MAX_USER_LENGTH ; i++)
	{
		//printf("userStoreArray[i].uid (%d)== userID(%d)\n ",userStoreArray[i].uid,userID);
		if( userStoreArray[i].uid == userID ){
			printf("clear bit(%d)\n",count);
			clearBit(count);
			
			return &userStoreArray[i];
		}
		count++;
	}
	printf("Not find this user ID=%d\n",userID);
	return 0;
}


/*存储格式与显示格式之间的互相转化:*/

/*存储格式->显示格式*/
user_Info_t StoreStyleToshowStyle(user_Info_Store_t ustore)
{
	user_Info_t userShow;
	short uid = ustore.uid;
	short pswd = ustore.passwd;
	
	int i = 1000;
	short index = 3;
	while(index >= 0)
	{
		userShow.uid[ index-- ] = uid/i + '0';
		uid = uid%i;
		i/=10;
	}
	
	i=1000000;
	index=5;
	while(index >= 0)
	{
		userShow.passwd[ index-- ] = pswd/i + '0';
		pswd = pswd%i;
		i/=10;
	}
}

/*显示格式->存储格式*/
user_Info_Store_t showStyleToStoreStyle(user_Info_t uinfo)
{
	user_Info_Store_t user_store;
	short index = 3;
	short temp = 0;
	int tempInt = 0;
	while(index >=0 )
	{
		temp = temp*10 + (uinfo.uid[ index-- ] - '0');
	}
	user_store.uid = temp;
	
	
	index = 5;
	while(index >=0 )
	{
		tempInt = tempInt*10 + (uinfo.passwd[ index-- ] - '0');
	}
	user_store.passwd = tempInt;
	
	user_store.fgNum = uinfo.fgNum;
	
}



int addUserInfo(int userID , int password)
{
	short firstUnused = -1;
	user_Info_Store_t usi,*search;
	printf("Add User: id=%d,pswd=%d\n",userID,password);
	search = getuserInfo(userID);
	if(!search)
	{
			firstUnused = getFirstUnusedBit();
			printf("firstUnUsedBit=%d\n",firstUnused);
			if(firstUnused < 0)
			{
				printf("Store Error: Array is FULL\n");
			}
			userStoreArray[ firstUnused ].uid = userID;
			userStoreArray[ firstUnused ].passwd = password;
	}
	else // this userID already exit
	{
		printf("Store Error: You want to add a user whose userID has exist in this array\n");
	}
	
}



int addUserInfoShowStyle(char userid[],char pswd[])
{
	//userid = "1234";
	//pswd = "987654";	printf("++++++++++++++++++ param=%s____%s\n",userid,pswd);
	int userIdInt,passwdInt;
	int index = 0;
	int temp = 0;
	int tempInt = 0;
	while(index <= 3)
	{
		
		temp = temp*10 + (userid[ index++ ] - '0');

	}
	userIdInt = temp;
	
	
	index = 0;
	while(index <=5 )
	{
		tempInt = tempInt*10 + (pswd[ index++ ] - '0');
		
	}
	passwdInt = tempInt;

	

	addUserInfo(userIdInt,passwdInt);

}


int deleteUser(int userID)
{
	getuserInfoClear(userID);
	printf("bitmapIntArray[0]=%d\n",bitmapIntArray[0]);
}


void showAllUser()
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
				printf("<Start Load:> uid=%d , pswd=%d\n",userStoreArray[ count ].uid , userStoreArray[ count ].passwd);	
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
			printf("<Start Load:> uid=%d , pswd=%d\n",userStoreArray[ count ].uid , userStoreArray[ count ].passwd);
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
