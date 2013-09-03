
/*
	将输入的用户ID（只含数字的字符串，四个数字）转换成两个字节。
参数：
	@userIDString:用户ID的字符串（只含数字）
返回：
	userID_t （结构体，包含转换之后的高位与低位字节）
	
示例：
	用户ID为 "1234".
	为了压缩，考虑到将字符串"1234"转成数字(short即可，因为最大转成9999，一个short最大65535)，
	即 "1234"->1234，然后将该数字对应的高位字节与低位字节取出，放到结构体中返回。
	由于1234 = 0x4d2,  因此，高位字节为0x04,低位字节为0xd2。
*/
typedef struct userID{
	unsigned char uidH;
	unsigned char uidL;
} userID_t;

struct userID getUidStrct(char *userIDString);


/*
	将输入的用户密码（只含数字的字符串，长度为6）转换成三个字节。
参数：
	@pswdString:用户密码的字符串（只含数字）
返回：
	passwd_t （结构体，包含转换之后的高位、中位、低位字节）
	
示例：
	用户密码为 "999999".
	为了压缩，考虑到将字符串"999999"转成数字(int即可，因为最大转成999999，一个short最大65535,int最大16M)，
	即 "999999"->999999，然后将该数字对应的高位字节与低位字节取出，放到结构体中返回。
	由于999999 = 0xf423f,  因此，高位字节为0x0f,中位字节为0x23，低位字节为0x3f。
*/
typedef struct passwd{
	unsigned char pswdH;
	unsigned char pswdM;
	unsigned char pswdL;
}  passwd_t;

passwd_t getPswdStrct(char *pswdString)；




/*
int main(int argc, char **argv) {//test
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
