/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

/*
typedef struct userInfoInNum{
	char passwd[6];
	char uid[4];
	char fgNum;
} user_Info_t;
user_Info_t userStoreArray[ MAX_USER_LENGTH ];
*/

#include <qthread.h>

extern "C"
{ 
#include "gprs/tty.c"
#include "gprs/gprs.c"
#include "serial/Serial485.c"
#include "serial/doParse.c"
#include "serial/myTty.c"
}


#include "pthread.h"
#include "mythread.cpp"

class QPushButton;
class QLineEdit;

QString numberText = "";
QString messageText = "";
bool isShiftToggled = false;
bool isCapsLockToggled = false;

int currentPage;
int totalPage;
int selectRow = 0;
int column = 0;//which column 1-username 2-password 3-fingnum

int inputModel = 0;//0-new user 1-modify pwd

user_Info_t loadUserArray[5];

QString addUserId="",addUserPswd="";

myThread *thread;

void mainForm::styleChar(QLineEdit *le1,QLineEdit *le2,QLineEdit *le3, int loadArrayIndex )
{
    int i;
    int fgInt;
    for(i=0; i < 4;i++)
	loadUserArray[loadArrayIndex].uid[i] += '0';
    const char *cuid = loadUserArray[loadArrayIndex].uid;
    QString uid( cuid );
    le1->setText(uid);
    for(i=0; i < 6;i++)
	loadUserArray[loadArrayIndex].passwd[i] += '0';
    
    const char *cpwd = loadUserArray[loadArrayIndex].passwd;
    QString pwd( cpwd );
    le2->setText(pwd);
    
    char fgIntChar[3];
    fgInt = loadUserArray[loadArrayIndex].fgNum;
    sprintf(fgIntChar,"%d",fgInt);
    const char *cfg = fgIntChar;
    QString fgnum(cfg);
    le3->setText(fgnum);
}

//load 'whichpage' userinfo to table
void mainForm::loadUserToTable(int whichpage)
{	
    int loadCount = 0 , i=0;
    char uidTail[5];
    char passwdTail[7];
    
    lineEdit11->setText("");
    lineEdit12->setText("");
    lineEdit13->setText("");
    
    lineEdit21->setText("");
    lineEdit22->setText("");
    lineEdit23->setText("");
    
    lineEdit31->setText("");
    lineEdit32->setText("");
    lineEdit33->setText("");
    
    lineEdit41->setText("");
    lineEdit42->setText("");
    lineEdit43->setText("");
    
    lineEdit51->setText("");
    lineEdit52->setText("");
    lineEdit53->setText("");
    
    loadCount = getAPageOfUser(whichpage, loadUserArray);
    printf("loadCount = %d\n",loadCount);
    if( loadCount >= 1 )
    {
	styleChar(lineEdit11,lineEdit12,lineEdit13,0);
    }
    
    if( loadCount >= 2 )
    {
	styleChar(lineEdit21,lineEdit22,lineEdit23,1);
    }
    if( loadCount >= 3 )
    {
	styleChar(lineEdit31,lineEdit32,lineEdit33,2);
    }
    if( loadCount >= 4 )
    {
	styleChar(lineEdit41,lineEdit42,lineEdit43,3);
    }
    
    if( loadCount >= 5 )
    {
	styleChar(lineEdit51,lineEdit52,lineEdit53,4);
    }
    
    if(totalPage == 0)
	totalPage += 1;
    textLabel7->setText("current page " + QString::number(currentPage + 1) + " total page " + QString::number(totalPage)); 
    
}



void mainForm::init()
{
    tty_init(); 
    gprs_init(); 
    
    init_listen_thread();
    currentPage = 0;
    loadUserToTable(currentPage);
    
    thread = new myThread(alertLabel);
    thread->start();
    
}

void mainForm::destroy()
{
    tty_end();
}

void mainForm::refreshButton_clicked()
{
    int stateOfDoorFlag;
    getStateOfDoor();
    mySleep(50);//wait 50 ms
    stateOfDoorFlag = getDoorStateFlag();
    
    if( stateOfDoorFlag == 0 ){
	doorstatusLabel->setText("Closed");
    }else if( stateOfDoorFlag == 1 ){
	doorstatusLabel->setText("Opened");
    }else{
	doorstatusLabel->setText("Unknown");
    }
}

void mainForm::unlockButton_clicked()
{
    setLockOpen();
}

