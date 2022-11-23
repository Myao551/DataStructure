#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

//区块链结构体
struct LNode
{
	//编号
	int code=0;
	//字符串信息
	string information="\0";
	//校验码
	int checkcode=0;
	LNode* next;
};

typedef LNode* LinkList;

//函数声明
void CreatList(LinkList& L);
void IsCorrect(LinkList L);
void Addinformation(LinkList& L, int s);
void ChangeInformation(LinkList& L, int i);
void DestroyList(LinkList& L);

int main()
{
	LinkList L;
	CreatList(L);
	IsCorrect(L);
	Addinformation(L, 3);
	ChangeInformation(L, 5);
	LNode* p;
	p = L->next;
	cout << "区块链信息为:";
	while (p)
	{
		cout << endl;
		cout << p->code << ' ' << p->information << ' ' << p->checkcode << ' ';
		p = p->next;
	}
	cout << endl;
	IsCorrect(L);
	DestroyList(L);
}

//创建区块链
void CreatList(LinkList& L)
{
	int i = 0;
	ifstream in;
	in.open("data.txt");
	if (!in)
	{
		cout << "打开data.txt文件失败:" << endl;
		exit(1);
	}
	LNode* q, * p;
	q = L=new LNode;
	//flag用于判断是否为第一个节点
	bool flag = 1;
	while (!in.eof())
	{
		p = new LNode;
		p->code = i;
		i++;
		in >> p->information;
		for (unsigned int size=0; size < p->information.size(); size++)
		{
			p->checkcode += (int(p->information[size]));
			p->checkcode %= 113;
		}
		if (flag == 0)
		{
			p->checkcode += q->checkcode;
			p->checkcode += p->code;
			p->checkcode %= 113;
		}
		else
		{
			flag = 0;
		}
		q->next=p;
		q = p;
	}
	p->next = NULL;
	in.close();
	cout << "区块链创建完毕!"<<endl;
}

//判断区块链是否有效
void IsCorrect(LinkList L)
{
	LNode* p ,*q;
	q = L;
	p = q->next;
	int i = 0;
	//检查链表每一个节点
	while (p)
	{
		//checkcode表示计算得到的正确校验码
		int checkcode = 0;
		for (unsigned int size = 0; size < p->information.size(); size++)
		{
			checkcode += int(p->information[size]);
			checkcode %= 113;
		}
		if (i!=0)
		{
			checkcode += q->checkcode;
			checkcode += p->code;
			checkcode %= 113;
		}
		if (checkcode != p->checkcode)
		{
			cout << "首个无效节点编号为" << i<<endl;
			return;
		}
		i++;
		q = p;
		p = p->next;
	}
	//循环结束表示链表中不存在无效节点
	cout << "该链表有效"<<endl;
	return;
}

//向链表中添加信息（在第s个节点后添加信息，可以避免第一个节点的特殊计算校验码）
void Addinformation(LinkList& L,int s)
{
	//寻找第s个节点
	int i = 0;
	LNode* q, * p;
	q = L;
	p = q->next;
	while (p)
	{
		if (i==s)
			break;
		else
		{
			p = p->next;
			q = q->next;
			i++;
		}
	}
    //添加新节点
	q = p;
	p = new LNode;
	p->code = i + 1;
	cout << "请输入第"<<i+1<<"个节点的信息:";
	cin >> p->information;
	for (unsigned int size = 0; size < p->information.size(); size++)
	{
		p->checkcode += int(p->information[size]);
		p->checkcode %= 113;
	}
	p->checkcode += q->checkcode;
	p->checkcode += p->code;
	p->checkcode %= 113;
	//将链表连接起来
	p->next = q->next;
	q->next = p;
	//修改该节点后节点信息使链表有效
	p = p->next;
	q = q->next;
	while (p)
	{
		p->code += 1;
		p->checkcode = 0;
		for (unsigned int size = 0; size < p->information.size(); size++)
		{
			p->checkcode += int(p->information[size]);
			p->checkcode %= 113;
		}
		p->checkcode += q->checkcode;
		p->checkcode += p->code;
		p->checkcode %= 113;
		q = p;
		p = p->next;
	}
}

//修改链表中某个特点编号的节点的信息内容，修改后保持链表有效
void ChangeInformation(LinkList& L, int i)
{
	//寻找第i个节点
	int s = 0;
	LNode* q, * p;
	q = L;
	p = q->next;
	while (p)
	{
		if (i == s)
			break;
		else
		{
			p = p->next;
			q = q->next;
			s++;
		}
	}
	cout << "请重新输入第" << i << "个节点的信息:";
	cin >> p->information;
	//修改当前节点的校验码
	//如果修改的是第1个节点
	if (i == 0)
	{
		p->checkcode = 0;
		for (unsigned int size = 0; size < p->information.size(); size++)
		{
			p->checkcode += int(p->information[size]);
			p->checkcode %= 113;
		}
	}
	//如果修改的不是第1个节点
	else
	{
		p->checkcode = 0;
		for (unsigned int size = 0; size < p->information.size(); size++)
		{
			p->checkcode += int(p->information[size]);
			p->checkcode %= 113;
		}
		p->checkcode += q->checkcode;
		p->checkcode += p->code;
		p->checkcode %= 113;
	}
	//修改后方节点信息使链表保持有效
	p = p->next;
	q = q->next;
	while (p)
	{
		p->checkcode = 0;
		for (unsigned int size = 0; size < p->information.size(); size++)
		{
			p->checkcode += int(p->information[size]);
			p->checkcode %= 113;
		}
		p->checkcode += q->checkcode;
		p->checkcode += p->code;
		p->checkcode %= 113;
		q = p;
		p = p->next;
	}
	cout << "第" << i << "个节点信息修改成功!"<<endl;
}

void DestroyList(LinkList& L)
{
	LNode* p;
	while (L->next != NULL)
	{
		p = L->next;
		L->next = p->next;
		delete p;
	}
	delete L;
}