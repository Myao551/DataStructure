#include<iostream>
#include<fstream>
using namespace std;

struct HTNode
{
	 int weight;
	 int parent, lchild, rchild;
};

typedef HTNode* HufTree;
HufTree T;
int N;

struct info
{
	int num;
	string code;
};

info c[128];   //��¼�ַ����ִ��������飬c[i]�洢ascii��Ϊi���ַ�

void ReadTxt(info c[])
{
	char ch;
	for (int i = 0; i < 128; i++)   //��ʼ����¼�ַ����ִ���������
	{
		c[i].num = 0;
	}
	ifstream read;
	read.open("�շ���������.txt");
	read.unsetf(ios::skipws);    //ȡ���Զ������ո��뻻�з���
	if (!read)
	{
		cout << "���ļ�ʧ��!" << endl;
		exit(1);
	}
	while (!read.eof())
	{
		read>> ch;
		c[int(ch)].num++;
	}
	read.close();
}

void HufCode(info c[], HufTree HT, char** HC)
{
	int i ,j;
	int n = 0;
	for (int i = 0; i < 128; i++)    //�����ȡ���˶��ٸ���ͬ������ַ�
	{
		if (c[i].num)
		{
			n++;
		}
	}
	N = n;
	HT = new HTNode[2 * n];   //��̬����2n����Ԫ��0�ŵ�Ԫ��ʹ��
	T = HT;
	i = 1;
	j = 0;
	while(i<=n)     //�������Ķ�Ӧ�ַ��ĸ������������Ȩֵ��
	{
		if (c[j].num>0)
		{
			HT[i].weight = c[j].num;
			i++;
		}
		j++;
	}
	for (int i = 1; i < 2 * n; i++)   //��ʼ��ÿ����Ԫ
	{
		HT[i].lchild = 0; HT[i].rchild = 0; HT[i].parent = 0;
	}
	for (int i = n + 1; i < 2 * n; i++)  //����
	{
		int s1, s2;
		//�����ĸ�forѭ������ѡ��Ȩֵ��С���С�ĵ�Ԫ
		for (int j = 1; j <= i - 1; j++)   
		{
			if (HT[j].parent == 0)
			{
				s1 = j;
				break;
			}
		}
		for (int j = 1; j <= i - 1; j++)
		{
			if (HT[j].weight < HT[s1].weight && HT[j].parent == 0)
				s1 = j;
		}
		for (int j = 1; j <= i - 1; j++)
		{
			if (HT[j].parent == 0 && j != s1)
			{
				s2 = j;
				break;
			}
		}
		for (int j = 1; j <= i - 1; j++)
		{
			if (HT[j].weight < HT[s2].weight && HT[j].parent == 0 && j != s1)
				s2 = j;
		}
		//ѡ����С���С�󽨳�һ���½ڵ㣬���ýڵ���Ϣ��������
		HT[s1].parent = HT[s2].parent = i;  
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
    //ͨ���������
	HC = new char* [n + 1];
	char* t = new char[n];
	t[n - 1] = '\0';
	for (int i = 1; i <= n; i++)
	{
		int start = n - 1;  //���ñ��뿪ʼλ��Ϊ�����Ϊ�ɻ��������ı������������ģ��Ӻ���ǰ���룬����Ȼ��������
		int c = i;
		int f = HT[c].parent;
		while (f)
		{
			start--;
			if (HT[f].lchild == c)t[start] = '0';
			else t[start] = '1';
			c = f;
			f = HT[f].parent;
		}
		HC[i] = new char[n - start];
		strcpy(HC[i], &t[start]);
	}
	delete[]t;
	ofstream write;
	write.open("Huffman.txt");
	j = 0;
	for (int i = 1; i <= n; i++)   //���
	{
		while (c[j].num == 0)
			j++;
		cout <<"�ַ�: " << char(j) << "�ĳ��ִ���Ϊ:" << c[j].num << "   ����������Ϊ:" << HC[i] << endl;
		write << "�ַ�: " << char(j) << "�ĳ��ִ���Ϊ:" << c[j].num << "   ����������Ϊ:" << HC[i] << endl;
		c[j].code = HC[i];
		j++;
	}
	write.close();
	delete[]HC;
}

//��������dat�ļ��洢
void Save_dat(info c[])
{
	char ch;
	ifstream read("�շ���������.txt");
	read.unsetf(ios::skipws);
	if (!read)
	{
		cout << "�򿪺շ����������ļ�ʧ��!" << endl;
		exit(1);
	}
	ofstream write("code.dat", ios::out | ios::binary);
	while (!read.eof())
	{
		read >> ch;
		write << c[int(ch)].code;
	}
	read.close();
	write.close();
}

//���벢���
void Decode(HufTree HT)
{
	cout << "����������:" << endl;
	ifstream read("code.dat");
	if (!read)
	{
		cout << "��dat�ļ�ʧ��!";
		exit(1);
	}
	char ch;
	int i = 1, j;
	int t = 2 * N - 1;
	while (!read.eof())
	{
		i = 0;
		t = 2 * N - 1;   //ÿ�ζ���2n-1��ʼ 
		while (t>N)
		{
			read >> ch;
			if (ch == '0')    //����0��������
				t = T[t].lchild;
			if (ch == '1')    //����1�����Һ���
				t = T[t].rchild;
		}
		j = 0;
		while (1)   //Ѱ���Ϸŵ�Ԫ���Ӧ���ַ�
		{
			if (c[j].num>0)
				i++;
			if (i == t)
				break;
			j++;
		} 
		cout << char(j);    //������ַ���j����ַ���acsii���Ӧ
	}
}

int main()
{
	HufTree HT = NULL;
	char** HC = NULL;
	ReadTxt(c);
	HufCode(c, HT, HC);
	Save_dat(c);
	Decode(T);
	delete T;
}