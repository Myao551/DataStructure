#include <iostream>
#include <time.h>
#include<fstream>
#define LISTINITSIZE 50000
#define LISTINCREMENT 10000	
using namespace std;
//待排序数组
int a[50001];

//基数排序需要借助队列
struct LNode
{
	int date;
	LNode* next;
};

typedef LNode* QueuePtr;

struct LinkQueue
{
	QueuePtr head;
	QueuePtr tail;
};

//快速排序栈中结构体
struct node
{
	int low, high;
};

//快速排序需要借助栈
struct SeqStack
{
	int stacksize;
	node* pBase;
	node* pTop;
};

//函数声明
void InitQueue(LinkQueue& Q);
void DestroyQueue(LinkQueue& Q);
void EnQueue(LinkQueue& Q, int e);
void DeQueue(LinkQueue& Q, int& e);
bool QueueEmpty(LinkQueue Q);
void InitStack(SeqStack& S);
void DestoryStack(SeqStack& S);
bool StackEmpty(SeqStack S);
void Push(SeqStack& S, node e);
void Pop(SeqStack& S, node& e);
void CreateRandomTxt();
void CreateOrderTxtp();
void CreateOrderTxtn();
void Inarr(int a[]);
void InsertSort(int a[]);//1
void ShellSort(int a[]);//2
void BubbleSort(int a[]);//3
int Partition(int a[], int low, int high);
void QSort(int a[], int low, int high);
void QuickSort(int a[], int low, int high);//4
void SelectSort(int a[]);//5
void HeapAdjust(int a[], int low, int high);
void HeapSort(int a[]);//6
void Merge(int a[], int low, int mid, int high);
void MSort(int a[], int len);
void MergeSort(int a[]);//7
void RadixSort(int a[]);//8

int main()
{
	int q,h;
	cout << "八次随机样本排序如下:"<<endl;
	for (int i = 1; i <= 8; i++)
	{
		CreateRandomTxt();
		Inarr(a);
		q = clock();
		InsertSort(a);
		h = clock();
		cout << "第" << i << "次直接插入排序用时:" << h - q << "ms"<<endl;
		Inarr(a);
		q = clock();
		ShellSort(a);
		h = clock();
		cout << "第" << i << "次希尔排序用时:" << h - q << "ms" << endl;
		Inarr(a);
		q = clock();
		BubbleSort(a);
		h = clock();
		cout << "第" << i << "次冒泡排序用时:" << h - q << "ms" << endl;
		Inarr(a);
		q = clock();
		SelectSort(a);
		h = clock();
		cout << "第" << i << "次选择排序用时:" << h - q << "ms" << endl;
		Inarr(a);
		q = clock();
		HeapSort(a);
		h = clock();
		cout << "第" << i << "次堆排序用时:" << h - q << "ms" << endl;
		Inarr(a);
		q = clock();
		QuickSort(a,1,50000);
		h = clock();
		cout << "第" << i << "次快速排序用时:" << h - q << "ms" << endl;
		Inarr(a);
		q = clock();
		MergeSort(a);
		h = clock();
		cout << "第" << i << "次归并排序用时:" << h - q << "ms" << endl;
		Inarr(a);
		q = clock();
		RadixSort(a);
		h = clock();
		cout << "第" << i << "次基数排序用时:" << h - q << "ms" << endl;
	}
	cout << "升序样本排序结果如下:" << endl;
	CreateOrderTxtp();
	Inarr(a);
	q = clock();
	InsertSort(a);
	h = clock();
	cout << "直接插入排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	ShellSort(a);
	h = clock();
	cout << "希尔排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	BubbleSort(a);
	h = clock();
	cout << "冒泡排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	SelectSort(a);
	h = clock();
	cout <<"选择排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	HeapSort(a);
	h = clock();
	cout <<"堆排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	QuickSort(a,1,50000);
	h = clock();
	cout <<"快速排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	MergeSort(a);
	h = clock();
	cout << "归并排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	RadixSort(a);
	h = clock();
	cout << "基数排序用时:" << h - q << "ms" << endl;
	cout << "降序样本排序结果如下:" << endl; 
	CreateOrderTxtn();
	Inarr(a);
	q = clock();
	InsertSort(a);
	h = clock();
	cout << "直接插入排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	ShellSort(a);
	h = clock();
	cout << "希尔排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	BubbleSort(a);
	h = clock();
	cout << "冒泡排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	SelectSort(a);
	h = clock();
	cout << "选择排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	HeapSort(a);
	h = clock();
	cout << "堆排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	QuickSort(a,1,50000);
	h = clock();
	cout << "快速排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	MergeSort(a);
	h = clock();
	cout << "归并排序用时:" << h - q << "ms" << endl;
	Inarr(a);
	q = clock();
	RadixSort(a);
	h = clock();
}

