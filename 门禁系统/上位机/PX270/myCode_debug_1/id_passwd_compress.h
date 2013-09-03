
/*
	��������û�ID��ֻ�����ֵ��ַ������ĸ����֣�ת���������ֽڡ�
������
	@userIDString:�û�ID���ַ�����ֻ�����֣�
���أ�
	userID_t ���ṹ�壬����ת��֮��ĸ�λ���λ�ֽڣ�
	
ʾ����
	�û�IDΪ "1234".
	Ϊ��ѹ�������ǵ����ַ���"1234"ת������(short���ɣ���Ϊ���ת��9999��һ��short���65535)��
	�� "1234"->1234��Ȼ�󽫸����ֶ�Ӧ�ĸ�λ�ֽ����λ�ֽ�ȡ�����ŵ��ṹ���з��ء�
	����1234 = 0x4d2,  ��ˣ���λ�ֽ�Ϊ0x04,��λ�ֽ�Ϊ0xd2��
*/
typedef struct userID{
	unsigned char uidH;
	unsigned char uidL;
} userID_t;

struct userID getUidStrct(char *userIDString);


/*
	��������û����루ֻ�����ֵ��ַ���������Ϊ6��ת���������ֽڡ�
������
	@pswdString:�û�������ַ�����ֻ�����֣�
���أ�
	passwd_t ���ṹ�壬����ת��֮��ĸ�λ����λ����λ�ֽڣ�
	
ʾ����
	�û�����Ϊ "999999".
	Ϊ��ѹ�������ǵ����ַ���"999999"ת������(int���ɣ���Ϊ���ת��999999��һ��short���65535,int���16M)��
	�� "999999"->999999��Ȼ�󽫸����ֶ�Ӧ�ĸ�λ�ֽ����λ�ֽ�ȡ�����ŵ��ṹ���з��ء�
	����999999 = 0xf423f,  ��ˣ���λ�ֽ�Ϊ0x0f,��λ�ֽ�Ϊ0x23����λ�ֽ�Ϊ0x3f��
*/
typedef struct passwd{
	unsigned char pswdH;
	unsigned char pswdM;
	unsigned char pswdL;
}  passwd_t;

passwd_t getPswdStrct(char *pswdString)��




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
