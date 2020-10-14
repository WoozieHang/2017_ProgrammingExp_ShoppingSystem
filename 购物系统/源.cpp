#include<iostream>
#include<cstring>
#include <iomanip>
#include<Windows.h>
using namespace std;
//已经拓展的功能：颜色、余额、修改密码、清屏、商品清单中商品的销售量展示、商品按id、价格、销售量顺序展示、常用收货地址查看、常用收货地址添加、常用收货地址删除、
//正在实现：下架后查看不到下架商品
//拓展功能:
void Showmenu()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY| FOREGROUND_RED);
	cout << "滑天下之大稽佬网：www.BigGay.com" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "===============================================================================================================" << endl;
	cout << "0.退出系统		1.用户登陆		2.用户注册		3.管理员登陆		4.修改密码		" ;
	cout << "===============================================================================================================" << endl;
	cout << "输入操作：";
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
//**********************************************************************************管理员类声明
class Admin
{
	AdminAccount adminaccount;
	int SortNum;//商品种类数
public:
	Admin();
	bool Login();//返回登陆结果，成功1，失败0
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
	bool Log;//标记登陆状态
	Goods* headgoods;
	Goods* lastgoods;
	Goods SoldList[1000];
};
//*************************************************************************************用户类声明
class User
{
	double restmoney;
public:
	User();
	ShoppingHistory* shoppinghistoryhead;//购物历史-链表
	Address* addresshead; //收获地址- 链表
	Goods* usercarthead;//购物车-链表
	UserAccount useraccount;
	Goods* headgoods;
	Goods* lastgoods;
	Goods SoldList[1000];
	bool Log;//标记登陆状态
	bool Login();//返回登陆结果，成功1，失败0
	bool Register();
	void ShowLoginInformation();
	void Logout();
	void ShowRestGoods();
	void ShowRestGoodsInIdOrder();//按id顺序展示商品
	void ShowRestGoodsInPriceIncreasingOrder();//按价格从低到高展示
	void ShowRestGoodsInPriceDecreasingOrder();//价格从高到低展示
	void ShowRestGoodsInSoldNumOrder();//按销售量从多到少顺序展示
	void searchgoods();
	void Showcart();
	bool addcart();//购物车-链表
	bool delecart();
	bool pay();
	bool ChangePassword();//修改密码
	void Showrestmoney();//余额
	void addrestmoney();
	void Showshoppinghistory();
	void Deleshoppinghistory();
	void Zeroshoppinghistory();
	void Showaddress();
	void Addaddress();
	void Deleaddress();
};
//**************************************************************************************文件类说明
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
//****************************************************************************************操作类说明
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
//*******************************************************************************************商品排序类说明
class OrderGoods
{
public:
	void OrderByPrice(Goods *head,int Order[]);//升序
	void OrderBySoldNum(Goods SoldList[],int Order[],Goods *head);
};
//********************************************************************************************文件类功能
void DoFile::GoodsInFile(Goods *headgoods)
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\库存.txt", "w");
	fputs("ID\t名称\t品牌\t价格\t数量\n", fp);
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
	err = fopen_s(&fp, "d:\\库存.txt", "r");
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
	err = fopen_s(&fp, "d:\\已售清单.txt", "r");
	char information[1000][50];
	int i = 0;
	for (; !feof(fp); i++)
	{
		fgets(information[i], 48, fp);
	}//完成读取
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
	err = fopen_s(&fp, "d:\\已售清单.txt", "r+");
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
int DoFile::UserOutFile(UserAccount auseraccount)//账号不存在返回0，密码错误返回-1，否则返回1
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\用户.txt", "r");
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
void DoFile::UserNewKeyInfile(UserAccount NewUserAccount)//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<to be done！！！
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, "d:\\用户.txt", "r");
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
	err1 = fopen_s(&fp1, "d:\\用户.txt", "w");
	fprintf(fp1, "用户名\t密码\n");
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
	err = fopen_s(&fp, "d:\\用户.txt", "a");
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
	fputs("ID\t名称\t品牌\t价格\t数量\n", fp);
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
	while (information[j][0] == 'F'||information[j][0]=='\n')//有信息未提取
	{
		if (information[j][0] == 'F')
		{
			ShoppingHistory *p = new ShoppingHistory;
			p->goodsnum = 0;
			for (int q = 0; information[j][0] == 'F'; q++)//提取商品信息
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
			strcpy_s(p->address, information[j]);//提取地址信息
			if (head == NULL)//头记录
			{
				head = p;
				tail = p;
				tail->next = NULL;
			}
			else//记录
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
//*********************************************************************************************商品排序类功能
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
	//对allgood排序
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
	//对allgood排序
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
//***************************************************************************************管理员类功能
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
	cout << "输入管理员ID：";
		char ID[20];
		cin >> ID;
		cout << "输入管理员密码：";
		char key[20];
		cin >> key;
		cout << "********" << endl;
		if (strcmp(ID, adminaccount.Id) == 0 && strcmp(key, adminaccount.Password) == 0)
		{
			cout << "登陆成功！\n********\n";
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
			cout << "登录失败,请重试！\n********\n";
			Showmenu();
			return 0;
		}
}
void Admin::Logout()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "已退出登陆！\n";
	Log = 0;
}
void Admin::ShowRestGoods()
{
	char choice = '5';
	cout << "请选择查看顺序 1.按Id顺序查看 2.按价格从低到高顺序查看 3.按价格从高到低顺序查看 4.按销售量从高到低顺序查看" << endl;
	while (choice != '1'&&choice != '2'&&choice != '3'&&choice != '4')
	{
		cout << "查看顺序为：";
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
			cout << "输入不合法!请重新输入" << endl;
	}
}
void Admin::ShowRestGoodsInIdOrder()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "【展示顺序：Id顺序】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "【展示顺序：价格降序】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "【展示顺序：价格升序】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "【展示顺序：销量优先】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "请输入添加的商品的品牌：";
	cin >> addgoodbandname;
	cout << "请输入添加的商品的名称：";
	cin >> addgoodname;
	Goods *x = headgoods;
	while (x != NULL&&(strcmp(x->BandName, addgoodbandname) != 0||strcmp(x->Name, addgoodname) != 0))
		x = x->next;
	if (x != NULL)
	{
		if (x->Rest != -1)
		{
			cout << "该商品已存在，请输入添加的数量：";
			int AddNum;
			cin >> AddNum;
			x->Rest += AddNum;
			dofile.GoodsInFile(headgoods);
			cout << "添加成功！！" << endl;
		}
		else//下架过的商品
		{
			cout << "该商品已下架，是否选择上架？1是 0否  ：" ;
			int resale;
			cin >> resale;
			if (resale == 0)
				cout << "添加商品操作取消！" << endl;
			else
			{
				cout << "请输入添加的数量：" ;
				int AddNum;
				cin >> AddNum;
				x->Rest = AddNum;
				dofile.GoodsInFile(headgoods);
				cout << "添加成功！！" << endl;
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
		cout << "输入新商品价格：";
		double newprice;
		cin >> newprice;
		p->Price = newprice;
		cout << "输入新商品的数量：";
		int newnum;
		cin >> newnum;
		p->Rest = newnum;
		x->next = p;
		lastgoods = p;
		lastgoods->next = NULL;
		dofile.GoodsInFile(headgoods);
		cout << "添加成功！！" << endl;
	}
	//需要更改库存文件
}
bool Admin::ChangeGoods()
{
	DoFile dofile;
	char changegoodname[15];
	char changebandname[15];
	cout << "请输入修改的商品的品牌：";
	cin >> changebandname;
	cout << "请输入修改的商品的名称：";
	cin >> changegoodname;
	Goods *x = headgoods;
	while ((strcmp(x->BandName, changebandname) != 0 || strcmp(x->Name, changegoodname) != 0) && x != NULL)
		x = x->next;
	if (x != NULL)
	{
		cout << "\n该商品的id是:" << x->Id << "\n1.修改价格  2.修改数量\n请选择对该商品要进行的操作:";
		int key = 0;
		while (key != 1 && key != 2)
		{
			cin >> key;
			if (key != 1 && key != 2)
				cout << "请输入有效操作！" << endl << "请选择对该商品要进行的操作:";
		}
		if (key == 1)
		{
			cout << "该商品原价是：" << x->Price << "\n请输入更改后的价格：";
			cin >> x->Price;
		}
		else
		{
			cout << "该商品的数量是：" << x->Rest << "\n请输入修改后的数量：";
			cin >> x->Rest;
		}
		//change file
		dofile.GoodsInFile(headgoods);
		return 1;
	}
	else
	{
		cout << "\n不存在该商品！" << endl;
		return 0;
	}
}
bool Admin::DeleGoods()
{
	DoFile dofile;
	char delegoodname[15];
	char delebandname[15];
	cout << "请输入删除的商品的品牌：";
	cin >> delebandname;
	cout << "请输入删除的商品的名称：";
	cin >> delegoodname;
	Goods *x = headgoods;
	while ((strcmp(x->BandName, delebandname) != 0 || strcmp(x->Name, delegoodname) != 0) && x != NULL)
		x = x->next;
	if (x != NULL)
	{
		cout << "\n该商品的id是:" << x->Id << "\n是否选择下架？1.是  2.否\n请选择对该商品要进行的操作:";
		int key = 0;
		while (key != 1 && key != 2)
		{
			cin >> key;
			if (key != 1 && key != 2)
				cout << "请输入有效操作！" << endl << "请选择对该商品要进行的操作:";
		}
		if (key == 1)
		{
			x->Rest = -1;
		}
		else
		{
			cout << "下架取消！" << endl;
			return 0;
		}
		//change file
		dofile.GoodsInFile(headgoods);
		cout << "下架成功！！" << endl;
		return 1;
	}
	else
	{
		cout << "\n商品不存在！\n";
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
	cout << "名称";
	cout.width(10);
	cout<<"品牌";
	cout.width(10);
	cout << "价格";
	cout<<"数量\n";
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
	cout << "1.注销登录 2.查询商品 3.增加商品 4.删除商品 5.修改商品数量 6售货清单\n";
	cout << "====================================================================================================\n";
	cout << "输入操作：";
}
//*******************************************************************************************用户类功能
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
	cout << "1.注销登录 2.查看商品 3.商品搜索 4.添加商品至购物车 5.删除购物车商品 6.查看购物车\n7.结账. 8.购物历史 9.我的余额 10.我的收货地址\n";
	cout << "====================================================================================================\n";
	cout << "输入操作：";
}
bool User::Login()
{
	DoFile dofile;
	cout << "输入用户名：";
	cin >> useraccount.Id;
	cout << "输入密码：";
	cin >> useraccount.Password;
	int readans=dofile.UserOutFile(useraccount);
	if (readans == 0)
	{
		cout << "用户名不存在，请检查输入是否正确！" << endl;
		return 0;
	}
	else if (readans == -1)
	{
		cout << "密码错误，请检查输入是否正确！" << endl;
		return 0;
	}
	else
	{
		cout << "********" << endl;
		cout << "登陆成功！"<<endl;
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
	cout << "输入用户名:";
	cin >> useraccount.Id;
	cout << "输入密码:";
	cin >> useraccount.Password;
	int readans = dofile.UserOutFile(useraccount);
	if (readans != 0)
	{
		cout << "用户已存在！\n";
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
		cout << "登陆成功！"<<endl;
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
	cout << "注销成功！\n";
	cout << "********" << endl;
	Log = 0;
}
void User::ShowRestGoods()
{
	char choice = '5';
	cout << "请选择查看顺序 1.按Id顺序查看 2.按价格从低到高顺序查看 3.按价格从高到低顺序查看 4.按销售量从高到低顺序查看" << endl;
	while (choice != '1'&&choice != '2'&&choice != '3'&&choice != '4')
	{
		cout << "查看顺序为：" ;
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
			cout << "输入不合法!请重新输入" << endl;
	}
}
void User::ShowRestGoodsInIdOrder()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "【展示顺序：Id顺序】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "【展示顺序：价格降序】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "【展示顺序：价格升序】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "【展示顺序：销量优先】" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "*********************************************";
	cout.setf(std::ios::left);
	cout << endl;
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout << "品牌";
	cout.width(10);
	cout << "价格";
	cout.width(10);
	cout << "数量";
	cout << "销量\n";
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
	cout << "输入商品名称：";
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
				cout << "名称";
				cout.width(10);
				cout << "品牌";
				cout.width(10);
				cout << "价格";
				cout.width(10);
				cout<<"数量";
				cout << "销量\n";
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
		cout << "商品不存在！\n";
	cout << "*********************************************************************\n";
}
void User::Showcart()
{
	cout << "*********************************************";
	cout << endl;
	cout.setf(std::ios::left);
	cout << "ID\t";
	cout.width(10);
	cout << "名称";
	cout.width(10);
	cout<<"品牌";
	cout.width(10);
	cout << "价格";
	cout<<"数量\n";
	Goods *x = usercarthead;
	if (x == NULL)
		cout<<"购物车为空！\n";
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
	cout << "输入商品ID和数量:";
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
		cout<<"商品不存在\n";
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
			cout << "购买量超过库存！请选择合适的购买数量！\n";
			cout << "*************************************************\n";
			return 0;
		}
		dofile.CartInFile(usercarthead, useraccount.Id);
		cout << "*************************************************\n";
		cout << "已经加入购物车！\n";
		cout << "*************************************************\n";
		return 1;
	}
	else//购物车中新加商品
	{
		if (addgood->PurchaseNum > x->Rest)
		{
			cout << "*************************************************\n";
			cout << "购买量超过库存！请选择合适的购买数量！\n";
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
		cout << "已经加入购物车！\n";
		cout << "*************************************************\n";
		return 1;
	}
}
bool User::delecart()
{
	DoFile dofile;
	Goods* delegood=new Goods;
	cout << "输入商品ID和数量:";
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
		cout << "购物车不存在该商品\n";
		cout << "*************************************************\n";
		return 0;
	}
	if (delegood->PurchaseNum < y->PurchaseNum)
	{
		y->PurchaseNum -= delegood->PurchaseNum;
	}
	else if (x == NULL)//只有一个商品
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
	cout << "删除成功！\n";
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
		cout << "购物车为空！交易失败！" << endl;
		return 0;
	}
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	cout << "一共需要付款" <<allpay<< "!确认付款请按 1 ，取消请按 0 ：";
	bool key;
	cin >> key;
	cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	cout << "***********" << endl;
	if (key!=1)
	{
		cout << "付款失败！\n";
		cout << "***********" << endl;
		return 0;
	}
	else if (allpay > restmoney)
	{
		cout << "余额不足！\n";
		cout << "***********" << endl;
		return 0;
	}
	else 
	{
		Admin ad;
		//该售出清单和库存（包括文件也要改）
		//修改购物历史和记录收货地址
		char thisaddress[80];
		int key=-1;
		if (addresshead != NULL&&key!=1&&key!=0)
		{
			cout << "默认使用收货地址1:"<<addresshead->address<<"  1.确认 2.其他常用地址 3.使用新地址:";
			cin >> key;
			if (key == 1)
			{
				strcpy_s(thisaddress, addresshead->address);
			}
			else if(key == 2)
				for(int ok=-1;ok!=1;)
			{
				cout << "请输入地址序号：";
					int f;
				cin >> f;
				Address *xx = addresshead;
				for (int i = 1; i < f&&xx!=NULL; i++)
					xx = xx->next;
				if (xx == NULL)
				{
					cout << "不存在该地址!"<<endl;
				}
				else
				{
					cout << "本次购物使用收货地址:" << f << ':' << xx->address << "  1.确认 2.取消:";
					cin >> ok;
					if (ok == 1)
						strcpy_s(thisaddress, xx->address);
					else if (ok != 0)
						cout << "无效指令！" << endl;
				}
			}
			else if (key != 3)
				cout << "输入不合法！"<<endl;
		}
		if(addresshead==NULL||key==0)
		{
				cout << "请输入本次交易的收货地址：";
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
		for (Goods *x = usercarthead; x != NULL;)//释放购物车空间
		{
			Goods *y = x;
			x = x->next;
			Goods *px = headgoods;//库存头指针
			int pn = 0;//售出清单序号
			while (strcmp(y->Id, px->Id) != 0)
			{
				px = px->next;
			}
			if (px->Rest < y->PurchaseNum)
			{
				cout << "序号为" << y->Id << "的商品断货或下架！\n";
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
		 dofile.CartInFile(usercarthead, useraccount.Id);//清空购物车
		cout << "付款成功！\n";
		cout << "***********" << endl;
		return 1;
	}
}
bool User::ChangePassword()
{
	DoFile dofile;
	cout << "输入用户名:";
	cin >> useraccount.Id;
	cout << "输入密码:";
	cin >> useraccount.Password;
	int readans = dofile.UserOutFile(useraccount);
	if (readans== 0)
	{
		cout << "用户不存在！\n";
		return 0;
	}
	else if (readans == -1)
	{
		cout << "密码错误！\n";
		return 0;
	}
	else
	{
		char OriginalKey[30];
		cout << "请输入原密码:";
		cin >> OriginalKey;
		while (strcmp(OriginalKey, useraccount.Password) != 0)
		{
			cout << "密码错误请重新输入！\n";
			cout << "请输入原密码:";
			cin >> OriginalKey;
		}
		int NewKeyFinshed = 0;
		while (NewKeyFinshed==0)
		{
			char NewKey[30];
			char ConfirmNewKey[30];
			cout << "请输入新密码:";
			cin >> NewKey;
			while (strcmp(NewKey, useraccount.Password) == 0)
			{
				cout << "新密码应与原密码不同！\n";
				cout << "请输入新密码:";
				cin >> NewKey;
			}
			cout << "请再次确认新密码：";
			cin >> ConfirmNewKey;
			if (strcmp(NewKey, ConfirmNewKey)!= 0)
			{
				cout << "两次输入密码不一致！\n";
			}
			else
			{
				strcpy_s(useraccount.Password, NewKey);
				dofile.UserNewKeyInfile(useraccount);
				cout << "密码修改成功！！！\n";
				cout << "请牢记您的密码！\n";
				NewKeyFinshed = 1;
			}
		}
		return 1;
	}
}
void User::Showrestmoney()
{
	cout << "您的余额为：" << restmoney << endl;
	cout << "返回菜单请按0，充值请按1：";
	int choice;
	cin >> choice;
	if (choice == 1)
		addrestmoney();
}
void User::addrestmoney()
{
	DoFile dofile;
	cout << "请输入充值金额：";
	double addmoney;
	cin >> addmoney;
	addmoney = (double)((int)(addmoney * 100) / 10) / 10;
	restmoney += addmoney;
	dofile.moneyinfile(restmoney, useraccount.Id);
}
void User::Addaddress()
{
	Address *newaddress = new Address;
	cout << "请输入新地址：";
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
	cout << "添加成功！" << endl;
}
void User::Deleaddress()
{
	bool rightkey = 0;
	while (rightkey == 0)
	{
		cout << "请输入待删除地址的序号：";
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
			cout << "错误序号！" << endl;
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
			cout << "已删除该地址！" << endl;
			DoFile dofile;
			dofile.AddressInFile(useraccount.Id, addresshead);
		}
	}
}
void User::Showaddress()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "【常用收货地址】"<<endl;
	int i = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	if (addresshead == NULL)
	{
		cout << "==================================================================================" << endl;
		cout << "暂未添加任何收货地址！" << endl;
		cout << "==================================================================================" << endl;
	}
	else
	{
		cout << "==================================================================================" << endl;
		for (Address* x = addresshead; x != NULL; x = x->next)
		{
			cout << "地址" << i << ':' << x->address<<endl;
			i++;
		}
		cout << "=================================================================================="<<endl;
	}
	cout << "0.返回菜单 1.添加地址 2.删除地址"<<endl;
	int key=-1;
	while (key != 0 && key != 1 && key != 2)
	{
		cout << "请输入操作：";
		cin >> key;
		if (key == 1)
			Addaddress();
		else if (key == 2)
			Deleaddress();
		else if (key != 0)
		{
			cout << "请输入合法操作！" << endl;
		}
	}
}
void User::Deleshoppinghistory()
{
	bool rightkey = 0;
	while (rightkey == 0)
	{
		cout << "请输入待删除历史的序号：";
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
			cout << "错误序号！" << endl;
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
			cout << "已删除该条历史！" << endl;
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
	cout << "已清空历史！" << endl;
	DoFile dofile;
	dofile.ShoppingHistoryInFile(useraccount.Id, NULL);
}
void User::Showshoppinghistory()
{
	cout.setf(std::ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "【购买历史】" << endl;
	int i = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	if (shoppinghistoryhead == NULL)
	{
		cout << "==================================================================================" << endl;
		cout << "暂无购买历史！" << endl;
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
			cout << "名称";
			cout.width(10);
			cout << "品牌";
			cout.width(10);
			cout << "价格";
			cout << "数量"<<endl;
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
			cout << "收获地址：" << x->address << endl;
			cout << "***********************************************************************************" << endl;
			n++;
		}
		cout << "==================================================================================" << endl;
	}
	cout << "0.返回菜单 1.删除购买历史 2.清空购买历史" << endl;
	int key=-1;
	while (key != 0 && key != 1 && key != 2)
	{
		cout << "请输入操作：";
		cin >> key;
		if (key == 1)
			Deleshoppinghistory();
		else if (key == 2)
			Zeroshoppinghistory();
		else if (key != 0)
		{
			cout << "请输入合法操作！" << endl;
		}
	}
}
//****************************************************************************************操作类功能
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
	while (user.Log == 1)//用户登陆后
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
			cout << "非法指令！" << endl;
	}
}
void Opera::OperateUser()
{
	User user;
	while (user.Log == 0)
		user.Login();
	while (user.Log == 1)//用户登陆后
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
			cout << "非法指令！" << endl;
	}
}
void Opera::OperateAdmin()
{
	Admin admin;
	while (admin.Log == 0)
		admin.Login();
	while (admin.Log == 1)//用户登陆后
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
		else cout << "输入不合法！" << endl;
	}
}
void Opera::OperateMenu()
{
	while (op != '0')
	{
		system("cls");
		Showmenu();
		cin >> op;
		if (op == '1')//用户登陆
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			OperateUser();
		}
		else if (op == '2')//用户注册
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			OperateUserregister();
		}
		else if (op == '3')//管理员登陆
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
			cout << "再见！";
		}

		else
		{
			cout << "输入不合法!" << endl;
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