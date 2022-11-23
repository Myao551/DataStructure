#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//存储信息记录的结构体
struct Information
{
	string ID;   //身份证
	string Name;  //名字
	string FlightNumber;    //航班号
	string Date;  //飞行日期
	int Distance;   //飞行距离
	Information* next;   //链地址法解决冲突
};

//散列表
Information record[500];   

//函数声明
int Hash(string ID);
int CreateHashTable1(Information record[500]);
void CreateHashTable2(Information record[500]);
int FindVIP(Information record[500]);
void print1(Information record[500]);
void print2(Information record[500]);

int main()
{
	CreateHashTable2(record);
	CreateHashTable1(record);
	FindVIP(record);
	return 0;
}

//Hash函数
int Hash(string ID)
{
	int t;
	//取身份证后四位
	t = int(ID[14]-'0') * 1000 + int(ID[15]-'0') * 100 + int(ID[16]-'0') * 10 + int(ID[17]-'0');
	t = t % 211;
	return t;
}

//记录散列表中某个位置是否已经存入信息
int visited1[500]; //开放地址法
int visited2[500];  //链地址法

//建立散列表（开放地址法）
int CreateHashTable1(Information record[500])
{
	cout << "开放地址法发生冲突如下:"<<endl;
	int k = 1, s = 0;
	int location;
	ifstream in;
	//n,t临时记录当前输入的姓名和身份证
	string n,t;
	for (int i = 0; i <500; i++)
	{
		visited1[i] = 0;
	}
	in.open("source.txt");
	if (!in)
	{
		cout << "打开 source.txt 失败!";
		exit(1);
	}
	while (!in.eof())
	{   
		s = 0;
		in >> n;
		in >> t;
		location = Hash(t);   //利用hash函数确定位置
		//如果确定的位置为空，则直接存入信息
		if (visited1[location] == 0)
		{
			record[location].ID = t;
			record[location].Name = n;
			in >> record[location].FlightNumber;
			in >> record[location].Date;
			in >> record[location].Distance;
			visited1[location] = 1;    //存入信息后将标志置为1
		}
		//若当前位置已经存入信息
		else
		{
			s++;
			//线性探测法将位置后移确定
			while (visited1[location] == 1)
			{
				location = (location + 17) % 500;
				s++;
			}
			record[location].ID = t;
			record[location].Name = n;
			in >> record[location].FlightNumber;
			in >> record[location].Date;
			in >> record[location].Distance;
			visited1[location] = 1;   //将存入信息的位置置为1
		}
		if (s) 
		{
			cout << "第" << k << "次解决冲突重定位了" << s << "次" << endl;
			k++;
		}
	}
	in.close();
	return 1;
}

//链地址法建立散列表
void CreateHashTable2(Information record[500])
{
	cout << "链地址法发生冲突如下:" << endl;
	Information* q, * p;
	int k = 1, s = 0;
	int location;
	ifstream in;
	//n,t临时记录当前输入的姓名和身份证
	string n, t;
	for (int i = 0; i < 500; i++)
	{
		visited2[i] = 0;
	}
	in.open("source.txt");
	if (!in)
	{
		cout << "打开 source.txt 失败!";
		exit(1);
	}
	while (!in.eof())
	{
		s = 0;
		in >> n;
		in >> t;
		location = Hash(t);   //利用hash函数确定位置
		//如果确定的位置为空，则直接存入信息
		if (visited2[location] == 0)
		{
			q = &record[location];
			p = new Information;
			p->Name = n;
			p->ID = t; 
			in >> p->FlightNumber;
			in >> p->Date;
			in >> p->Distance;
			q->next = p;
			q = p;
			p->next = NULL;
			visited2[location] = 1;
		}
		else
		{
			s++;
			q = &record[location];
			p = q->next;
			while (p)
			{
				q = q->next;
				p = p->next;
				s++;
			}
			p = new Information;
			p->Name = n;
			p->ID = t;
			in >> p->FlightNumber;
			in >> p->Date;
			in >> p->Distance;
			q->next = p;
			q = p;
			p->next = NULL;
		}
		if (s)
		{
			cout << "第" << k << "次解决冲突重定位了" << s << "次" << endl;
			k++;
		}
	}
}

//发掘vip客户
int FindVIP(Information record[500])
{
	int flag[500];  //另一个标记数组，用来保证同一个身份证的信息在一次循环中全部访问
	for (int i = 0; i < 500; i++)
	{
		flag[i] = 0;
	}
	for (int i = 0; i < 500; i++)
	{
		int ts = 0;   //临时记录一个人的乘机次数
		int ds = 0;    //临时记录一个人的飞行总里程
		string tempID;   //临时记录一个人的身份证号
		//在散列表中寻找存入了信息但是没有确定是否为vip的客户
		if (visited1[i] == 1&&flag[i]==0)
		{
			tempID = record[i].ID;  //记录当前身份证
			ts++;  //找到一个记录飞行次数加一
			ds = ds + record[i].Distance;   //总里程加上当前记录
			flag[i] = 1;    //表示已经查找过该条记录
			//寻找剩余记录中同一个人的记录
			for (int j = i+1; j < 500; j++)
			{
				if (tempID == record[j].ID)//如果身份证相同，那么则是同一个人的飞行记录
				{
					ts++;     //飞行次数再加一
					ds = ds + record[j].Distance;   //飞行距离累加
					flag[j] = 1;   //flag置为1表示已经查找过
				}
			}
			//平均每次飞行里程超过800km且该段时间内乘机次数大于等于4确定为VIP客户
			if (ds / ts >= 800&&ts>3)
			{
				cout << record[i].Name << "是VIP客户 " << "总里程为:" << ds << "km 乘机次数为:" << ts<<endl;
			}
		}
	}
	return 1;
}

//输出散列表中信息(开放地址法）
void print1(Information record[500])
{
	cout << "开放地址法客户乘坐记录为:" << endl;
	for (int i = 0; i < 500; i++)
	{
		if (visited1[i] == 1)
		{
			cout <<"姓名:" << record[i].Name << ' ' <<"身份证号:" << record[i].ID << ' ' <<"航班号:" << record[i].FlightNumber << ' ' <<"日期:" << record[i].Date << ' ' <<"飞行距离:" << record[i].Distance <<"km" << endl;
		}
	}
}

void print2(Information record[500])
{
	Information* p;
	cout << "链地址法客户乘坐记录为:" << endl;
	for (int i = 0; i < 500; i++)
	{
		if (visited2[i] == 1)
		{
			p = record[i].next;
			while (p)
			{
				cout << "姓名:" << p->Name << ' ' << "身份证号:" << p->ID << ' ' << "航班号:" << p->FlightNumber << ' ' << "日期:" << p->Date << ' ' << "飞行距离:" << p->Distance << "km" << endl;
				p = p->next;
			}
		}
	}
}