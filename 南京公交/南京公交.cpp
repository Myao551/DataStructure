#include<iostream>
#include<string>
#include<fstream>
#define MAX_V 10000
using namespace std;
int visited[MAX_V] = { 0 };

struct Bus
{
	int No;   //公交车编号
	int PassNum;   //经过站点的数量
	int route[100] = {0}; //公交车的路线
};

struct Station
{
	string name;  //该站点的名称
	int bus[30];  //经过该站点公交车编号
	int PassNum;  //经过该站点公交车的数量
};

struct Map
{
	Bus bus[1000];    //南京的公交车信息（共990路车）
	int BusNum;      //公交车数量
	Station station[10000];    //南京公交车站点信息
	int StationNum;     //站点数量
};

//存入队列的辅助元素
struct element
{
	int bus;    //该点的公交车编号
	int before;   //到达该站点前一站点的编号
	int cur;     //该站点的编号
};

//队列节点结构体
struct QNode
{
	element date;
	QNode* next;
};

typedef QNode* QueuePtr;

struct LinkQueue
{
	QueuePtr front;
	QueuePtr rear;
};

//函数声明
void InitMap(Map& M);
void CreateMap(Map& M);
int locatebus(Map M, int no); 
int locatestation(Map M, string name);
int GetBusStation(Map& M, string t);
void out(Map M);
int InitQueue(LinkQueue& Q);
int EnQueue(LinkQueue& Q, element e);
element DeQueue(LinkQueue& Q);
int MinTransfer(Map M, string startpoint, string destination);
int MinStation(Map M, string startpoint, string destination);
void DestroyQueue(LinkQueue& Q);

int main()
{
	Map M;
	InitMap(M);
	CreateMap(M);
	string qd, zd;
	while (1)
	{   
		cout << "若要退出系统请输入‘退出’!"<<endl;
		cout << "请输入起点名:";
		cin >> qd;
		if (qd == "退出")
			break;
		if (!locatestation(M, qd))
		{
			cout << "请检查起点输入是否有误!";
			continue;
		}
		cout << "请输入终点名;";
		cin >> zd;
		if (zd == "退出")
			break;
		if (!locatestation(M, qd))
		{
			cout << "请检查终点输入是否有误!";
			continue;
		}
		cout << "最少转车路线:" << endl;
		MinTransfer(M, qd, zd);
		cout << "最少站点路线:" << endl;
		MinStation(M, qd, zd);
	}
	return 0;
}

void InitMap(Map& M)
{
	M.BusNum = 0;
	M.StationNum = 0;
	for (int i = 0; i < 1000; i++)
	{
		M.bus[i].PassNum = 1;
	}
	for (int i = 0; i < 10000; i++)
	{
		M.station[i].PassNum = 0;
	}
}

//寻找xxx路公交车的编号
int locatebus(Map M, int no)
{
	for (int i = 1; i <= M.BusNum; i++)
	{
		if (M.bus[i].No == no)
		{
			return i;
		}
	}
	return 0;
}

//寻找name站的编号
int locatestation(Map M, string name)
{
	for (int i = 1; i <= M.StationNum; i++)
	{
		if (M.station[i].name == name)
		{
			return i;
		}
	}
	return 0;
}

//寻找t站的编号，若未找到t站，则将t站编号存入Map和当前公交车的路线站点中，返回该站编号
int GetBusStation(Map& M, string t)
{
	int i = locatestation(M, t);
	if (!i)     //没有找到该站，将该站信息存入Map
	{
		M.station[++M.StationNum].name = t;
		i = M.StationNum;
	}
	M.bus[M.BusNum].route[M.bus[M.BusNum].PassNum] = i;   //将该站加入当前公交车路线中
	M.station[i].bus[++M.station[i].PassNum] = M.bus[M.BusNum].No;  //将该公交车加入当前站经过的公交车中
	t = "";
	return i;
}