void mainForm::previewButton_clicked()
{
    totalPage = totalUser/5 + (totalUser%5==0 ? 0:1);
    printf("preview  totalUser = %d totalPage = %d currentPage = %d\n",totalUser, totalPage, currentPage);
    if(currentPage <= 0){
	loadUserToTable(currentPage);
	
	return;
    }
    
    currentPage--;
    loadUserToTable(currentPage);
}

void mainForm::nextviewButton_clicked()
{
    
    totalPage = totalUser/5 + (totalUser%5==0 ? 0:1);
    
    printf("nextview totalUser = %d totalPage = %d currentPage = %d\n",totalUser, totalPage, currentPage);
    if( currentPage >= totalPage-1 ){
	loadUserToTable(currentPage);
	
	return;
    }

    currentPage++;
    printf("Next>currentPage=%d   totalPage=%d\n",currentPage,totalPage);
    loadUserToTable(currentPage);
    
}


void mainForm::newuserButton_clicked()
{
    lineEdit11->setReadOnly(false);
    lineEdit12->setReadOnly(false);
    lineEdit13->setReadOnly(false);
    
    lineEdit11->setEnabled(true);
    lineEdit12->setEnabled(true);
    lineEdit13->setEnabled(true);
    
    lineEdit21->setEnabled(false);
    lineEdit22->setEnabled(false);
    lineEdit23->setEnabled(false);
    
    lineEdit31->setEnabled(false);
    lineEdit32->setEnabled(false);
    lineEdit33->setEnabled(false);
    
    lineEdit41->setEnabled(false);
    lineEdit42->setEnabled(false);
    lineEdit43->setEnabled(false);
    
    lineEdit51->setEnabled(false);
    lineEdit52->setEnabled(false);
    lineEdit53->setEnabled(false);
    
    lineEdit11->setText("");
    lineEdit12->setText("");
    lineEdit13->setText("");
    
    lineEdit21->setText("");
    lineEdit22->setText("");
    lineEdit23->setText("");
    
    lineEdit31->setText("");
    lineEdit32->setText("");
    lineEdit33->setText("");
    
    lineEdit41->setText("");
    lineEdit42->setText("");
    lineEdit43->setText("");
    
    lineEdit51->setText("");
    lineEdit52->setText("");
    lineEdit53->setText("");
    
    
    newuserButton->setEnabled(false);
    saveButton->setEnabled(true);
    
    
}

void mainForm::deleteuserButton_clicked()
{
    QString temp;
    const char *const_uid;
    char *uid;
    if(selectRow == 1){
	temp = lineEdit11->text();
    }else if(selectRow == 2){
	temp = lineEdit21->text();
    }else if(selectRow == 3){
	temp = lineEdit31->text();
    }else if(selectRow == 4){
	temp = lineEdit41->text();
    }else if(selectRow == 5){
	temp = lineEdit51->text();
    }else if(selectRow == 0){
	return;
    }
    if(temp.length() < 4){
	deleteuserButton->setEnabled(false);
       modifypwdButton->setEnabled(false);
       loadUserToTable(0);
	return;
    }
    const_uid = temp.ascii();
    uid = new char[4];
    for(int i=0; i<4; i++){
	uid[i] = const_uid[i] - '0';
    }
    printf("uid = %d%d%d%d\n",uid[0],uid[1],uid[2],uid[3]);
    deleteUser(uid);
    selectRow = 0;
    delete uid;
    
    newuserButton->setEnabled(true);
    deleteuserButton->setEnabled(false);
    modifypwdButton->setEnabled(false);
    loadUserToTable(0);
    
    inputModel = 0;
}

void mainForm::modifypwdButton_clicked()
{
    QString temp1, temp2;
    const char *const_uid, *const_pwd;
    char *uid, *pwd;
    if(selectRow == 1){
	temp1 = lineEdit11->text();
	temp2 = lineEdit12->text();
	
    }else if(selectRow == 2){
	temp1 = lineEdit21->text();
	temp2 = lineEdit22->text();
    }else if(selectRow == 3){		
	temp1 = lineEdit31->text();
	temp2 = lineEdit32->text();
    }else if(selectRow == 4){
	temp1 = lineEdit41->text();
	temp2 = lineEdit42->text();
    }else if(selectRow == 5){
	temp1 = lineEdit51->text();
	temp2 = lineEdit52->text();
    }else if(selectRow == 0){
	return;
    }
    printf("selectRow = %d\n", selectRow);
    if((temp1.length() < 4)  || (temp2.length() < 4)){
	deleteuserButton->setEnabled(false);
       modifypwdButton->setEnabled(false);
       loadUserToTable(0);
	return;
    }
    const_uid = temp1.ascii();
    const_pwd = temp2.ascii();
    uid = new char[4];
    pwd = new char[6];
    for(int i=0; i<4; i++){
	uid[i] = const_uid[i] - '0';
    }
    
    for(int i=0; i<6; i++){
	pwd[i] = const_pwd[i] - '0';
    }
    
    printf("uid = %d%d%d%d\n",uid[0],uid[1],uid[2],uid[3]);
    
     printf("pwd = %d%d%d%d%d%d\n",pwd[0],pwd[1],pwd[2],pwd[3],pwd[4],pwd[5]);
    ModifyPasswd(uid, pwd);
    selectRow = 0;
    delete uid;
    delete pwd;
    
    newuserButton->setEnabled(true);
    deleteuserButton->setEnabled(false);
    modifypwdButton->setEnabled(false);
    loadUserToTable(0);
    
    inputModel = 0;
}

