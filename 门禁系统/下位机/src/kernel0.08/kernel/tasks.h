#ifndef _TASKS_H_
#define _TASKS_H_

#include "../include.h"

//定义全局标志位
extern unsigned char passwdauth; //进行密码验证
extern unsigned char passwdmodify; //进行密码修改
extern unsigned char fingermapauth; //进行指纹验证
extern unsigned char fingermapmodify; //进行指纹修改
extern unsigned char processcommand; //处理上位机数据
extern unsigned char fingermapid; //当前用户指纹号
extern unsigned char confirm; //F4确认
extern unsigned char get_key_count; //用户名和密码键数

//保存用户名和密码
extern unsigned char name[4];
extern unsigned char passwd[6];
extern unsigned char passwd_confirm[6];

void tasks_init(void);

//密码验证
void password_authentication(void); 

//密码修改
void password_modify(void); 

//指纹验证
void fingermap_authentication(void); 

// 指纹添加或修改
void fingermap_modify(void); 

//处理上位机控制命令
void process_commands(void); 
#endif
