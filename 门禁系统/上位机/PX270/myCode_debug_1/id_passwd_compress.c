
#include "id_passwd_compress.h"

struct userID getUidStrct(char *userIDString)
{
	userID_t u;
	short userIDinShort = (userIDString[0]-'0')*1000 + (userIDString[1]-'0')*100 +
								 (userIDString[2]-'0')*10 + (userIDString[3]-'0');
	unsigned char userIDinCmdL = (unsigned char)(userIDinShort & 0xFF);
	unsigned char userIDinCmdH = userIDinShort >>8;
	u.uidH = userIDinCmdH;
	u.uidL = userIDinCmdL;
	return u;
}



passwd_t getPswdStrct(char *pswdString)
{
	passwd_t p;
	int pswdInInt = (pswdString[0]-'0')*100000 + (pswdString[1]-'0')*10000 +
								 (pswdString[2]-'0')*1000 + (pswdString[3]-'0')*100+
									 (pswdString[4]-'0')*10+ (pswdString[5]-'0');
	
	unsigned char pH = (unsigned char)((pswdInInt & 0x00FF0000)>>16);
	unsigned char pM = (unsigned char)((pswdInInt & 0x0000FF00)>>8);
	unsigned char pL = (unsigned char)(pswdInInt & 0x000000FF);

	p.pswdH = pH;
	p.pswdM = pM;
	p.pswdL = pL;

	return p;

}


/*
int test(int argc, char **argv) {
	char userID[]="1234";
	userID_t u = getUidStrct(userID);
	printf("u.H=0x%x\n",u.uidH);
	printf("u.L=0x%x\n",u.uidL);
	char passwordString[]="999999";
	passwd_t p = getPswdStrct( passwordString );
	printf("p = 0x%x\n",999999);
	printf("p.pH = 0x%x\n",p.pswdH);
	printf("p.pM = 0x%x\n",p.pswdM);
	printf("p.pL = 0x%x\n",p.pswdL);
	return 0;
}
*/