void mainForm::backspaceButton1_clicked()
{
    int index;
    if(inputModel == 0){
       if(column == 1){
	   addUserId = lineEdit11->text();
	   index = addUserId.length() - 1;
	   addUserId.remove(index, 1);
	   lineEdit11->setText(addUserId);
       }else if(column == 2){
	   addUserPswd = lineEdit12->text();
	   index = addUserPswd.length() - 1; 
	   addUserPswd.remove(index, 1);
	   lineEdit12->setText(addUserPswd);
       }
    }else if(inputModel == 1){
	if(selectRow == 1){
	   addUserPswd = lineEdit12->text();
	   index = addUserPswd.length() -1;
	   addUserPswd.remove(index, 1);
	   lineEdit12->setText(addUserPswd);
	}else if(selectRow == 2){
	   addUserPswd = lineEdit22->text();
	   index = addUserPswd.length() -1;
	   addUserPswd.remove(index, 1);
	   lineEdit22->setText(addUserPswd);
	}else if(selectRow == 3){
	   addUserPswd = lineEdit32->text();
	   index = addUserPswd.length() -1;
	   addUserPswd.remove(index, 1);
	   lineEdit32->setText(addUserPswd); 
	}else if(selectRow == 4){
	   addUserPswd = lineEdit42->text();
	   index = addUserPswd.length() -1;
	   addUserPswd.remove(index, 1);
	   lineEdit42->setText(addUserPswd); 
	}else if(selectRow == 5){
	   addUserPswd = lineEdit52->text();
	   index = addUserPswd.length() -1;
	   addUserPswd.remove(index, 1);
	   lineEdit52->setText(addUserPswd); 
	}
    }
}

void mainForm::callButton_clicked()
{
    numberText = numberEdit->text();
    const char *temp = numberText.ascii();
    int len = numberText.length();
    char *number = new char[len];
    for(int i=0; i<len; i++)
	number[i] = temp[i];
    gprs_call(number);
    
    numberText = "";
    numberEdit->setText(numberText);
    
    delete number;
}

void mainForm::holdButton_clicked()
{
    gprs_hold();
}

void mainForm::messageButton_clicked()
{
    numberText = numberEdit->text();
    messageText = textEdit->text();
    const char *temp1 = numberText.ascii();
    const char *temp2 = messageText.ascii();
    int len1 = numberText.length();
    int len2 = messageText.length();
    char *number = new char[len1];
    char *text = new char[len2];
    for(int i=0; i<len1; i++)
	number[i] = temp1[i];
    for(int j=0; j<len2; j++)
	text[j] = temp2[j];
    
    gprs_msg(number, text);
    
    numberText = "";
    numberEdit->setText(numberText);
    messageText = "";
    textEdit->setText(messageText);
    
    delete number;
    delete text;
}

void mainForm::pushButton_enter_clicked()
{
    if(isShiftToggled){
	messageText += "\n";
	textEdit->setText(messageText);
    }else{
	numberText += "\n";
	numberEdit->setText(numberText);
    }   
}

void mainForm::shiftButton_toggled( bool on )
{
    isShiftToggled = on;
}

void mainForm::capslockButton_toggled( bool on )
{
    isCapsLockToggled = on;
}

void mainForm::spaceButton_clicked()
{
    if(isShiftToggled){
	messageText += " ";
	textEdit->setText(messageText);
    }else{
	numberText += " ";
	numberEdit->setText(numberText);
    }   
}