//读取文件内容
void CreateMap(Map& M)
{
	int k = 0;
	string temp;  //临时存储一个站名的变量
	char ch='0';
	ifstream in;
	in.unsetf(ios::skipws);
	in.open("南京公交线路.txt");
	if (!in)
	{
		cout << "打开文件 南京公交线路.txt 失败";
		exit(1);
	}
	for (int i = 0; i < 1000; i++)
	{
		M.bus[i].PassNum = 1;   //将所有公交车经过站点数置为1
	}
	in >> M.bus[++M.BusNum].No; //输入编号为1的公交车路数
	in >> ch;   //读取两次跳过汉字“路”
	in >> ch;
	while (1)
	{
		in >> ch;
		if (ch == ',')     //读取的下一个字符为逗号说明前面一个站名已经读取完成
		{
			GetBusStation(M, temp);
			M.bus[M.BusNum].PassNum++;
			temp = "";
			continue;
		}
		if (ch=='\n')   //读取到换行时，说明前面一个站名已经读取完毕
		{
			GetBusStation(M, temp);
			temp = "";
			if (in.eof())
			{
				break;
			}
			in >> M.bus[++M.BusNum].No;   //换行后读取对应编号公交车路数
			in >> ch;  //读取两次跳过汉字“路”
			in >> ch;
			continue;
		}
		if (in.eof())   //到达文件尾部，说明读取上一个站名完成
		{
			GetBusStation(M, temp);
			break;
		}
		if (ch == ' ')
		{
			continue;
		}
		temp += ch;  //将读取的每个字累加至一个站名
	}
	in.close();
}

//输出文件内容
void out(Map M)
{
	for (int i = 1; i <= M.BusNum; i++)
	{
		cout << M.bus[i].No << "路\t";
		for (int j = 1; j <= M.bus[i].PassNum; j++)
		{
			cout << M.station[M.bus[i].route[j]].name<<' ';
		}
		cout << endl;
	}
}

//初始化队列
int InitQueue(LinkQueue& Q)
{
	Q.front = new QNode;
	if (!Q.front)
	{
		exit(1);
	}
	Q.front->next = NULL;
	Q.rear = Q.front;
	return 0;
}

//进队
int EnQueue(LinkQueue& Q, element e)
{
	QueuePtr p = new QNode;
	if (!p)
	{
		exit(1);
	}
	p->date = e;
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return 0;
}

//出队
element DeQueue(LinkQueue& Q)
{
	element e;
	if (Q.front == Q.rear)
	{
		cout << "队列已空!";
		exit(0);
	}
	QueuePtr p = Q.front->next;
	e = p->date;
	Q.front->next = p->next;
	if (Q.rear == p)
	{
		Q.rear = Q.front;
	}
	delete p;
	return e;
}

//销毁队伍
void DestroyQueue(LinkQueue& Q)
{
	QNode* p;
	while (Q.front->next != NULL)
	{
		p = Q.front->next;
		Q.front->next = p->next;
		delete p;
	}
	delete Q.front;
}

//寻找换乘最少路线
int MinTransfer(Map M, string startpoint, string destination)
{
	int v1 = locatestation(M, startpoint);  //起点对应编号
	int v2 = locatestation(M, destination);   //终点对应编号
	int t;
	t = v1;    //将起点终点编号互换，便于在队列中溯回寻路
	v1 = v2;
	v2 = t;
	for (int i = 1; i < MAX_V; i++)    //初始化标记数组
	{
		visited[i] = 0;
	}
	element e[8000];
	for (int i = 1; i < 8000; i++)
	{
		e[i].cur = i;         //将所有元素当前站点设为自身
		e[i].bus = 0;         //初始化经过的公交
	}
	element e0;    //标记元素，检测到e0出队表示广度遍历的一层寻路完成，跳出当层循环
	int count = 1;    //记录经过站点数量，减2表示转车次数
	LinkQueue Q;
	InitQueue(Q);
	EnQueue(Q, e[v1]);  //终点进队(为第一层）
	visited[v1] = 1;    //终点设为已经经过
	while (1)
	{
		int k = 1;
		count++;
		e0.cur = -1;
		EnQueue(Q, e0);
		while (Q.front != Q.rear)
		{
			v1 = DeQueue(Q).cur;  //将队列中下一个点作为终点 
			if (v1 == -1)
			{
				break;
			}
			for (int i = 1; i <= M.station[v1].PassNum; i++) //尝试经过终点的不同公交
			{
				int bus1 = locatebus(M, M.station[v1].bus[i]);  //设置bus1为经过终点的第i辆公交的编号
				for (int j = 1; j <= M.bus[bus1].PassNum; j++) //尝试当前公交是否能到达起点
				{
					if (M.bus[bus1].route[j] == v2)        //寻找到经过若干次转车到达起点的公交
					{
						int bus2;
						bus2 = locatebus(M, e[v1].bus);   //选择bus2为队列中经过终点的那辆公交车
						cout << "最少转车" << count - 2 << "次" << endl;
						cout << M.station[v2].name << "(乘坐" << M.station[v1].bus[i] << "路公交车)-->";
						count--;
						while (count--)
						{
							if (count == 0)
							{
								cout << M.station[v1].name << "(终点)" << endl;
								DestroyQueue(Q);
								return 1;
							}
							else
							{
								cout << M.station[v1].name << "(换乘" << M.bus[bus2].No << "路公交车）-->";
								v1 = e[v1].before;   //将v1置为从终点寻路到起点到达v1的前一辆公交车，也就是正向的下一辆公交车
								bus2 = locatebus(M, e[v1].bus);  //将bus2设为下一站点乘坐的公交车以便输出
							}
						}
					}
					if (!visited[M.bus[bus1].route[j]])   //如果该站点未经过过
					{
						e[M.bus[bus1].route[j]].bus = M.bus[bus1].No;  //经过该点的公交车设为bus1
						e[M.bus[bus1].route[j]].before = v1;          //该点的上一个点为v1（也就是正向的下一个）
						EnQueue(Q, e[M.bus[bus1].route[j]]);   //将所有与终点相邻的点入队，作为下一个终点进行广度遍历，直到找到起点	
						visited[M.bus[bus1].route[j]] = 1;     //将该站点置为已经经过
					}
				}

			}
		}
	}
}

