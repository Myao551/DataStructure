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

info c[128];   //记录字符出现次数的数组，c[i]存储ascii码为i的字符

void ReadTxt(info c[])
{
	char ch;
	for (int i = 0; i < 128; i++)   //初始化记录字符出现次数的数组
	{
		c[i].num = 0;
	}
	ifstream read;
	read.open("赫夫曼树材料.txt");
	read.unsetf(ios::skipws);    //取消自动跳过空格与换行符号
	if (!read)
	{
		cout << "打开文件失败!" << endl;
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
	for (int i = 0; i < 128; i++)    //计算读取到了多少个不同种类的字符
	{
		if (c[i].num)
		{
			n++;
		}
	}
	N = n;
	HT = new HTNode[2 * n];   //动态申请2n个单元，0号单元不使用
	T = HT;
	i = 1;
	j = 0;
	while(i<=n)     //将读到的对应字符的个数赋到数组的权值中
	{
		if (c[j].num>0)
		{
			HT[i].weight = c[j].num;
			i++;
		}
		j++;
	}
	for (int i = 1; i < 2 * n; i++)   //初始化每个单元
	{
		HT[i].lchild = 0; HT[i].rchild = 0; HT[i].parent = 0;
	}
	for (int i = n + 1; i < 2 * n; i++)  //建树
	{
		int s1, s2;
		//下面四个for循环用来选出权值最小与次小的单元
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
		//选出最小与次小后建成一个新节点，将该节点信息存入数组
		HT[s1].parent = HT[s2].parent = i;  
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
    //通过数组编码
	HC = new char* [n + 1];
	char* t = new char[n];
	t[n - 1] = '\0';
	for (int i = 1; i <= n; i++)
	{
		int start = n - 1;  //设置编码开始位置为最后，因为由霍夫曼树的编码过程是逆向的，从后往前读入，便自然成了正向
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
	for (int i = 1; i <= n; i++)   //输出
	{
		while (c[j].num == 0)
			j++;
		cout <<"字符: " << char(j) << "的出现次数为:" << c[j].num << "   霍夫曼编码为:" << HC[i] << endl;
		write << "字符: " << char(j) << "的出现次数为:" << c[j].num << "   霍夫曼编码为:" << HC[i] << endl;
		c[j].code = HC[i];
		j++;
	}
	write.close();
	delete[]HC;
}

//将编码以dat文件存储
void Save_dat(info c[])
{
	char ch;
	ifstream read("赫夫曼树材料.txt");
	read.unsetf(ios::skipws);
	if (!read)
	{
		cout << "打开赫夫曼树材料文件失败!" << endl;
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

//解码并输出
void Decode(HufTree HT)
{
	cout << "解码结果如下:" << endl;
	ifstream read("code.dat");
	if (!read)
	{
		cout << "打开dat文件失败!";
		exit(1);
	}
	char ch;
	int i = 1, j;
	int t = 2 * N - 1;
	while (!read.eof())
	{
		i = 0;
		t = 2 * N - 1;   //每次都从2n-1开始 
		while (t>N)
		{
			read >> ch;
			if (ch == '0')    //遇到0，找左孩子
				t = T[t].lchild;
			if (ch == '1')    //遇到1，找右孩子
				t = T[t].rchild;
		}
		j = 0;
		while (1)   //寻找上放单元格对应的字符
		{
			if (c[j].num>0)
				i++;
			if (i == t)
				break;
			j++;
		} 
		cout << char(j);    //输出该字符，j与该字符的acsii码对应
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