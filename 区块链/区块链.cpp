#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

//�������ṹ��
struct LNode
{
	//���
	int code=0;
	//�ַ�����Ϣ
	string information="\0";
	//У����
	int checkcode=0;
	LNode* next;
};

typedef LNode* LinkList;

//��������
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
	cout << "��������ϢΪ:";
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

//����������
void CreatList(LinkList& L)
{
	int i = 0;
	ifstream in;
	in.open("data.txt");
	if (!in)
	{
		cout << "��data.txt�ļ�ʧ��:" << endl;
		exit(1);
	}
	LNode* q, * p;
	q = L=new LNode;
	//flag�����ж��Ƿ�Ϊ��һ���ڵ�
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
	cout << "�������������!"<<endl;
}

//�ж��������Ƿ���Ч
void IsCorrect(LinkList L)
{
	LNode* p ,*q;
	q = L;
	p = q->next;
	int i = 0;
	//�������ÿһ���ڵ�
	while (p)
	{
		//checkcode��ʾ����õ�����ȷУ����
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
			cout << "�׸���Ч�ڵ���Ϊ" << i<<endl;
			return;
		}
		i++;
		q = p;
		p = p->next;
	}
	//ѭ��������ʾ�����в�������Ч�ڵ�
	cout << "��������Ч"<<endl;
	return;
}

//�������������Ϣ���ڵ�s���ڵ�������Ϣ�����Ա����һ���ڵ���������У���룩
void Addinformation(LinkList& L,int s)
{
	//Ѱ�ҵ�s���ڵ�
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
    //����½ڵ�
	q = p;
	p = new LNode;
	p->code = i + 1;
	cout << "�������"<<i+1<<"���ڵ����Ϣ:";
	cin >> p->information;
	for (unsigned int size = 0; size < p->information.size(); size++)
	{
		p->checkcode += int(p->information[size]);
		p->checkcode %= 113;
	}
	p->checkcode += q->checkcode;
	p->checkcode += p->code;
	p->checkcode %= 113;
	//��������������
	p->next = q->next;
	q->next = p;
	//�޸ĸýڵ��ڵ���Ϣʹ������Ч
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

//�޸�������ĳ���ص��ŵĽڵ����Ϣ���ݣ��޸ĺ󱣳�������Ч
void ChangeInformation(LinkList& L, int i)
{
	//Ѱ�ҵ�i���ڵ�
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
	cout << "�����������" << i << "���ڵ����Ϣ:";
	cin >> p->information;
	//�޸ĵ�ǰ�ڵ��У����
	//����޸ĵ��ǵ�1���ڵ�
	if (i == 0)
	{
		p->checkcode = 0;
		for (unsigned int size = 0; size < p->information.size(); size++)
		{
			p->checkcode += int(p->information[size]);
			p->checkcode %= 113;
		}
	}
	//����޸ĵĲ��ǵ�1���ڵ�
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
	//�޸ĺ󷽽ڵ���Ϣʹ��������Ч
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
	cout << "��" << i << "���ڵ���Ϣ�޸ĳɹ�!"<<endl;
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