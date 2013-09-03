

#include <stdio.h>

/********************************************************************************************/
/*
Here is the bitmap operations
�ṩbitmap������
*/
/*
(________)    (________)       (________)
IntAry[2]  IntAry[1]  IntAry[0]
 _32bit_

*/

#define MAX_USER_LENGTH 64					//bitmap�ܳ��ȡ�
//������Ҫ����int(32λ���ϸ���˵��Ҫ���sizeof(int)��ֵ)��
#define reminder  (MAX_USER_LENGTH % 32)	//����һ��int����ͷ
#define reminder_addtional  reminder?1:0	//�Ƿ�����û����ͷ����=0 ��=1
#define intArrayLength  (((MAX_USER_LENGTH)/32)+(reminder_addtional))	//��Ҫ��int����
int bitmapIntArray[ intArrayLength ];		//bitmap

//��ʼ��bitmap
void init_bitmap()
{
	short i=0;
#ifdef DEBUG
	printf("******* intArrayLength=%d   reminder=%d\n", intArrayLength , reminder);
#endif
	for(i=0; i < intArrayLength ;  i++)
		bitmapIntArray[i]=0;

}

//���ص�һ����λ��λ�á�0 ~ MAX_USER_LENGTH-1 , û�п�λ����-1
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

//���ĳһλ
//@index :  0~MAX_USER_LENGTH-1 ,Խ���򷵻�-1
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



/*�洢��ʽ�������͵�����*/
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






/*���Ҳ����botmap�������Ӧλ*/
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
