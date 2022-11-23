#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//�洢��Ϣ��¼�Ľṹ��
struct Information
{
	string ID;   //���֤
	string Name;  //����
	string FlightNumber;    //�����
	string Date;  //��������
	int Distance;   //���о���
	Information* next;   //����ַ�������ͻ
};

//ɢ�б�
Information record[500];   

//��������
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

//Hash����
int Hash(string ID)
{
	int t;
	//ȡ���֤����λ
	t = int(ID[14]-'0') * 1000 + int(ID[15]-'0') * 100 + int(ID[16]-'0') * 10 + int(ID[17]-'0');
	t = t % 211;
	return t;
}

//��¼ɢ�б���ĳ��λ���Ƿ��Ѿ�������Ϣ
int visited1[500]; //���ŵ�ַ��
int visited2[500];  //����ַ��

//����ɢ�б����ŵ�ַ����
int CreateHashTable1(Information record[500])
{
	cout << "���ŵ�ַ��������ͻ����:"<<endl;
	int k = 1, s = 0;
	int location;
	ifstream in;
	//n,t��ʱ��¼��ǰ��������������֤
	string n,t;
	for (int i = 0; i <500; i++)
	{
		visited1[i] = 0;
	}
	in.open("source.txt");
	if (!in)
	{
		cout << "�� source.txt ʧ��!";
		exit(1);
	}
	while (!in.eof())
	{   
		s = 0;
		in >> n;
		in >> t;
		location = Hash(t);   //����hash����ȷ��λ��
		//���ȷ����λ��Ϊ�գ���ֱ�Ӵ�����Ϣ
		if (visited1[location] == 0)
		{
			record[location].ID = t;
			record[location].Name = n;
			in >> record[location].FlightNumber;
			in >> record[location].Date;
			in >> record[location].Distance;
			visited1[location] = 1;    //������Ϣ�󽫱�־��Ϊ1
		}
		//����ǰλ���Ѿ�������Ϣ
		else
		{
			s++;
			//����̽�ⷨ��λ�ú���ȷ��
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
			visited1[location] = 1;   //��������Ϣ��λ����Ϊ1
		}
		if (s) 
		{
			cout << "��" << k << "�ν����ͻ�ض�λ��" << s << "��" << endl;
			k++;
		}
	}
	in.close();
	return 1;
}

//����ַ������ɢ�б�
void CreateHashTable2(Information record[500])
{
	cout << "����ַ��������ͻ����:" << endl;
	Information* q, * p;
	int k = 1, s = 0;
	int location;
	ifstream in;
	//n,t��ʱ��¼��ǰ��������������֤
	string n, t;
	for (int i = 0; i < 500; i++)
	{
		visited2[i] = 0;
	}
	in.open("source.txt");
	if (!in)
	{
		cout << "�� source.txt ʧ��!";
		exit(1);
	}
	while (!in.eof())
	{
		s = 0;
		in >> n;
		in >> t;
		location = Hash(t);   //����hash����ȷ��λ��
		//���ȷ����λ��Ϊ�գ���ֱ�Ӵ�����Ϣ
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
			cout << "��" << k << "�ν����ͻ�ض�λ��" << s << "��" << endl;
			k++;
		}
	}
}

//����vip�ͻ�
int FindVIP(Information record[500])
{
	int flag[500];  //��һ��������飬������֤ͬһ�����֤����Ϣ��һ��ѭ����ȫ������
	for (int i = 0; i < 500; i++)
	{
		flag[i] = 0;
	}
	for (int i = 0; i < 500; i++)
	{
		int ts = 0;   //��ʱ��¼һ���˵ĳ˻�����
		int ds = 0;    //��ʱ��¼һ���˵ķ��������
		string tempID;   //��ʱ��¼һ���˵����֤��
		//��ɢ�б���Ѱ�Ҵ�������Ϣ����û��ȷ���Ƿ�Ϊvip�Ŀͻ�
		if (visited1[i] == 1&&flag[i]==0)
		{
			tempID = record[i].ID;  //��¼��ǰ���֤
			ts++;  //�ҵ�һ����¼���д�����һ
			ds = ds + record[i].Distance;   //����̼��ϵ�ǰ��¼
			flag[i] = 1;    //��ʾ�Ѿ����ҹ�������¼
			//Ѱ��ʣ���¼��ͬһ���˵ļ�¼
			for (int j = i+1; j < 500; j++)
			{
				if (tempID == record[j].ID)//������֤��ͬ����ô����ͬһ���˵ķ��м�¼
				{
					ts++;     //���д����ټ�һ
					ds = ds + record[j].Distance;   //���о����ۼ�
					flag[j] = 1;   //flag��Ϊ1��ʾ�Ѿ����ҹ�
				}
			}
			//ƽ��ÿ�η�����̳���800km�Ҹö�ʱ���ڳ˻��������ڵ���4ȷ��ΪVIP�ͻ�
			if (ds / ts >= 800&&ts>3)
			{
				cout << record[i].Name << "��VIP�ͻ� " << "�����Ϊ:" << ds << "km �˻�����Ϊ:" << ts<<endl;
			}
		}
	}
	return 1;
}

//���ɢ�б�����Ϣ(���ŵ�ַ����
void print1(Information record[500])
{
	cout << "���ŵ�ַ���ͻ�������¼Ϊ:" << endl;
	for (int i = 0; i < 500; i++)
	{
		if (visited1[i] == 1)
		{
			cout <<"����:" << record[i].Name << ' ' <<"���֤��:" << record[i].ID << ' ' <<"�����:" << record[i].FlightNumber << ' ' <<"����:" << record[i].Date << ' ' <<"���о���:" << record[i].Distance <<"km" << endl;
		}
	}
}

void print2(Information record[500])
{
	Information* p;
	cout << "����ַ���ͻ�������¼Ϊ:" << endl;
	for (int i = 0; i < 500; i++)
	{
		if (visited2[i] == 1)
		{
			p = record[i].next;
			while (p)
			{
				cout << "����:" << p->Name << ' ' << "���֤��:" << p->ID << ' ' << "�����:" << p->FlightNumber << ' ' << "����:" << p->Date << ' ' << "���о���:" << p->Distance << "km" << endl;
				p = p->next;
			}
		}
	}
}