//寻找经过站点最少路线
int MinStation(Map M, string startpoint, string destination)
{
	int v1 = locatestation(M, startpoint);
	int v2 = locatestation(M, destination);
	int t;
	t = v1;      //与上一个函数相同，将起点终点编号互换，便于在队列中溯回寻路,方便输出
	v1 = v2;
	v2 = t;
	for (int i = 1; i < MAX_V; i++)
	{
		visited[i] = 0;
	}
	element e[8000];
	for (int i = 1; i < 8000; i++)
	{
		e[i].cur = i;         //将所有元素当前站点设为自身
		e[i].bus = 0;         //初始化经过的公交
	}
	element e0;    //标记元素，检测到e0出队表示广度遍历的一层寻路完成，跳出当层循环
	int count = 1;    //记录经过的站点数量
	LinkQueue Q;
	InitQueue(Q);
	EnQueue(Q, e[v1]);  //终点进队(为第一层）
	visited[v1] = 1;    //终点设为已经经过
	while (1)
	{
		int k = 1;
		count++;
		e0.cur = -1;
		EnQueue(Q, e0);
		while (Q.front != Q.rear)
		{
			v1 = DeQueue(Q).cur;    //将队列中下一个点作为终点
			if (v1 == -1)           //广度遍历层的标志
			{
				break;
			}
			for (int i = 1; i <= M.station[v1].PassNum; i++)    //在所有经过终点的公交中筛选
			{
				int bus1 = locatebus(M, M.station[v1].bus[i]);   //bus1为经过终点的某一辆公交
				for (int j = 1; j <= M.bus[bus1].PassNum; j++)   //在bus1路线中所有站点进行筛选
				{
					if (M.bus[bus1].route[j] == v1)       //找到bus1路线中终点的位置
					{
						if (M.bus[bus1].route[j - 1] == v2 || M.bus[bus1].route[j + 1] == v2)  //若与终点相邻的一站便是起点
						{
							int bus2 = e[v1].bus;   //bus2记录当前到达终点的车辆,便于输出
							cout << "最少经过" << count << "个站点" << endl;
							cout << M.station[v2].name << "(乘坐" << M.bus[bus1].No << "路公交车)-->";
							count--;
							while (count--)
							{
								if (count == 0)
								{
									cout << M.station[v1].name << "(终点)" << endl;
									DestroyQueue(Q);
									return 1;
								}
								else
								{
									cout << M.station[v1].name << "(乘坐" << bus2 << "路公交车）-->";
								}
								v1 = e[v1].before;  //将v1置为从终点寻路到起点到达v1的前一站点，也就是正向的下一站点
								bus2 = e[v1].bus;  //bus2重新记录到下一个终点的公交
							}
							
						}
						//将与当前终点相邻的车站全部入队
						if (!visited[M.bus[bus1].route[j - 1]] && j - 1 > 0)
						{
							e[M.bus[bus1].route[j - 1]].bus = M.bus[bus1].No;
							e[M.bus[bus1].route[j - 1]].before = v1;
							EnQueue(Q, e[M.bus[bus1].route[j - 1]]);
							visited[M.bus[bus1].route[j - 1]] = 1;
						}
						if (!visited[M.bus[bus1].route[j + 1]] && j + 1 <= M.bus[bus1].PassNum)
						{
							e[M.bus[bus1].route[j + 1]].bus = M.bus[bus1].No;
							e[M.bus[bus1].route[j + 1]].before = v1;
							EnQueue(Q, e[M.bus[bus1].route[j + 1]]);
							visited[M.bus[bus1].route[j + 1]] = 1;
						}
						break;
					}
				}
			}
		}
	}
}