void InitQueue(LinkQueue& Q)
{
	Q.head = Q.tail = new LNode;
	Q.head->next = NULL;
}

void DestroyQueue(LinkQueue& Q)
{
	LNode* p;
	while (Q.head->next != NULL)
	{
		p = Q.head->next;
		Q.head->next = p->next;
		delete p;
	}
	delete Q.head;
}

void EnQueue(LinkQueue& Q, int e)
{
	LNode* s;
	s = new LNode;
	s->date = e;
	s->next = NULL;
	Q.tail->next = s;
	Q.tail = s;
}

void DeQueue(LinkQueue& Q, int& e)
{
	if (Q.head == Q.tail)cout << "错误";
	LNode* p;
	p = Q.head->next;
	e = p->date;
	Q.head->next = p->next;
	if (Q.tail == p)Q.tail = Q.head;
	delete p;
}

bool QueueEmpty(LinkQueue Q)
{
	if (Q.head==Q.tail)
		return 1;
	else return 0;
}

//创建随机样本
void CreateRandomTxt()
{
	srand((int)time(0));
	int i = 1;
	ofstream out;
	out.open("样本.txt");
	if (!out)
	{
		cout << "打开文件失败!";
		exit(1);
	}
	for (i; i < 50001; i++)
	{
		out << rand() % 50000 + 1<<' ';
		if (i%20==0)
		{
			out << endl;
		}
	}
	out.close();
}

//创建升序样本
void CreateOrderTxtp()
{
	int i = 1;
	ofstream out;
	out.open("样本.txt");
	if (!out)
	{
		cout << "打开文件失败!";
		exit(1);
	}
	for (i; i < 50001; i++)
	{
		out << i<<' ';
		if (i % 20 == 0)
		{
			out << endl;
		}
	}
	out.close();
}

//创建降序样本
void CreateOrderTxtn()
{
	int i = 50000;
	ofstream out;
	out.open("样本.txt");
	if (!out)
	{
		cout << "打开文件失败!";
		exit(1);
	}
	for (i; i >0; i--)
	{
		out << i<<' ';
		if (i % 20 == 0)
		{
			out << endl;
		}
	}
	out.close();
}

//将样本输入数组
void Inarr(int a[])
{
	int i = 1;
	ifstream in;
	in.open("样本.txt");
	if (!in)
	{
		cout << "打开样本文件失败!";
		exit(0);
	}
	for (i; i <= 50000; i++)
	{
		in >> a[i];
	}
	in.close();
}

//直接插入排序
void InsertSort(int a[])
{
	for (int i = 2; i <= 50000; i++)
	{
		int t = a[i];
		int j;
		for (j = i; j > 1; j--)
		{
			if (t < a[j - 1])
			{
				a[j] = a[j - 1];
			}
			else break;
		}
		a[j] = t;
	}
}

//希尔排序
void ShellSort(int a[])
{
	for (int d = 5; d > 0; d = d - 2)
	{
		for (int i = d + 1; i <= 50000; i++)
		{
			if (a[i] < a[i - d])
			{
				int j;
				int t = a[i];
				for ( j = i; j > d; j = j - d)
				{
					if (t < a[j - d])
						a[j] = a[j - d];
					else break;
				}
				a[j] = t;
			}
		}
	}
}

//冒泡排序
void BubbleSort(int a[])
{
	int i, j, t;
	for (i = 1; i <= 50000; i++)
	{
		for (j = 1; j <= 50000; j++)
		{
			if (a[j] > a[j + 1])
			{
				t = a[j];
				a[j]=a[j + 1];
				a[j + 1] = t;
			}
		}
	}
}

//快速排序
int Partition(int a[], int low, int high)
{
	int t = a[low];
	while (low < high)
	{
		while (low < high && a[high] >= t)
			high--;
		a[low] = a[high];
		while (low < high && a[high] <= t)
			low++;
		a[high] = a[low];
	}
	a[low] = t;
	return low;
}

void QuickSort(int a[], int low, int high)
{
	SeqStack S;
	InitStack(S);
	node no;
	int i;
	if (low < high)
	{
		i = Partition(a, low, high);
		if (low < i - 1)
		{
			no.low = low;
			no.high = i - 1;
			Push(S, no);
		}
		if (high > i + 1)
		{
			no.low = i + 1;
			no.high = high;
			Push(S, no);
		}
		while (!StackEmpty(S))
		{
			Pop(S, no);
			int l = no.low;
			int h = no.high;
			i = Partition(a, l, h);
			if (l < i - 1)
			{
				no.low = l;
				no.high = i - 1;
				Push(S, no);
			}
			if (h > i + 1)
			{
				no.low = i + 1;
				no.high = h;
				Push(S, no);
			}
		}
	}
}