void mainForm::bachspaceButton2_clicked()
{
    int index;
    if(isShiftToggled){
	index = messageText.length() - 1;
	messageText.remove(index, 1);
	textEdit->setText(messageText);
    }else{
	index = numberText.length() - 1;
	numberText.remove(index, 1);
	numberEdit->setText(numberText);
    }
}

void mainForm::numKey_clicked()
{
    if(column == 1)
    {
	addUserId = lineEdit11->text();
	addUserId+= ((QPushButton *)this->sender())->text();
	lineEdit11->setText(addUserId);
    }else if(column == 2){
	addUserPswd = lineEdit12->text();
	addUserPswd+= ((QPushButton *)this->sender())->text();
	lineEdit12->setText(addUserPswd);
    }
}

void mainForm::charKey_clicked()
{
    
    if(isShiftToggled){
	if(isCapsLockToggled){
	    messageText += (((QPushButton *)this->sender())->text()).upper();	
	}else{
	    messageText += ((QPushButton *)this->sender())->text();	
	}
	textEdit->setText(messageText);
    }else{
	numberText += ((QPushButton *)this->sender())->text();	
	numberEdit->setText(numberText);
    }
    
}

void mainForm::usernameButton_clicked()
{
    column = 1;        
}

void mainForm::pwdButton_clicked()
{
    column = 2;    
}

void mainForm::fingermapButton_clicked()
{
    column = 3;    
}

void mainForm::labelButton1_clicked()
{
    selectRow = 1;   
    inputModel = 1;
    newuserButton->setEnabled(false);
    deleteuserButton->setEnabled(true);
    modifypwdButton->setEnabled(true);
}

void mainForm::labelButton2_clicked()
{
    selectRow = 2;
    inputModel = 1;
    newuserButton->setEnabled(false);
    deleteuserButton->setEnabled(true);
    modifypwdButton->setEnabled(true);
}

void mainForm::labelButton3_clicked()
{
    selectRow = 3;
    inputModel = 1;
    newuserButton->setEnabled(false);
    deleteuserButton->setEnabled(true);
    modifypwdButton->setEnabled(true);
}

void mainForm::labelButton4_clicked()
{
    selectRow = 4;
    inputModel = 1;
    newuserButton->setEnabled(false);
    deleteuserButton->setEnabled(true);
    modifypwdButton->setEnabled(true);
}

void mainForm::labelButton5_clicked()
{
    selectRow = 5;
    inputModel = 1;
    newuserButton->setEnabled(false);
    deleteuserButton->setEnabled(true);
    modifypwdButton->setEnabled(true);
}






void mainForm::saveButton_clicked()
{
    printf(".......save...1....\n");
    if((lineEdit11->text()).length() < 4 || (lineEdit12->text()).length() < 6){
	 newuserButton->setEnabled(true);
        saveButton->setEnabled(false);
        loadUserToTable(0);
	 return;
    }
    const char *useridconst = (lineEdit11->text()).ascii();
    printf(".......save...ascii ok 1 ....\n");
    char *user_id = new char[4];
    for(int i=0; i < 4; i++)
	user_id[i]= useridconst[i] - '0';
    const char *pswdconst = (lineEdit12->text()).ascii();
    printf(".......save...ascii ok  2 ....\n");
    char *user_pswd = new char[4];
    for(int i=0; i < 6; i++)
	user_pswd[i]= pswdconst[i] - '0';
    
    printf(".......save...2....\n");
    lineEdit11->setEnabled(true);
    lineEdit12->setEnabled(true);
    lineEdit13->setEnabled(true);
    
    lineEdit21->setEnabled(true);
    lineEdit22->setEnabled(true);
    lineEdit23->setEnabled(true);
    
    lineEdit31->setEnabled(true);
    lineEdit32->setEnabled(true);
    lineEdit33->setEnabled(true);
    
    lineEdit41->setEnabled(true);
    lineEdit42->setEnabled(true);
    lineEdit43->setEnabled(true);
    
    lineEdit51->setEnabled(true);
    lineEdit52->setEnabled(true);
    lineEdit53->setEnabled(true);
    
    printf(".......save...3....\n");
    
    printf("<***>ADD USER:id=%d%d%d%d  pswd=%d%d%d%d%d%d\n",user_id[0],user_id[1],user_id[2],user_id[3],user_pswd[0],user_pswd[1],user_pswd[2],user_pswd[3],user_pswd[4],user_pswd[5]);
    AddNewUser(user_id,user_pswd);
    
    delete user_id;
    delete user_pswd;
    
    newuserButton->setEnabled(true);
    saveButton->setEnabled(false);
    loadUserToTable(0);
}
