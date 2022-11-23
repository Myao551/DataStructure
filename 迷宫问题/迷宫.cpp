#include<iostream>
#include<fstream>
using namespace std;
#define MAZESIZE 25
#define LISTINITSIZE 128
#define LISTINCREMENT 64

//�洢�Թ�·���ĵ�����ṹ��
struct point
{
	int x, y;
};

struct maze
{
	char map[MAZESIZE][MAZESIZE];
};

//�Թ�������Ҫ����ջ���
struct SeqStack
{
	int stacksize;
	point* pBase;
	point* pTop;
};

//��������
void InitStack(SeqStack& S);
int StackLength(SeqStack S);
void StackTraverse(SeqStack S);
void DestoryStack(SeqStack& S);
void Push(SeqStack& S, point e);
void Pop(SeqStack& S, point& e);
bool StackEmpty(SeqStack S);
int GetTop(SeqStack S, point& e);
int CreateMaze(maze* p, point& en, point& ex);
void Mazes(maze* p, point en, point ex);

int main()
{
	maze a;
	maze* p = &a;
	point en, ex;
	CreateMaze(p, en, ex);
	Mazes(p, en, ex);
}

void InitStack(SeqStack& S)
{
	S.pBase = new point[LISTINITSIZE];
	S.stacksize = LISTINITSIZE;
	S.pTop = S.pBase;
}

int StackLength(SeqStack S)
{
	return S.pTop - S.pBase;
}

void StackTraverse(SeqStack S)
{
	cout << "�Թ�·��Ϊ:"<<endl;
	for (int i = 1; i <= StackLength(S); i++)
	{
		cout <<'(' << S.pBase[i-1].x <<',' << S.pBase[i-1].y<<')'<<' ';
		if (i % 8 == 0)
			cout << endl;
	}
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

void Push(SeqStack& S, point e)
{
	if (S.pTop - S.pBase == S.stacksize)
	{
		point* newbase;
		newbase = new point[S.stacksize + LISTINCREMENT];
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

void Pop(SeqStack& S, point& e)
{
	if (S.pBase == S.pTop)cout << "����";
	else
	{
		e = *(S.pTop - 1);
		S.pTop--;
	}
}

bool StackEmpty(SeqStack S)
{
	if (S.pBase == S.pTop)
		return 1;
	else return 0;
}

int GetTop(SeqStack S, point& e)
{
	if (S.pBase == S.pTop)
		return 0;
	e = *(S.pTop - 1);
	return 1;
}

//�����Թ�
int CreateMaze(maze *p,point &en,point &ex )
{
	//�洢�Թ����Ϳ�
	int x, y;
	ifstream in;
	in.open("�Թ�·.txt");
	if (!in)
	{
		cout << "�� �Թ�·.txt ʧ��" << endl;
		exit(1);
	}
	//�����Թ�
	in >> x >> y;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			in >>p->map[i][j];
		}
	}
	in.close();
	//����Թ�
	cout << "ԭ�Թ�Ϊ:" << endl;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			cout << p->map[i][j]<<' ';
			if (j == y - 1)
				cout << endl;
		}
	}
	cout << "�������Թ����������:";
	cin >> en.x >> en.y;
	en.x--; en.y--;
	cout << "�������Թ��ĳ�������:";
	cin >> ex.x >> ex.y;
	ex.x--; ex.y--;
}

//���Թ�����
void Mazes(maze *p,point en,point ex)
{
	//����ջ
	SeqStack S;
	InitStack(S);
	Push(S, en);
	//move,x,y��ʾ��ǰλ��
	point move = en,q;
	int x, y;
	x = en.x;
	y = en.y;
	bool flag = 0;
	int i;
	//m[][]��ʾ���ĸ������ƶ�
	int m[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
	//��ջ��Ϊ�գ��ҵ�ǰ��ĺ������겻ͬʱ�����յ�������꣬˵����δ�����յ�
	while (!StackEmpty(S) && ((move.x != ex.x) ||(move.y != ex.y)))
	{
		GetTop(S, move);  //����ǰ���ջ
		flag = 0;
		i = 0;
		//��ǰ��δ��������ʼ�����ĸ�����
		while (flag == 0&&i<4)
		{
			x = move.x + m[i][0];
			y = move.y + m[i][1];
			//��ĳ������Ϊͨ·������÷����ƶ�
			if (p->map[x][y] == '0')
			{
				move.x = x;
				move.y = y;
				Push(S, move);
				p->map[x][y] = '*';   //�ҽ��õ���Ϊ*��ʾͨ·
				flag = 1;      //��־��Ϊ1˵���Ѿ��߹��õ�
			}
			i++;  //i++��ʾ��һ����������
		}
		//����ĸ�����ȫ��������û������
		if (i == 4 && flag == 0)
		{
			p->map[move.x][move.y] = '1';  //��ô����ǰ����Ϊǽ��...���˴���Ϊ�˱��ڴ���
			Pop(S, q);     //���õ��ջ�ص���һ����
		}
	}
	p->map[en.x][en.y]='*';   //���յ���Ϊͨ·��һ����
	//�����ջ�ձ�ʾ�ص�����㣬�Ӷ�û��ͨ·
	if (StackEmpty(S))   
	{
		cout << "û��·��";
	}
	//����ͨ·���
	else
	{
		cout << "����Թ�·�����£���*��ʾ·����"<<endl;
		for (int i = 0; i < MAZESIZE; i++)
		{
			for (int j = 0; j < MAZESIZE; j++)
			{
				cout << p->map[i][j]<<' ';
				if (j == MAZESIZE - 1)
					cout << endl;
			}
		}
		StackTraverse(S);
	}
	
}