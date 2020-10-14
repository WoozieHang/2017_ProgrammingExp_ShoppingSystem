#include<iostream>
#include<cstring>
#include <iomanip>
#include<Windows.h>
using namespace std;
//�Ѿ���չ�Ĺ��ܣ���ɫ�����޸����롢��������Ʒ�嵥����Ʒ��������չʾ����Ʒ��id���۸�������˳��չʾ�������ջ���ַ�鿴�������ջ���ַ��ӡ������ջ���ַɾ����
//����ʵ�֣��¼ܺ�鿴�����¼���Ʒ
//��չ����:
void Showmenu()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY| FOREGROUND_RED);
	cout << "������֮���������www.BigGay.com" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "===============================================================================================================" << endl;
	cout << "0.�˳�ϵͳ		1.�û���½		2.�û�ע��		3.����Ա��½		4.�޸�����		" ;
	cout << "===============================================================================================================" << endl;
	cout << "���������";
}
int IdTurnNum(char a,char b,char c,char d)
{
	int aa, bb, cc, dd;
	aa = a- '0';
	bb = b - '0';
	cc = c - '0';
	dd = d - '0';
	return(dd + 10 * cc + 100 * bb + 1000 * aa);
}
struct Goods
{
	char Id[15];
	char Name[15];
	char BandName[15];
	double Price;
	int Rest;
	int SoldNum;
	int PurchaseNum;
	Goods* next;
};
struct AdminAccount
{
	char Id[10];
	char Password[30];
};
struct UserAccount
{
	char Id[10];
	char Password[30];
};
struct ShoppingHistory
{
	Goods goods[20];
	int goodsnum;
	char address[80];
	ShoppingHistory* next;
};
struct Address
{
	char address[80];
	Address* next;
};
//**********************************************************************************����Ա������
class Admin
{
	AdminAccount adminaccount;
	int SortNum;//��Ʒ������
public:
	Admin();
	bool Login();//���ص�½������ɹ�1��ʧ��0
	void Logout();
	void ShowRestGoods();
	void ShowRestGoodsInIdOrder();
	void ShowRestGoodsInPriceDecreasingOrder();
	void ShowRestGoodsInPriceIncreasingOrder();
	void Admin::ShowRestGoodsInSoldNumOrder();
	void AddGoods();
	bool ChangeGoods();
	bool DeleGoods();
	void ShowSoldList();
	void ShowLoginInformation();
	bool Log;//��ǵ�½״̬
	Goods* headgoods;
	Goods* lastgoods;
	Goods SoldList[1000];
};
//*************************************************************************************�û�������
class User
{
	double restmoney;
public:
	User();
	ShoppingHistory* shoppinghistoryhead;//������ʷ-����
	Address* addresshead; //�ջ��ַ- ����
	Goods* usercarthead;//���ﳵ-����
	UserAccount useraccount;
	Goods* headgoods;
	Goods* lastgoods;
	Goods SoldList[1000];
	bool Log;//��ǵ�½״̬
	bool Login();//���ص�½������ɹ�1��ʧ��0
	bool Register();
	void ShowLoginInformation();
	void Logout();
	void ShowRestGoods();
	void ShowRestGoodsInIdOrder();//��id˳��չʾ��Ʒ
	void ShowRestGoodsInPriceIncreasingOrder();//���۸�ӵ͵���չʾ
	void ShowRestGoodsInPriceDecreasingOrder();//�۸�Ӹߵ���չʾ
	void ShowRestGoodsInSoldNumOrder();//���������Ӷൽ��˳��չʾ
	void searchgoods();
	void Showcart();
	bool addcart();//���ﳵ-����
	bool delecart();
	bool pay();
	bool ChangePassword();//�޸�����
	void Showrestmoney();//���
	void addrestmoney();
	void Showshoppinghistory();
	void Deleshoppinghistory();
	void Zeroshoppinghistory();
	void Showaddress();
	void Addaddress();
	void Deleaddress();
};
//**************************************************************************************�ļ���˵��
class DoFile
{
public:
	void GoodsInFile(Goods *headgoods);
	Goods* GoodsOutFile(Goods *headgoods, Goods *lastgoods);
	void SoldOutFile(Goods SoldList[]);
	void SoldInFile(char UserId[],Goods* y);
	int UserOutFile(UserAccount auseraccount);
	void UserNewKeyInfile(UserAccount NewUserAccount);
	void UserInFile(UserAccount NewUserAccount);
	Goods* CartOutFile(Goods *usercarthead, char UserId[]);
	void CartInFile(Goods *usercarthead, char UserId[]);
	void moneyinfile(double restmoney, char UserId[]);
	double moneyoutfile(char UserId[]);
	void AddressInFile(char UserId[], Address *addresshead);
	Address* AddressOutFile(char UserId[]);
	void ShoppingHistoryInFile(char UserId[], ShoppingHistory *shoppinghistoryhead);
	ShoppingHistory *ShoppingHistoryOutFile(char UserId[],Goods* headgoods);
};
//****************************************************************************************������˵��
class Opera
{
public:
	Opera();
	void OperateMenu();
	void OperateUser();
	void OperateUserregister();
	void OperateAdmin();
	void OperateChangeKey();
	char op;
};
//*******************************************************************************************��Ʒ������˵��
class OrderGoods
{
public:
	void OrderByPrice(Goods *head,int Order[]);//����
	void OrderBySoldNum(Goods SoldList[],int Order[],Goods *head);
};
//********************************************************************************************�ļ��๦��
void DoFile::GoodsInFile(Goods *headgoods)
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\���.txt", "w");
	fputs("ID\t����\tƷ��\t�۸�\t����\n", fp);
	Goods *x = headgoods;
	while (x != NULL)
	{
		fputs(x->Id, fp);
		fprintf(fp, "\t");
		fputs(x->Name, fp);
		fprintf(fp, "\t");
		fputs(x->BandName, fp);
		fprintf(fp, "\t");
		fprintf(fp, "%.1f", x->Price);
		fprintf(fp, "\t");
		fprintf(fp, "%d", x->Rest);
		fprintf(fp, "\n");
		x = x->next;
	}
	fclose(fp);
}
Goods* DoFile::GoodsOutFile(Goods *headgoods, Goods *lastgoods)
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\���.txt", "r");
	char information[20][100];
	int i = 0;
	while (!feof(fp))
	{
		fgets(information[i], 95, fp);
		i++;
	}
	Goods *p;
	p = new Goods;
	i--;
	if (headgoods == NULL)
	{
		headgoods = p;
		lastgoods = p;
	}
	for (int j = 2; information[j][0]=='F'; j++)
	{
		p = new Goods;
		lastgoods->next = p;
		lastgoods = p;
		lastgoods->next = NULL;
	}
	Goods *x = headgoods;
	int j;
	for (j = 1; information[j][0]=='F'; j++, x = x->next)
	{
		int k = 0;
		while (information[j][k] == '\t')
			k++;
		int nb=0;
		for (int q = 0; q < 5; q++)
		{
			x->Id[q] = information[j][k];
			k++;
			nb = q;
		}
		x->Id[nb+1] = '\0';
		while (information[j][k] == '\t')
			k++;
		for (int q = 0; information[j][k] != '\t'; q++)
		{
			x->Name[q] = information[j][k];
			k++;
			nb = q;
		}
		x->Name[nb+1] = '\0';
		while (information[j][k] == '\t')
			k++;
		for (int q = 0; information[j][k] != '\t'; q++)
		{
			x->BandName[q] = information[j][k];
			k++;
			nb = q;
		}
		x->BandName[nb+1] = '\0';
		while (information[j][k] == '\t')
			k++;
		x->Price = 0;
		while (information[j][k] != '\t'&&information[j][k] != '.')
		{
			x->Price *= 10;
			x->Price += information[j][k] - '0';
			k++;
		}
		if (information[j][k] == '.')
		{
			double dot;
			int mi = 1;
			k++;
			while (information[j][k] != '\t')
			{
				dot = (information[j][k] - '0');
				for (int m = 0; m<mi; m++)
					dot /= 10;
				x->Price += dot;
				k++;
				mi++;
			}
		}
		while (information[j][k] == '\t')
			k++;
		x->Rest = 0;
		if (information[j][k] == '-')
			x->Rest = -1;
		else
			while (information[j][k] != '\n'&&information[j][k] != '\0')
		{
			x->Rest *= 10;
			x->Rest += information[j][k] - '0';
			k++;
		}
	}
	fclose(fp);
	return headgoods;
}
void DoFile::SoldOutFile(Goods SoldList[])
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\�����嵥.txt", "r");
	char information[1000][50];
	int i = 0;
	for (; !feof(fp); i++)
	{
		fgets(information[i], 48, fp);
	}//��ɶ�ȡ
	i--;
	int k;
	for (int j =1; information[j][0]=='F'; j++)
	{
		int x = IdTurnNum(information[j][1], information[j][2], information[j][3], information[j][4]);
		k = 6;
		int nb = 0;
		for (int index = 0; information[j][k] != '\t'; index++, k++)
		{
			SoldList[x].Name[index] = information[j][k];
			nb = index;
		}
		SoldList[x].Name[nb+1] = '\0';
		k++;
		for (int index = 0; information[j][k] != '\t'; index++, k++)
		{
			SoldList[x].BandName[index] = information[j][k];
			nb = index;
		}
		SoldList[x].BandName[nb+1] = '\0';
		k++;
		double De=0;
		while (information[j][k] != '.'&&information[j][k] != '\t')
		{
			De *= 10;
			De += information[j][k] - '0';
			k++;
		}
		if (information[j][k]== '.')
		{
			double Do = 0;
			int kk = k;
			while (information[j][kk] != '\t')
				kk++;
			for (kk--; kk >k; kk--)
			{
				Do /= 10;
				Do+= information[j][kk] - '0';
			}
			Do /= 10;
			SoldList[x].Price = De + Do;
		}
		while (information[j][k]!= '\t')
			k++;
		int SNum = 0;
		k++;
		while (information[j][k] != '\t')
		{
			SNum *= 10;
			SNum += information[j][k] - '0';
			k++;
		}
		SoldList[x].SoldNum += SNum;
	}
	fclose(fp);
}
void DoFile::SoldInFile(char UserId[], Goods*y)
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\�����嵥.txt", "r+");
	fseek(fp, 0, SEEK_END);
	Goods *x = y;
	fputs(x->Id, fp);
	fprintf(fp, "\t");
	fputs(x->Name, fp);
	fprintf(fp, "\t");
	fputs(x->BandName, fp);
	fprintf(fp, "\t");
	fprintf(fp, "%.1f", x->Price);
	fprintf(fp, "\t");
	fprintf(fp, "%d", x->PurchaseNum);
	fprintf(fp, "\t");
	fputs(UserId, fp);
	fprintf(fp, "\n");
	fclose(fp);
}
int DoFile::UserOutFile(UserAccount auseraccount)//�˺Ų����ڷ���0��������󷵻�-1�����򷵻�1
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\�û�.txt", "r");
	char information[20][40];
	int UserNum = 1;
	fgets(information[0], 50, fp);
	while (!feof(fp))
	{
		fgets(information[UserNum], 50, fp);
		UserNum++;
	}
	int i, j;
	for (i = 1; i < UserNum; i++)
	{
		for (j = 0; information[i][j] != '\t'&&information[i][j] == auseraccount.Id[j]&&auseraccount.Id[j]!='\0'; j++);
		if (information[i][j] == '\t'&&auseraccount.Id[j]=='\0')
			i += UserNum;
	}
	if (i == UserNum)
	{
		fclose(fp);
		return 0;
	}
	i--;
	i -= UserNum;
	j++;
	int index;
	for (index = 0; information[i][j] == auseraccount.Password[index] && information[i][j] != '\n'&&auseraccount.Password[index]!='\0'; index++, j++)
		;
	if (information[i][j] != '\n'||auseraccount.Password[index]!='\0')
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);
	return 1;
}
void DoFile::UserNewKeyInfile(UserAccount NewUserAccount)//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<to be done������
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\�û�.txt", "r");
	char information[20][40];
	int UserNum = 1;
	int thisline=-1;
	fgets(information[0], 50, fp);
	while (!feof(fp))
	{
		fgets(information[UserNum], 50, fp);
		UserNum++;
	}
	for (int k = 1;information[k][0]=='u'&&thisline==-1; k++)
	{
		int q;
		for (q = 0; information[k][q] != '\t'&& information[k][q] == NewUserAccount.Id[q]; q++)
			;
		if (information[k][q]== '\t')
			thisline = k;
	}
	fclose(fp);
	errno_t err1;
	FILE *fp1;
	err1 = fopen_s(&fp1, "d:\\�û�.txt", "w");
	fprintf(fp1, "�û���\t����\n");
	for (int bt = 1; bt < thisline; bt++)
		fprintf(fp1, "%s", information[bt]);
	fprintf(fp1, "%s\t%s\n", NewUserAccount.Id, NewUserAccount.Password);
	for(int at=thisline+1;information[at][0]=='u';at++)
		fprintf(fp1, "%s", information[at]);
	fclose(fp1);
}
void DoFile::UserInFile(UserAccount NewUserAccount)
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\�û�.txt", "a");
	fprintf(fp, NewUserAccount.Id);
	fprintf(fp, "\t");
	fprintf(fp, NewUserAccount.Password);
	fprintf(fp, "\n");
	fclose(fp);
}
Goods* DoFile::CartOutFile(Goods *usercarthead,char UserId[])
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, ".txt");
	err = fopen_s(&fp, addressch, "r");
	Goods *xhead = NULL;
	Goods *y = NULL;
	char information[20][100];
	int i = 0;
	while (!feof(fp))
	{
		fgets(information[i], 95, fp);
		i++;
	}
	i--;
	if (i <=1)
	{
		fclose(fp);
		return NULL;
	}
	
	Goods *p;
	p = new Goods;
	if (xhead== NULL)
	{
		xhead = p;
		y = p;
		y->next = NULL;
	}
	for (int j = 2; information[j][0]=='F'; j++)
	{
		p = new Goods;
		y->next = p;
		y = p;
		y->next = NULL;
	}
	Goods *x = xhead;
	int j;
	for (j = 1; information[j][0]=='F'; j++, x = x->next)
	{
		int k = 0;
		while (information[j][k] == '\t')
			k++;
		int nb = 0;
		for (int q = 0; q < 5; q++)
		{
			x->Id[q] = information[j][k];
			k++;
			nb = q;
		}
		x->Id[nb + 1] = '\0';
		while (information[j][k] == '\t')
			k++;
		for (int q = 0; information[j][k] != '\t'; q++)
		{
			x->Name[q] = information[j][k];
			k++;
			nb = q;
		}
		x->Name[nb + 1] = '\0';
		while (information[j][k] == '\t')
			k++;
		for (int q = 0; information[j][k] != '\t'; q++)
		{
			x->BandName[q] = information[j][k];
			k++;
			nb = q;
		}
		x->BandName[nb + 1] = '\0';
		while (information[j][k] == '\t')
			k++;
		x->Price = 0;
		while (information[j][k] != '\t'&&information[j][k] != '.')
		{
			x->Price *= 10;
			x->Price += information[j][k] - '0';
			k++;
		}
		if (information[j][k] == '.')
		{
			double dot;
			int mi = 1;
			k++;
			while (information[j][k] != '\t')
			{
				dot = (information[j][k] - '0');
				for (int m = 0; m<mi; m++)
					dot /= 10;
				x->Price += dot;
				k++;
				mi++;
			}
		}
		while (information[j][k] == '\t')
			k++;
		x->PurchaseNum = 0;
		while (information[j][k] != '\n'&&information[j][k] != '\0')
		{
			x->PurchaseNum*= 10;
			x->PurchaseNum += information[j][k] - '0';
			k++;
		}
	}
	fclose(fp);
	return xhead;
}
void DoFile::CartInFile(Goods *usercarthead, char UserId[])
{

	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, ".txt");
	err = fopen_s(&fp, addressch, "w");
	fputs("ID\t����\tƷ��\t�۸�\t����\n", fp);
	Goods* x = usercarthead;
	while (x != NULL)
	{
		fputs(x->Id, fp);
		fprintf(fp, "\t");
		fputs(x->Name, fp);
		fprintf(fp, "\t");
		fputs(x->BandName, fp);
		fprintf(fp, "\t");
		fprintf(fp, "%.1f", x->Price);
		fprintf(fp, "\t");
		fprintf(fp, "%d", x->PurchaseNum);
		fprintf(fp, "\n");
		x = x->next;
	}
	fclose(fp);
}
void DoFile::moneyinfile(double restmoney, char UserId[])
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, "restmoney.txt");
	err = fopen_s(&fp, addressch, "w");
	fprintf(fp, "%.1f", restmoney);
	fclose(fp);
}
double DoFile::moneyoutfile(char UserId[])
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, "restmoney.txt");
	err = fopen_s(&fp, addressch, "r");
	double restmoney;
	fscanf_s(fp, "%lf", &restmoney);
	fclose(fp);
	return restmoney;
}
void DoFile::AddressInFile(char UserId[], Address *addresshead)
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, "address.txt");
	err = fopen_s(&fp, addressch, "w");
	if (addresshead != NULL)
	{
		Address* x = addresshead;
		while (x != NULL)
		{
			fputs(x->address, fp);
			fprintf(fp, "\n");
			x = x->next;
		}
	}
	fclose(fp);
}
Address* DoFile::AddressOutFile(char UserId[])
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, "address.txt");
	err = fopen_s(&fp, addressch, "r");
	char information[10][80];
	for (int i = 0; i < 10; i++)
		information[i][0] = '\0';
	int i = 0;
	while (!feof(fp))
	{
		fgets(information[i], 80, fp);
		i++;
	}
	Address* head = NULL;
	Address* tail = NULL;
	for (int index = 0; information[index][0]!='\0'; index++)
	{
		if (head == NULL)
		{
			Address *p = new Address;
			strcpy_s(p->address, information[0]);
			head = p;
			tail = p;
			tail->next = NULL;
		}
		else
		{
			Address*p = new Address;
			strcpy_s(p->address, information[index]);
			tail->next = p;
			tail = p;
			tail->next = NULL;
		}
	}
	fclose(fp);
	return head;
}
void DoFile::ShoppingHistoryInFile(char UserId[], ShoppingHistory *shoppinghistoryhead)
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, "shoppinghistory.txt");
	err = fopen_s(&fp, addressch, "w");
	ShoppingHistory *x = shoppinghistoryhead;
	while (x != NULL)
	{
		for (int i = 0; i < x->goodsnum; i++)
		{
			fputs(x->goods[i].Id, fp);
			fprintf(fp, "\t%d", x->goods[i].PurchaseNum);
			fprintf(fp, "\n");
		}
		fputs(x->address, fp);
		fprintf(fp, "\n");
		x = x->next;
	}
	fclose(fp);
}
ShoppingHistory *DoFile::ShoppingHistoryOutFile(char UserId[],Goods* headgoods)
{
	errno_t err;
	FILE *fp;
	char addressch[30] = "d:\\";
	strcat_s(addressch, UserId);
	strcat_s(addressch, "shoppinghistory.txt");
	err = fopen_s(&fp, addressch, "r");
	char information[30][80];
	int i = 0;
	while (!feof(fp))
	{
		fgets(information[i], 80, fp);
		i++;
	}
	Goods* x;
	ShoppingHistory *head=NULL;
	ShoppingHistory *tail=NULL;
	int j = 0;
	while (information[j][0] == 'F'||information[j][0]=='\n')//����Ϣδ��ȡ
	{
		if (information[j][0] == 'F')
		{
			ShoppingHistory *p = new ShoppingHistory;
			p->goodsnum = 0;
			for (int q = 0; information[j][0] == 'F'; q++)//��ȡ��Ʒ��Ϣ
			{
				x = headgoods;
				p->goods[q].Id[0] = 'F';
				p->goods[q].Id[1] = information[j][1];
				p->goods[q].Id[2] = information[j][2];
				p->goods[q].Id[3] = information[j][3];
				p->goods[q].Id[4] = information[j][4];
				p->goods[q].Id[5] = '\0';
				while (x->Id[1] != information[j][1] || x->Id[2] != information[j][2] || x->Id[3] != information[j][3] || x->Id[4] != information[j][4])
					x = x->next;
				strcpy_s(p->goods[q].BandName, x->BandName);
				strcpy_s(p->goods[q].Name, x->Name);
				p->goods[q].Price = x->Price;
				p->goods[q].PurchaseNum = 0;
				int k = 6;
				while (information[j][k] != '\n')
				{
					p->goods[q].PurchaseNum *= 10;
					p->goods[q].PurchaseNum += information[j][k] - '0';
					k++;
				}
				j++;
				p->goodsnum++;
			}
			strcpy_s(p->address, information[j]);//��ȡ��ַ��Ϣ
			if (head == NULL)//ͷ��¼
			{
				head = p;
				tail = p;
				tail->next = NULL;
			}
			else//��¼
			{
				tail->next = p;
				tail = p;
				tail->next = NULL;
			}
		}
		j++;
	}
	fclose(fp);
	return head;
}
//*********************************************************************************************��Ʒ�����๦��
void OrderGoods::OrderByPrice(Goods* head,int Order[])
{
	Goods allgood[100];
	int goodsnum = 0;
	for (Goods* x = head; x != NULL; x = x->next)
	{
		strcpy_s(allgood[goodsnum].Id,x->Id);
		allgood[goodsnum].Price = x->Price;
		goodsnum++;
	}
	//��allgood����
	for (int i = 0; i < goodsnum - 1; i++)
	{
		int max = 0;
		for (int index = 1; index < goodsnum-i; index++)
			if (allgood[index].Price > allgood[max].Price)
				max = index;
		Goods a;
		a = allgood[max];
		allgood[max] = allgood[goodsnum - 1 - i];
		allgood[goodsnum - 1 - i] = a;
	}
	for (int i = 0; i < goodsnum; i++)
	{
		Order[i] = IdTurnNum(allgood[i].Id[1], allgood[i].Id[2], allgood[i].Id[3], allgood[i].Id[4]);
	}
}
void OrderGoods::OrderBySoldNum(Goods SoldList[],int Order[], Goods *head)
{
	Goods allgood[100];
	int goodsnum = 0;
	for (Goods *x=head;x!=NULL; goodsnum++)
	{
		strcpy_s(allgood[goodsnum].Id, x->Id);
		allgood[goodsnum].SoldNum = SoldList[goodsnum].SoldNum;
		x = x->next;
	}
	//��allgood����
	for (int i = 0; i < goodsnum - 1; i++)
	{
		int max = 0;
		for (int index = 1; index < goodsnum - i; index++)
			if (allgood[index].SoldNum> allgood[max].SoldNum)
				max = index;
		Goods a;
		a = allgood[max];
		allgood[max] = allgood[goodsnum - 1 - i];
		allgood[goodsnum - 1 - i] = a;
	}
	for (int i = 0; i < goodsnum; i++)
	{
		Order[i] = IdTurnNum(allgood[i].Id[1], allgood[i].Id[2], allgood[i].Id[3], allgood[i].Id[4]);
	}
}
//***************************************************************************************����Ա�๦��
Admin::Admin()
{
	strcpy_s(adminaccount.Id, "Admin");
	strcpy_s(adminaccount.Password, "admin");
	Log = 0;
	headgoods= NULL;
	lastgoods = NULL;
	for (int i = 0; i < 50; i++)
		SoldList[i].SoldNum = 0;
	for (int i = 0; i < 50; i++)
		SoldList[i].Price = 0;
}
bool Admin::Login()
{
	DoFile dofile;
	cout << "�������ԱID��";
		char ID[20];
		cin >> ID;
		cout << "�������Ա���룺";
		char key[20];
		cin >> key;
		cout << "********" << endl;
		if (strcmp(ID, adminaccount.Id) == 0 && strcmp(key, adminaccount.Password) == 0)
		{
			cout << "��½�ɹ���\n********\n";
			headgoods = dofile.GoodsOutFile(headgoods, lastgoods);
			lastgoods = headgoods;
			while (lastgoods->next != NULL)
				lastgoods = lastgoods->next;
			SortNum = IdTurnNum(lastgoods->Id[1], lastgoods->Id[2], lastgoods->Id[3], lastgoods->Id[4]);
			dofile.SoldOutFile(SoldList);
			Log = 1;
			return 1;
		}
		else
		{
			cout << "��¼ʧ��,�����ԣ�\n********\n";
			Showmenu();
			return 0;
		}
}
void Admin::Logout()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "���˳���½��\n";
	Log = 0;
}
void Admin::ShowRestGoods()
{
	char choice = '5';
	cout << "��ѡ��鿴˳�� 1.��Id˳��鿴 2.���۸�ӵ͵���˳��鿴 3.���۸�Ӹߵ���˳��鿴 4.���������Ӹߵ���˳��鿴" << endl;
	while (choice != '1'&&choice != '2'&&choice != '3'&&choice != '4')
	{
		cout << "�鿴˳��Ϊ��";
		cin >> choice;
		if (choice == '1')
			ShowRestGoodsInIdOrder();
		else if (choice == '2')
			ShowRestGoodsInPriceIncreasingOrder();
		else if (choice == '3')
			ShowRestGoodsInPriceDecreasingOrder();
		else if (choice == '4')
			ShowRestGoodsInSoldNumOrder();
		else
			cout << "���벻�Ϸ�!����������" << endl;
	}
}
void Admin::ShowRestGoodsInIdOrder()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳��Id˳��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	Goods *x = headgoods;
	while (x != NULL)
	{
		cout << x->Id << '\t';
		cout.width(10);
		cout << x->Name;
		cout.width(10);
		cout << x->BandName;
		cout.width(10);
		cout << x->Price;
		cout.width(10);
		cout << x->Rest;
		cout.width(10);
		cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		x = x->next;
	}
	cout << "*********************************************\n";
}
void Admin::ShowRestGoodsInPriceDecreasingOrder()
{
	int Order[50];
	for (int i = 0; i < 50; i++)
		Order[i] = -1;
	OrderGoods ordergoods;
	ordergoods.OrderByPrice(headgoods, Order);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳�򣺼۸���" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	int i = 0;
	while (Order[i] != -1)
		i++;
	Goods *x;
	for (i--; i >= 0; i--)
	{
		x = headgoods;
		while (IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4]) != Order[i])
			x = x->next;
		cout << x->Id << '\t';
		cout.width(10);
		cout << x->Name;
		cout.width(10);
		cout << x->BandName;
		cout.width(10);
		cout << x->Price;
		cout.width(10);
		cout << x->Rest;
		cout.width(10);
		cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		x = x->next;
	}
	cout << "*********************************************\n";
}
void Admin::ShowRestGoodsInPriceIncreasingOrder()
{
	int Order[50];
	for (int i = 0; i < 50; i++)
		Order[i] = -1;
	OrderGoods ordergoods;
	ordergoods.OrderByPrice(headgoods, Order);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳�򣺼۸�����" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	Goods *x;
	for (int i = 0; Order[i] != -1; i++)
	{
		x = headgoods;
		while (IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4]) != Order[i])
			x = x->next;
		cout << x->Id << '\t';
		cout.width(10);
		cout << x->Name;
		cout.width(10);
		cout << x->BandName;
		cout.width(10);
		cout << x->Price;
		cout.width(10);
		cout << x->Rest;
		cout.width(10);
		cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		x = x->next;
	}
	cout << "*********************************************\n";
}
void Admin::ShowRestGoodsInSoldNumOrder()
{
	int Order[50];
	for (int i = 0; i < 50; i++)
		Order[i] = -1;
	OrderGoods ordergoods;
	ordergoods.OrderBySoldNum(SoldList, Order, headgoods);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳���������ȡ�" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	Goods *x;
	int i = 0;
	while (Order[i] != -1)
		i++;
	for (i--; i >= 0; i--)
	{
		x = headgoods;
		while (IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4]) != Order[i])
			x = x->next;
		cout << x->Id << '\t';
		cout.width(10);
		cout << x->Name;
		cout.width(10);
		cout << x->BandName;
		cout.width(10);
		cout << x->Price;
		cout.width(10);
		cout << x->Rest;
		cout.width(10);
		cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		x = x->next;
	}
	cout << "*********************************************\n";
}
void Admin::AddGoods()
{
	DoFile dofile;
	char addgoodname[15];
	char addgoodbandname[15];
	cout << "��������ӵ���Ʒ��Ʒ�ƣ�";
	cin >> addgoodbandname;
	cout << "��������ӵ���Ʒ�����ƣ�";
	cin >> addgoodname;
	Goods *x = headgoods;
	while (x != NULL&&(strcmp(x->BandName, addgoodbandname) != 0||strcmp(x->Name, addgoodname) != 0))
		x = x->next;
	if (x != NULL)
	{
		if (x->Rest != -1)
		{
			cout << "����Ʒ�Ѵ��ڣ���������ӵ�������";
			int AddNum;
			cin >> AddNum;
			x->Rest += AddNum;
			dofile.GoodsInFile(headgoods);
			cout << "��ӳɹ�����" << endl;
		}
		else//�¼ܹ�����Ʒ
		{
			cout << "����Ʒ���¼ܣ��Ƿ�ѡ���ϼܣ�1�� 0��  ��" ;
			int resale;
			cin >> resale;
			if (resale == 0)
				cout << "�����Ʒ����ȡ����" << endl;
			else
			{
				cout << "��������ӵ�������" ;
				int AddNum;
				cin >> AddNum;
				x->Rest = AddNum;
				dofile.GoodsInFile(headgoods);
				cout << "��ӳɹ�����" << endl;
			}
		}
	}
	else//a new goods
	{
		x = lastgoods;
		Goods *p = new Goods;
		p->Id[0] = 'F';
		if (SortNum < 9)
		{
			p->Id[1] = '0';
			p->Id[2] = '0';
			p->Id[3] = '0';
			p->Id[4] = SortNum+1+ '0';
			p->Id[5] = '\0';
		}
		else if (SortNum < 99)
		{
			p->Id[1] = '0';
			p->Id[2] = '0';
			p->Id[3] = (SortNum+1)/10+'0';
			p->Id[4] = (SortNum+1)%10 + '0';
			p->Id[5] = '\0';
		}
		else if (SortNum <999)
		{
			p->Id[1] = '0';
			p->Id[2] = (SortNum+1)/100+'0';
			p->Id[3] =((SortNum+1)%100)/10+ '0';
			p->Id[4] = (SortNum+1)% 10 + '0';
			p->Id[5] = '\0';
		}
		strcpy_s(p->Name, addgoodname);
		strcpy_s(p->BandName, addgoodbandname);
		cout << "��������Ʒ�۸�";
		double newprice;
		cin >> newprice;
		p->Price = newprice;
		cout << "��������Ʒ��������";
		int newnum;
		cin >> newnum;
		p->Rest = newnum;
		x->next = p;
		lastgoods = p;
		lastgoods->next = NULL;
		dofile.GoodsInFile(headgoods);
		cout << "��ӳɹ�����" << endl;
	}
	//��Ҫ���Ŀ���ļ�
}
bool Admin::ChangeGoods()
{
	DoFile dofile;
	char changegoodname[15];
	char changebandname[15];
	cout << "�������޸ĵ���Ʒ��Ʒ�ƣ�";
	cin >> changebandname;
	cout << "�������޸ĵ���Ʒ�����ƣ�";
	cin >> changegoodname;
	Goods *x = headgoods;
	while ((strcmp(x->BandName, changebandname) != 0 || strcmp(x->Name, changegoodname) != 0) && x != NULL)
		x = x->next;
	if (x != NULL)
	{
		cout << "\n����Ʒ��id��:" << x->Id << "\n1.�޸ļ۸�  2.�޸�����\n��ѡ��Ը���ƷҪ���еĲ���:";
		int key = 0;
		while (key != 1 && key != 2)
		{
			cin >> key;
			if (key != 1 && key != 2)
				cout << "��������Ч������" << endl << "��ѡ��Ը���ƷҪ���еĲ���:";
		}
		if (key == 1)
		{
			cout << "����Ʒԭ���ǣ�" << x->Price << "\n��������ĺ�ļ۸�";
			cin >> x->Price;
		}
		else
		{
			cout << "����Ʒ�������ǣ�" << x->Rest << "\n�������޸ĺ��������";
			cin >> x->Rest;
		}
		//change file
		dofile.GoodsInFile(headgoods);
		return 1;
	}
	else
	{
		cout << "\n�����ڸ���Ʒ��" << endl;
		return 0;
	}
}
bool Admin::DeleGoods()
{
	DoFile dofile;
	char delegoodname[15];
	char delebandname[15];
	cout << "������ɾ������Ʒ��Ʒ�ƣ�";
	cin >> delebandname;
	cout << "������ɾ������Ʒ�����ƣ�";
	cin >> delegoodname;
	Goods *x = headgoods;
	while ((strcmp(x->BandName, delebandname) != 0 || strcmp(x->Name, delegoodname) != 0) && x != NULL)
		x = x->next;
	if (x != NULL)
	{
		cout << "\n����Ʒ��id��:" << x->Id << "\n�Ƿ�ѡ���¼ܣ�1.��  2.��\n��ѡ��Ը���ƷҪ���еĲ���:";
		int key = 0;
		while (key != 1 && key != 2)
		{
			cin >> key;
			if (key != 1 && key != 2)
				cout << "��������Ч������" << endl << "��ѡ��Ը���ƷҪ���еĲ���:";
		}
		if (key == 1)
		{
			x->Rest = -1;
		}
		else
		{
			cout << "�¼�ȡ����" << endl;
			return 0;
		}
		//change file
		dofile.GoodsInFile(headgoods);
		cout << "�¼ܳɹ�����" << endl;
		return 1;
	}
	else
	{
		cout << "\n��Ʒ�����ڣ�\n";
		return 0;
	}
}
void Admin::ShowSoldList()
{

	cout.setf(std::ios::left);
	cout << endl;
	cout << "********************************************************" << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout<<"Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout<<"����\n";
	int x = 0;
	while (x<1000)
	{
		if (SoldList[x].SoldNum != 0)
		{
			if (x < 10)
				cout << "F000" << x << '\t';
			else if (9 < x&&x < 100)
				cout << "F00" << x / 10 << x % 10 << '\t';
			else if (99 < x&&x < 999)
				cout << "F0" << x / 100 << (x / 10) % 10 << x / 10 << '\t';
			cout.width(10);
			cout << SoldList[x].Name;
			cout.width(10);
			cout << SoldList[x].BandName;
			cout.width(10);
			cout << SoldList[x].Price;
			cout << SoldList[x].SoldNum << '\n';
		}
		x++;
	}
	cout << "********************************************************" << endl;
}
void Admin::ShowLoginInformation()
{

	cout << "===================================================================================================\n";
	cout << "1.ע����¼ 2.��ѯ��Ʒ 3.������Ʒ 4.ɾ����Ʒ 5.�޸���Ʒ���� 6�ۻ��嵥\n";
	cout << "====================================================================================================\n";
	cout << "���������";
}
//*******************************************************************************************�û��๦��
User::User()
{
	usercarthead =NULL;
	headgoods = NULL;
	lastgoods = NULL;
	strcpy_s(useraccount.Id," \0");
	strcpy_s(useraccount.Password, " \0");
	Log = 0;
	restmoney = 0;
	shoppinghistoryhead=NULL;
	addresshead = NULL;
	for (int i = 0; i < 50; i++)
		SoldList[i].SoldNum = 0;
	for (int i = 0; i < 50; i++)
		SoldList[i].Price= 0;
}
void User::ShowLoginInformation()
{
	cout << "===================================================================================================\n";
	cout << "1.ע����¼ 2.�鿴��Ʒ 3.��Ʒ���� 4.�����Ʒ�����ﳵ 5.ɾ�����ﳵ��Ʒ 6.�鿴���ﳵ\n7.����. 8.������ʷ 9.�ҵ���� 10.�ҵ��ջ���ַ\n";
	cout << "====================================================================================================\n";
	cout << "���������";
}
bool User::Login()
{
	DoFile dofile;
	cout << "�����û�����";
	cin >> useraccount.Id;
	cout << "�������룺";
	cin >> useraccount.Password;
	int readans=dofile.UserOutFile(useraccount);
	if (readans == 0)
	{
		cout << "�û��������ڣ����������Ƿ���ȷ��" << endl;
		return 0;
	}
	else if (readans == -1)
	{
		cout << "����������������Ƿ���ȷ��" << endl;
		return 0;
	}
	else
	{
		cout << "********" << endl;
		cout << "��½�ɹ���"<<endl;
		cout << "********" << endl;
		Log = 1;
		headgoods = dofile.GoodsOutFile(headgoods, lastgoods);
		lastgoods = headgoods;
		while (lastgoods->next != NULL)
			lastgoods = lastgoods->next;
		dofile.SoldOutFile(SoldList);
		shoppinghistoryhead = dofile.ShoppingHistoryOutFile(useraccount.Id,headgoods);
		addresshead = dofile.AddressOutFile(useraccount.Id);
		usercarthead = dofile.CartOutFile(usercarthead, useraccount.Id);
		restmoney = dofile.moneyoutfile(useraccount.Id);
		return 1;
	}
}
bool User::Register()
{
	DoFile dofile;
	cout << "�����û���:";
	cin >> useraccount.Id;
	cout << "��������:";
	cin >> useraccount.Password;
	int readans = dofile.UserOutFile(useraccount);
	if (readans != 0)
	{
		cout << "�û��Ѵ��ڣ�\n";
		return 0;
	}
	else 
	{
		headgoods = dofile.GoodsOutFile(headgoods, lastgoods);
		lastgoods = headgoods;
		while (lastgoods->next != NULL)
			lastgoods = lastgoods->next;
		dofile.UserInFile(useraccount);
		dofile.CartInFile(usercarthead, useraccount.Id);
		dofile.moneyinfile(0, useraccount.Id);
		dofile.AddressInFile(useraccount.Id, NULL);
		dofile.ShoppingHistoryInFile(useraccount.Id, NULL);
		cout << "********" << endl;
		cout << "��½�ɹ���"<<endl;
		cout << "********" << endl;
		dofile.SoldOutFile(SoldList);
		Log = 1;
		return 1;
	}
}
void User::Logout()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
	cout << "********" << endl;
	cout << "ע���ɹ���\n";
	cout << "********" << endl;
	Log = 0;
}
void User::ShowRestGoods()
{
	char choice = '5';
	cout << "��ѡ��鿴˳�� 1.��Id˳��鿴 2.���۸�ӵ͵���˳��鿴 3.���۸�Ӹߵ���˳��鿴 4.���������Ӹߵ���˳��鿴" << endl;
	while (choice != '1'&&choice != '2'&&choice != '3'&&choice != '4')
	{
		cout << "�鿴˳��Ϊ��" ;
		cin >> choice;
		if (choice == '1')
			ShowRestGoodsInIdOrder();
		else if (choice == '2')
			ShowRestGoodsInPriceIncreasingOrder();
		else if (choice == '3')
			ShowRestGoodsInPriceDecreasingOrder();
		else if (choice == '4')
			ShowRestGoodsInSoldNumOrder();
		else
			cout << "���벻�Ϸ�!����������" << endl;
	}
}
void User::ShowRestGoodsInIdOrder()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳��Id˳��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	Goods *x = headgoods;
	while (x != NULL)
	{
		if (x->Rest>0)
		{
			cout << x->Id << '\t';
			cout.width(10);
			cout << x->Name;
			cout.width(10);
			cout << x->BandName;
			cout.width(10);
			cout << x->Price;
			cout.width(10);
			cout << x->Rest;
			cout.width(10);
			cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		}
		x = x->next;
	}
	cout<< "*********************************************\n";
}
void User::ShowRestGoodsInPriceDecreasingOrder()
{
	int Order[50];
	for (int i = 0; i < 50; i++)
		Order[i] = -1;
	OrderGoods ordergoods;
	ordergoods.OrderByPrice(headgoods,Order);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳�򣺼۸���" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	int i = 0;
	while (Order[i] != -1)
		i++;
	Goods *x;
	for(i--;i>=0;i--)
	{
		x = headgoods;
		while (IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4]) != Order[i])
			x = x->next;
		if (x->Rest>0)
		{
			cout << x->Id << '\t';
			cout.width(10);
			cout << x->Name;
			cout.width(10);
			cout << x->BandName;
			cout.width(10);
			cout << x->Price;
			cout.width(10);
			cout << x->Rest;
			cout.width(10);
			cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		}
	}
	cout << "*********************************************\n";
}
void User::ShowRestGoodsInPriceIncreasingOrder()
{
	int Order[50];
	for (int i = 0; i < 50; i++)
		Order[i] = -1;
	OrderGoods ordergoods;
	ordergoods.OrderByPrice(headgoods, Order);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳�򣺼۸�����" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	Goods *x;
	for (int i = 0; Order[i] != -1; i++)
	{
		x = headgoods;
		while (IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4]) != Order[i])
			x = x->next;
		if (x->Rest>0)
		{
			cout << x->Id << '\t';
			cout.width(10);
			cout << x->Name;
			cout.width(10);
			cout << x->BandName;
			cout.width(10);
			cout << x->Price;
			cout.width(10);
			cout << x->Rest;
			cout.width(10);
			cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		}
	}
	cout << "*********************************************\n";
}
void User::ShowRestGoodsInSoldNumOrder()
{
	int Order[50];
	for (int i = 0; i < 50; i++)
		Order[i] = -1;
	OrderGoods ordergoods;
	ordergoods.OrderBySoldNum(SoldList, Order,headgoods);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��չʾ˳���������ȡ�" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout << "Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout.width(10);
	cout << "����";
	cout << "����\n";
	Goods *x;
	int i = 0;
	while (Order[i] != -1)
		i++;
	for (i--; i>=0; i--)
	{
		x = headgoods;
		while (IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4]) != Order[i])
			x = x->next;
		if (x->Rest >0)
		{
			cout << x->Id << '\t';
			cout.width(10);
			cout << x->Name;
			cout.width(10);
			cout << x->BandName;
			cout.width(10);
			cout << x->Price;
			cout.width(10);
			cout << x->Rest;
			cout.width(10);
			cout << SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		}
	}
	cout << "*********************************************\n";
}
void User::searchgoods()
{
	cout.setf(std::ios::left);
	cout << "������Ʒ���ƣ�";
	char destination_goods[20];
	cin >> destination_goods;
	cout << "*********************************************************************\n";
	Goods *x = headgoods;
	int SearchGoodsNum = 0;
	while (x != NULL)
	{
		if (strcmp(x->Name, destination_goods) == 0&&x->Rest>0)
		{
			SearchGoodsNum++;
			if (SearchGoodsNum == 1)
			{
				cout << "ID\t";
				cout.width(10);
				cout << "����";
				cout.width(10);
				cout << "Ʒ��";
				cout.width(10);
				cout << "�۸�";
				cout.width(10);
				cout<<"����";
				cout << "����\n";
			}
			cout << x->Id << '\t';
			cout.width(10);
			cout<< x->Name;
			cout.width(10);
			cout << x->BandName;
			cout.width(10);
			cout << x->Price;
			cout.width(10);
			cout << x->Rest;
			cout<< SoldList[IdTurnNum(x->Id[1], x->Id[2], x->Id[3], x->Id[4])].SoldNum << endl;
		}
		x = x->next;
	}
	if (SearchGoodsNum == 0)
		cout << "��Ʒ�����ڣ�\n";
	cout << "*********************************************************************\n";
}
void User::Showcart()
{
	cout << "*********************************************";
	cout << endl;
	cout.setf(std::ios::left);
	cout << "ID\t";
	cout.width(10);
	cout << "����";
	cout.width(10);
	cout<<"Ʒ��";
	cout.width(10);
	cout << "�۸�";
	cout<<"����\n";
	Goods *x = usercarthead;
	if (x == NULL)
		cout<<"���ﳵΪ�գ�\n";
	else
	while (x != NULL)
	{
		cout << x->Id << '\t';
		cout.width(10);
		cout << x->Name;
		cout.width(10);
		cout << x->BandName;
		cout.width(10);
		cout<< x->Price;
		cout << x->PurchaseNum;
		cout<< endl;
		x = x->next;
	}
	cout << "*********************************************"<<endl;
}
bool User::addcart()
{
	DoFile dofile;
	Goods* addgood=new Goods;
	cout << "������ƷID������:";
	cin >> addgood->Id;
	cin>> addgood->PurchaseNum;
	Goods* x =headgoods;
	Goods* y = usercarthead;
	while (x!= NULL&&strcmp(addgood->Id, x->Id) != 0)
	{
		x = x->next;
	}
	if (x == NULL||x->Rest<=0)
	{
		cout << "*************************************************\n";
		cout<<"��Ʒ������\n";
		cout << "*************************************************\n";
		return 0;
	}
	while (y != NULL&&strcmp(addgood->Id, y->Id) != 0)
	{
		y = y->next;
	}
	if (y != NULL)
	{
		y->PurchaseNum += addgood->PurchaseNum;
		if (y->PurchaseNum > x->Rest)
		{	
			cout<< "*************************************************\n";
			cout << "������������棡��ѡ����ʵĹ���������\n";
			cout << "*************************************************\n";
			return 0;
		}
		dofile.CartInFile(usercarthead, useraccount.Id);
		cout << "*************************************************\n";
		cout << "�Ѿ����빺�ﳵ��\n";
		cout << "*************************************************\n";
		return 1;
	}
	else//���ﳵ���¼���Ʒ
	{
		if (addgood->PurchaseNum > x->Rest)
		{
			cout << "*************************************************\n";
			cout << "������������棡��ѡ����ʵĹ���������\n";
			cout << "*************************************************\n";
			return 0;
		}
		strcpy_s(addgood->Name, x->Name);
		strcpy_s(addgood->BandName, x->BandName);
		addgood->Price = x->Price;
		addgood->next = NULL;
		y = usercarthead;
		if(y==NULL)
		usercarthead= addgood;
		else
		{
			while (y->next != NULL)
				y = y->next;
			y->next = addgood;
		}
		dofile.CartInFile(usercarthead, useraccount.Id);
		cout << "*************************************************\n";
		cout << "�Ѿ����빺�ﳵ��\n";
		cout << "*************************************************\n";
		return 1;
	}
}
bool User::delecart()
{
	DoFile dofile;
	Goods* delegood=new Goods;
	cout << "������ƷID������:";
	cin >> delegood->Id;
	cin >> delegood->PurchaseNum;
	Goods* x= NULL;
	Goods* y = usercarthead;
	while (y != NULL&&strcmp(delegood->Id, y->Id) != 0)
	{
		x = y;
		y = y->next;
	}
	if (y == NULL)
	{
		cout << "*************************************************\n";
		cout << "���ﳵ�����ڸ���Ʒ\n";
		cout << "*************************************************\n";
		return 0;
	}
	if (delegood->PurchaseNum < y->PurchaseNum)
	{
		y->PurchaseNum -= delegood->PurchaseNum;
	}
	else if (x == NULL)//ֻ��һ����Ʒ
	{
			usercarthead = usercarthead->next;
			delete y;
	}
	else 
	{
		x->next = y->next;
		delete y;
	}
	dofile.CartInFile(usercarthead, useraccount.Id);
	cout << "*************************************************\n";
	cout << "ɾ���ɹ���\n";
	cout << "*************************************************\n";
	return 1;
}
bool User::pay()
{
	DoFile dofile;
	Showcart();
	double allpay=0;
	for (Goods * x = usercarthead; x != NULL; x = x->next)
		allpay += x->Price*x->PurchaseNum;
	if (allpay == 0)
	{
		cout << "���ﳵΪ�գ�����ʧ�ܣ�" << endl;
		return 0;
	}
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	cout << "һ����Ҫ����" <<allpay<< "!ȷ�ϸ����밴 1 ��ȡ���밴 0 ��";
	bool key;
	cin >> key;
	cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	cout << "***********" << endl;
	if (key!=1)
	{
		cout << "����ʧ�ܣ�\n";
		cout << "***********" << endl;
		return 0;
	}
	else if (allpay > restmoney)
	{
		cout << "���㣡\n";
		cout << "***********" << endl;
		return 0;
	}
	else 
	{
		Admin ad;
		//���۳��嵥�Ϳ�棨�����ļ�ҲҪ�ģ�
		//�޸Ĺ�����ʷ�ͼ�¼�ջ���ַ
		char thisaddress[80];
		int key=-1;
		if (addresshead != NULL&&key!=1&&key!=0)
		{
			cout << "Ĭ��ʹ���ջ���ַ1:"<<addresshead->address<<"  1.ȷ�� 2.�������õ�ַ 3.ʹ���µ�ַ:";
			cin >> key;
			if (key == 1)
			{
				strcpy_s(thisaddress, addresshead->address);
			}
			else if(key == 2)
				for(int ok=-1;ok!=1;)
			{
				cout << "�������ַ��ţ�";
					int f;
				cin >> f;
				Address *xx = addresshead;
				for (int i = 1; i < f&&xx!=NULL; i++)
					xx = xx->next;
				if (xx == NULL)
				{
					cout << "�����ڸõ�ַ!"<<endl;
				}
				else
				{
					cout << "���ι���ʹ���ջ���ַ:" << f << ':' << xx->address << "  1.ȷ�� 2.ȡ��:";
					cin >> ok;
					if (ok == 1)
						strcpy_s(thisaddress, xx->address);
					else if (ok != 0)
						cout << "��Чָ�" << endl;
				}
			}
			else if (key != 3)
				cout << "���벻�Ϸ���"<<endl;
		}
		if(addresshead==NULL||key==0)
		{
				cout << "�����뱾�ν��׵��ջ���ַ��";
				cin >> thisaddress;
		}
		ShoppingHistory *v = new ShoppingHistory;
		v->goodsnum = 0;
		int t = 0;
		for (Goods *x = usercarthead; x != NULL;)
		{
			strcpy_s(v->goods[t].Id,x->Id);
			strcpy_s(v->goods[t].BandName, x->BandName);
			strcpy_s(v->goods[t].Name, x->Name);
			v->goods[t].Price=x->Price;
			v->goods[t].PurchaseNum = x->PurchaseNum;
			v->goodsnum++;
			t++;
			x = x->next;
		}
		strcpy_s(v->address, thisaddress);
		ShoppingHistory *w = shoppinghistoryhead;
		if (w != NULL)
		{
			while (w->next != NULL)
				w = w->next;
			w->next = v;
		}
		else
			shoppinghistoryhead = v;
		v->next = NULL;
		dofile.ShoppingHistoryInFile(useraccount.Id, shoppinghistoryhead);
		for (Goods *x = usercarthead; x != NULL;)//�ͷŹ��ﳵ�ռ�
		{
			Goods *y = x;
			x = x->next;
			Goods *px = headgoods;//���ͷָ��
			int pn = 0;//�۳��嵥���
			while (strcmp(y->Id, px->Id) != 0)
			{
				px = px->next;
			}
			if (px->Rest < y->PurchaseNum)
			{
				cout << "���Ϊ" << y->Id << "����Ʒ�ϻ����¼ܣ�\n";
				cout << "***********";
				return 0;
			}
			px->Rest -= y->PurchaseNum;
			int in;
			for (in = 0; ad.SoldList[in].SoldNum != 0; in++);
			strcpy_s(ad.SoldList[in].Id, y->Id);
			strcpy_s(ad.SoldList[in].Name, y->Name);
			strcpy_s(ad.SoldList[in].BandName, y->BandName);
			ad.SoldList[in].Price = y->Price;
			ad.SoldList[in].SoldNum = y->PurchaseNum;
			dofile.SoldInFile(useraccount.Id, y);
			delete y;
		}
		restmoney -= allpay;
		dofile.moneyinfile(restmoney,useraccount.Id);
		dofile.GoodsInFile(headgoods);
		usercarthead = NULL;
		 dofile.CartInFile(usercarthead, useraccount.Id);//��չ��ﳵ
		cout << "����ɹ���\n";
		cout << "***********" << endl;
		return 1;
	}
}
bool User::ChangePassword()
{
	DoFile dofile;
	cout << "�����û���:";
	cin >> useraccount.Id;
	cout << "��������:";
	cin >> useraccount.Password;
	int readans = dofile.UserOutFile(useraccount);
	if (readans== 0)
	{
		cout << "�û������ڣ�\n";
		return 0;
	}
	else if (readans == -1)
	{
		cout << "�������\n";
		return 0;
	}
	else
	{
		char OriginalKey[30];
		cout << "������ԭ����:";
		cin >> OriginalKey;
		while (strcmp(OriginalKey, useraccount.Password) != 0)
		{
			cout << "����������������룡\n";
			cout << "������ԭ����:";
			cin >> OriginalKey;
		}
		int NewKeyFinshed = 0;
		while (NewKeyFinshed==0)
		{
			char NewKey[30];
			char ConfirmNewKey[30];
			cout << "������������:";
			cin >> NewKey;
			while (strcmp(NewKey, useraccount.Password) == 0)
			{
				cout << "������Ӧ��ԭ���벻ͬ��\n";
				cout << "������������:";
				cin >> NewKey;
			}
			cout << "���ٴ�ȷ�������룺";
			cin >> ConfirmNewKey;
			if (strcmp(NewKey, ConfirmNewKey)!= 0)
			{
				cout << "�����������벻һ�£�\n";
			}
			else
			{
				strcpy_s(useraccount.Password, NewKey);
				dofile.UserNewKeyInfile(useraccount);
				cout << "�����޸ĳɹ�������\n";
				cout << "���μ��������룡\n";
				NewKeyFinshed = 1;
			}
		}
		return 1;
	}
}
void User::Showrestmoney()
{
	cout << "�������Ϊ��" << restmoney << endl;
	cout << "���ز˵��밴0����ֵ�밴1��";
	int choice;
	cin >> choice;
	if (choice == 1)
		addrestmoney();
}
void User::addrestmoney()
{
	DoFile dofile;
	cout << "�������ֵ��";
	double addmoney;
	cin >> addmoney;
	addmoney = (double)((int)(addmoney * 100) / 10) / 10;
	restmoney += addmoney;
	dofile.moneyinfile(restmoney, useraccount.Id);
}
void User::Addaddress()
{
	Address *newaddress = new Address;
	cout << "�������µ�ַ��";
	cin >> newaddress->address;
	newaddress->next = NULL;
	Address* x = addresshead;
	if (x == NULL)
	{
		addresshead = newaddress;
	}
	else
	{
		while (x->next != NULL)
			x = x->next;
		x->next = newaddress;
	}
	DoFile dofile;
	dofile.AddressInFile(useraccount.Id, addresshead);
	cout << "��ӳɹ���" << endl;
}
void User::Deleaddress()
{
	bool rightkey = 0;
	while (rightkey == 0)
	{
		cout << "�������ɾ����ַ����ţ�";
		int key;
		cin >> key;
		Address *a = NULL;
		Address *b = addresshead;
		int i;
		for (i = 1; i < key; i++)
		{
			if (b == NULL)
			{
				i = key + 1;
			}
			else
			{
				a = b;
				b = b->next;
			}
		}
		if (i == key + 1 || b == NULL)
			cout << "������ţ�" << endl;
		else
		{
			rightkey = 1;
			if (a != NULL)
			{
				a->next = b->next;
				delete b;
			}
			else
			{
				addresshead = addresshead->next;
				delete b;
			}
			cout << "��ɾ���õ�ַ��" << endl;
			DoFile dofile;
			dofile.AddressInFile(useraccount.Id, addresshead);
		}
	}
}
void User::Showaddress()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "�������ջ���ַ��"<<endl;
	int i = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	if (addresshead == NULL)
	{
		cout << "==================================================================================" << endl;
		cout << "��δ����κ��ջ���ַ��" << endl;
		cout << "==================================================================================" << endl;
	}
	else
	{
		cout << "==================================================================================" << endl;
		for (Address* x = addresshead; x != NULL; x = x->next)
		{
			cout << "��ַ" << i << ':' << x->address<<endl;
			i++;
		}
		cout << "=================================================================================="<<endl;
	}
	cout << "0.���ز˵� 1.��ӵ�ַ 2.ɾ����ַ"<<endl;
	int key=-1;
	while (key != 0 && key != 1 && key != 2)
	{
		cout << "�����������";
		cin >> key;
		if (key == 1)
			Addaddress();
		else if (key == 2)
			Deleaddress();
		else if (key != 0)
		{
			cout << "������Ϸ�������" << endl;
		}
	}
}
void User::Deleshoppinghistory()
{
	bool rightkey = 0;
	while (rightkey == 0)
	{
		cout << "�������ɾ����ʷ����ţ�";
		int key;
		cin >> key;
		ShoppingHistory *a = NULL;
		ShoppingHistory *b = shoppinghistoryhead;
		int i;
		for (i = 1; i < key; i++)
		{
			if (b == NULL)
			{
				i = key + 1;
			}
			else
			{
				a = b;
				b = b->next;
			}
		}
		if (i == key + 1 || b == NULL)
			cout << "������ţ�" << endl;
		else
		{
			rightkey = 1;
			if (a != NULL)
			{
				a->next = b->next;
				delete b;
			}
			else
			{
				shoppinghistoryhead = shoppinghistoryhead->next;
				delete b;
			}
			cout << "��ɾ��������ʷ��" << endl;
			DoFile dofile;
			dofile.ShoppingHistoryInFile(useraccount.Id,shoppinghistoryhead);
		}
	}
}
void User::Zeroshoppinghistory()
{
	ShoppingHistory *y = NULL;
	ShoppingHistory *x = shoppinghistoryhead;
	while (x!= NULL)
	{
		y = x;
		x = x->next;
		delete y;
	}
	shoppinghistoryhead = NULL;
	cout << "�������ʷ��" << endl;
	DoFile dofile;
	dofile.ShoppingHistoryInFile(useraccount.Id, NULL);
}
void User::Showshoppinghistory()
{
	cout.setf(std::ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "��������ʷ��" << endl;
	int i = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	if (shoppinghistoryhead == NULL)
	{
		cout << "==================================================================================" << endl;
		cout << "���޹�����ʷ��" << endl;
		cout << "==================================================================================" << endl;
	}
	else
	{
		int n = 1;
		cout << "==================================================================================" << endl;
		for (ShoppingHistory* x = shoppinghistoryhead; x != NULL; x = x->next)
		{
			cout << n << ":" << endl;
			cout << "***********************************************************************************" << endl;
			cout << "ID\t";
			cout.width(10);
			cout << "����";
			cout.width(10);
			cout << "Ʒ��";
			cout.width(10);
			cout << "�۸�";
			cout << "����"<<endl;
			for (int j = 0; j < x->goodsnum; j++)
			{
				cout << x->goods[j].Id << '\t';
				cout.width(10);
				cout << x->goods[j].Name;
				cout.width(10);
				cout << x->goods[j].BandName;
				cout.width(10);
				cout << x->goods[j].Price;
				cout << x->goods[j].PurchaseNum << endl;
			}
			cout << "�ջ��ַ��" << x->address << endl;
			cout << "***********************************************************************************" << endl;
			n++;
		}
		cout << "==================================================================================" << endl;
	}
	cout << "0.���ز˵� 1.ɾ��������ʷ 2.��չ�����ʷ" << endl;
	int key=-1;
	while (key != 0 && key != 1 && key != 2)
	{
		cout << "�����������";
		cin >> key;
		if (key == 1)
			Deleshoppinghistory();
		else if (key == 2)
			Zeroshoppinghistory();
		else if (key != 0)
		{
			cout << "������Ϸ�������" << endl;
		}
	}
}
//****************************************************************************************�����๦��
Opera::Opera()
{
	op = 'N';
}
void Opera::OperateChangeKey()
{
	User user;
	user.ChangePassword();
}
void Opera::OperateUserregister()
{
	User user;
	while (user.Register() == 0);
	while (user.Log == 1)//�û���½��
	{
		int opera;
		user.ShowLoginInformation();
		cin >> opera;
		if (opera == 1)
		{
			user.Logout();
		}
		else if (opera == 2)
		{
			user.ShowRestGoods();
		}
		else if (opera == 3)
		{
			user.searchgoods();
		}
		else if (opera == 6)
		{
			user.Showcart();
		}
		else if (opera == 4)
		{
			user.addcart();
		}
		else if (opera == 5)
		{
			user.delecart();
		}
		else if (opera == 7)
		{
			user.pay();
		}
		else if (opera == 9)
		{
			user.Showrestmoney();
		}
		else if (opera == 10)
		{
			user.Showaddress();
		}
		else if (opera == 8)
		{
			user.Showshoppinghistory();
		}
		else
			cout << "�Ƿ�ָ�" << endl;
	}
}
void Opera::OperateUser()
{
	User user;
	while (user.Log == 0)
		user.Login();
	while (user.Log == 1)//�û���½��
	{
		int opera;
		user.ShowLoginInformation();
		cin >> opera;
		if (opera == 1)
		{
			user.Logout();
		}
		else if (opera == 2)
		{
			user.ShowRestGoods();
		}
		else if (opera == 3)
		{
			user.searchgoods();
		}
		else if (opera == 6)
		{
			user.Showcart();
		}
		else if (opera == 4)
		{
			user.addcart();
		}
		else if (opera == 5)
		{
			user.delecart();
		}
		else if (opera == 7)
		{
			user.pay();
		}
		else if (opera == 9)
		{
			user.Showrestmoney();
		}
		else if (opera == 10)
		{
			user.Showaddress();
		}
		else if (opera == 8)
		{
			user.Showshoppinghistory();
		}
		else
			cout << "�Ƿ�ָ�" << endl;
	}
}
void Opera::OperateAdmin()
{
	Admin admin;
	while (admin.Log == 0)
		admin.Login();
	while (admin.Log == 1)//�û���½��
	{
		int opera;
		admin.ShowLoginInformation();
		cin >> opera;
		if (opera == 1)
		{
			admin.Logout();
		}
		else if (opera == 2)
		{
			admin.ShowRestGoods();
		}
		else if (opera == 3)
		{
			admin.AddGoods();
		}
		else if (opera == 4)
		{
			admin.DeleGoods();
		}
		else if (opera == 5)
		{
			admin.ChangeGoods();
		}
		else if (opera == 6)
		{
			admin.ShowSoldList();
		}
		else cout << "���벻�Ϸ���" << endl;
	}
}
void Opera::OperateMenu()
{
	while (op != '0')
	{
		system("cls");
		Showmenu();
		cin >> op;
		if (op == '1')//�û���½
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			OperateUser();
		}
		else if (op == '2')//�û�ע��
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			OperateUserregister();
		}
		else if (op == '3')//����Ա��½
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			OperateAdmin();
		}
		else if (op == '4')
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			OperateChangeKey();
		}
		else if (op == '0')
		{
			cout << "�ټ���";
		}

		else
		{
			cout << "���벻�Ϸ�!" << endl;
		}
		Sleep(1000);
	}
}
//*******************************************************************************************
int main()
{
	Opera opera;
	opera.OperateMenu();
	return 0;
}