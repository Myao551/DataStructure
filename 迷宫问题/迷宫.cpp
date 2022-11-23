#include<iostream>
#include<fstream>
using namespace std;
#define MAZESIZE 25
#define LISTINITSIZE 128
#define LISTINCREMENT 64

//存储迷宫路径的点坐标结构体
struct point
{
	int x, y;
};

struct maze
{
	char map[MAZESIZE][MAZESIZE];
};

//迷宫问题需要借助栈解决
struct SeqStack
{
	int stacksize;
	point* pBase;
	point* pTop;
};

//函数声明
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
	cout << "迷宫路径为:"<<endl;
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
	if (S.pBase == S.pTop)cout << "错误";
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

//创建迷宫
int CreateMaze(maze *p,point &en,point &ex )
{
	//存储迷宫长和宽
	int x, y;
	ifstream in;
	in.open("迷宫路.txt");
	if (!in)
	{
		cout << "打开 迷宫路.txt 失败" << endl;
		exit(1);
	}
	//输入迷宫
	in >> x >> y;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			in >>p->map[i][j];
		}
	}
	in.close();
	//输出迷宫
	cout << "原迷宫为:" << endl;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			cout << p->map[i][j]<<' ';
			if (j == y - 1)
				cout << endl;
		}
	}
	cout << "请输入迷宫的入口坐标:";
	cin >> en.x >> en.y;
	en.x--; en.y--;
	cout << "请输入迷宫的出口坐标:";
	cin >> ex.x >> ex.y;
	ex.x--; ex.y--;
}

//走迷宫函数
void Mazes(maze *p,point en,point ex)
{
	//辅助栈
	SeqStack S;
	InitStack(S);
	Push(S, en);
	//move,x,y表示当前位置
	point move = en,q;
	int x, y;
	x = en.x;
	y = en.y;
	bool flag = 0;
	int i;
	//m[][]表示向四个方向移动
	int m[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
	//若栈不为空，且当前点的横纵坐标不同时等于终点横纵坐标，说明还未到达终点
	while (!StackEmpty(S) && ((move.x != ex.x) ||(move.y != ex.y)))
	{
		GetTop(S, move);  //将当前点进栈
		flag = 0;
		i = 0;
		//当前点未到过，开始尝试四个方向
		while (flag == 0&&i<4)
		{
			x = move.x + m[i][0];
			y = move.y + m[i][1];
			//若某个方向为通路，则向该方向移动
			if (p->map[x][y] == '0')
			{
				move.x = x;
				move.y = y;
				Push(S, move);
				p->map[x][y] = '*';   //且将该点置为*表示通路
				flag = 1;      //标志置为1说明已经走过该点
			}
			i++;  //i++表示换一个方向行走
		}
		//如果四个方向全部尝试且没有行走
		if (i == 4 && flag == 0)
		{
			p->map[move.x][move.y] = '1';  //那么将当前点置为墙壁...（此处是为了便于处理）
			Pop(S, q);     //将该点出栈回到上一个点
		}
	}
	p->map[en.x][en.y]='*';   //将终点置为通路的一部分
	//若最后栈空表示回到了起点，从而没有通路
	if (StackEmpty(S))   
	{
		cout << "没有路径";
	}
	//否则将通路输出
	else
	{
		cout << "解后迷宫路径如下（用*表示路径）"<<endl;
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