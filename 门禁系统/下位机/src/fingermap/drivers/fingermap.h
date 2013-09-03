#ifndef _FINGERMAP_H_
#define _FINGERMAP_H_

#include "../include.h"


extern unsigned char fingermap_id;


//插入用户指纹
unsigned char user_enroll(unsigned char ucFPIdx); 
unsigned char fingermap_enroll_apl();

//指纹验证
unsigned char User_auth(unsigned char ucFPIdx);
unsigned char fingermap_auth_apl();


//指纹删除的函数封装
unsigned char User_erase(unsigned char ucFPIdx);
int fingermap_erase_apl(unsigned char fgNum);
	

#endif