//选择排序
void SelectSort(int a[])
{
	for (int i = 1; i <= 50000; i++)
	{
		int m = a[i];
		int k = i;
		for (int j = i + 1; j <= 50000; j++)
		{
			if (a[j] < m)
			{
				m = a[j];
				k = j;
			}
		}
		int t = a[i];
		a[i] = a[k];
		a[k] = t;
	}
}

//堆排序
void HeapAdjust(int a[], int low, int high)
{
	int t = a[low];
	int i = low, j = 2 * i;
	while (j <= high)
	{
		if (j + 1 <= high && a[j] < a[j + 1])
			j++;
		if (t >= a[j])
			break;
		a[i] = a[j];
		i = j;
		j = 2 * i;
	}
	a[i] = t;
}

void HeapSort(int a[])
{
	int t;
	for (int i = 50000 / 2; i > 0; i--)
		HeapAdjust(a, i, 50000);
	for (int i = 50000; i > 1; i--)
	{
		t = a[1];
		a[1] = a[i];
		a[i] = t;
		HeapAdjust(a, 1, i - 1);
	}
}

//并归排序
void Merge(int a[], int low, int mid, int high)
{
	int b[50001];
	int i = low;
	int j = mid + 1;
	int k = 0;
	while (i <= mid && j <= high)
	{
		if (a[i] <= a[j])
		{
			b[k] = a[i];
			i++;
			k++;
		}
		else
		{
			b[k] = a[j];
			j++;
			k++;
		}
	}
	while (i <= mid)
	{
		b[k] = a[i];
		i++;
		k++;
	}
	while (j <= high)
	{
		b[k] = a[j];
		j++;
		k++;
	}
	for (k = 0, i = low; i <= high; k++, i++)
	{
		a[i] = b[k];
	}
}

void MSort(int a[], int len)
{
	int i = 1;
	while (i + 2 * len <= 50000)
	{
		Merge(a, i, i + len - 1, i + 2 * len - 1);
		i = i + 2 * len;
	}
	if (i + len <= 50000)
		Merge(a, i, i + len - 1, 50000);
}

void MergeSort(int a[])
{
	for (int len = 1; len <= 50000; len = 2 * len)
	{
		MSort(a, len);
	}
}

//基数排序
void RadixSort(int a[])
{
	LinkQueue Q[10];
	for (int i = 0; i < 10; i++)
	{
		InitQueue(Q[i]);
	}
	int radix = 1;
	for (int k = 1; k <= 5; k++)
	{
		int m;
		radix *= 10;
		for (int i = 1; i <= 50000; i++)
		{
			m = (a[i] % radix) / (radix  / 10);
			EnQueue(Q[m], a[i]);
		}
		for (int i = 1, m = 0; m < 10; m++)
		{
			while (!QueueEmpty(Q[m]))
			{
				DeQueue(Q[m], a[i]);
				i++;
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		DestroyQueue(Q[i]);
	}
}

//栈函数
void InitStack(SeqStack& S)
{
	S.pBase = new node[LISTINITSIZE];
	S.stacksize = LISTINITSIZE;
	S.pTop = S.pBase;
}

void DestoryStack(SeqStack& S)
{
	if (S.pBase != NULL)
	{
		delete[]S.pBase;
		S.pBase = NULL;
	}
	S.pTop = NULL;
	S.stacksize = 0;
}

bool StackEmpty(SeqStack S)
{
	if (S.pBase == S.pTop)
		return 1;
	else return 0;
}

void Push(SeqStack& S,node e)
{
	if (S.pTop - S.pBase == S.stacksize)
	{
		node* newbase;
		newbase = new node[S.stacksize + LISTINCREMENT];
		for (int a = 0; a < S.stacksize; a++)
		{
			newbase[a] = S.pBase[a];
		}
		delete[]S.pBase;
		S.pBase = newbase;
		S.pTop = S.pBase + S.stacksize;
		S.stacksize += LISTINCREMENT;
	}
	*S.pTop = e;
	S.pTop++;
}

void Pop(SeqStack& S, node& e)
{
	if (S.pBase == S.pTop)cout << "错误";
	else
	{
		e = *(S.pTop - 1);
		S.pTop--;
	}
}