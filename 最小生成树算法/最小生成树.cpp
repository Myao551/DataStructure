#include<iostream>
#include<fstream>
#include <iomanip>
using namespace std;
#define MAX_V  20
int visited[MAX_V + 1];

//存放边的结构体
struct Edge
{
	int v1, v2;  //边关联的两个顶点
	float w;   //边的权重
};

Edge e[MAX_V * (MAX_V - 1) / 2];

struct VertexType
{
	int code;                                      //顶点编号
};

struct MGraph
{
	float arcs[MAX_V][MAX_V];                          //邻接矩阵
	int vexnum,arcnum=0;                                 //顶点数,边数
	VertexType vexs[MAX_V];                          //顶点名称数组
};

//创建图
void CreateGraph(MGraph& G)                          
{
	ifstream in;
	in.open("图.txt");
	if (!in)
	{
		cout << "打开文件失败!";
		exit(1);
	}
	for (int i = 0; i <= G.vexnum; i++)
		visited[i] = 0;
	int i = 0, j = 0,k=1;
    in >> G.vexnum;
	for (i = 1; i <= G.vexnum; i++)                 //输入图中顶点名
	{
		in >> G.vexs[i].code;
	}
	for (i = 0; i < G.vexnum; i++)              //初始化矩阵
	{
		for (j = 0; j < G.vexnum; j++)
		{
			G.arcs[i][j] = 0;
		}
	}                    
	while (!in.eof())                //输入领接矩阵
	{
		in >> i;
		in >> j;
		in>>G.arcs[i-1][j-1];
		 G.arcs[j-1][i-1]=G.arcs[i-1][j-1];
		 G.arcnum++;
		 e[k].v1 = i;
		 e[k].v2 = j;
		 e[k].w = G.arcs[i - 1][j - 1];
		 k++;
	}                                       
	cout << "邻接矩阵为:" << endl;
	for (i = 0; i < G.vexnum; i++)      //输出矩阵
	{
		for (j = 0; j < G.vexnum; j++)
			cout << setw(4) << G.arcs[i][j] << " ";
		cout << endl;
	}                                             
}

//Prim算法求最小生成树
MGraph Prim(MGraph G)              
{
	MGraph T;                              //最小生成树
	T.vexnum = G.vexnum;                   //生成树顶点数等于原来图中的定点数
	visited[0] = 0;
	float s = 0;
	int vexcode[MAX_V + 1];                //记录对应编号顶点最近顶点的数组
	float lowcost[MAX_V + 1];               //记录对应vexcode中顶点离对应编号顶点的最小距离
	int i, j, k;
	for (i = 0; i < G.vexnum; i++)         //初始化树
		for (j = 0; j < G.vexnum; j++)
			T.arcs[i][j] = 0;
	for (j = 1; j <= G.vexnum; j++)         //初始化树中的顶点
	{
		T.vexs[j].code = G.vexs[j].code;
	}
	for (j = 1; j <= G.vexnum; j++)       //初始化Prim算法的三个辅助数组
	{
		lowcost[j] = G.arcs[0][j - 1];
		visited[j] = 0;
		vexcode[j] =1;
	}
	visited[1] = 1;                     //默认以1号顶点为起点生成最小生成树，将其置为已经经过
	for (i = 2; i <= G.vexnum; i++)     //在lowcost中寻找最小值
	{
		float min = 999999;                  //先将最小值赋为大值
		for (j = 1; j <= G.vexnum; j++)        //在lowcost中寻找没有经过且最小的值
		{
			if (visited[j] != 1 && lowcost[j] < min)     //没有经过且最小
			{
				min = lowcost[j];             //将最小值赋值给对应编号的lowcost数组
				k = j;                        //记录最小值的位置
			}
		}
		T.arcs[k - 1][vexcode[k]-1] = T.arcs[vexcode[k]-1][k - 1] = min;    //将找到的最小边加入新图（树）中
		visited[k] = 1;                                       //标记顶点k已经选中
		for (j = 1; j <= G.vexnum; j++)                       //由于k加入，对lowcost数组进行更新
		{
			if (G.arcs[k - 1][j - 1] < lowcost[j] && visited[j] != 1)//不在树上的顶点到新加入顶点k的距离比原来更小
			{
				lowcost[j] = G.arcs[k - 1][j - 1];
				vexcode[j] =  k ;
			}
		}
	}
	cout << endl << "树的邻接矩阵为:" << endl;
	for (i = 0; i < T.vexnum; i++)
	{
		for (j = 0; j < T.vexnum; j++)
		{
			cout <<setw(4)<< T.arcs[i][j] << ' ';
			s += T.arcs[i][j];
		}
		cout << endl;
	}
	cout << "如下为prim算法结果:"<<endl;
	cout << "最小生成树权值之和为:" << s/2<<endl;
	cout << "最小生成树的边有:"<<endl;
	for (i = 0; i < T.vexnum; i++)
	{
		for (j = i+1; j < T.vexnum; j++)
		{
			if (T.arcs[i][j])
				cout << '<' << T.vexs[i+1].code << ',' << T.vexs[j+1].code << '>' << T.arcs[i][j] << endl;
		}
	}
	return T;
}


int f[MAX_V + 1];//f[i]代表顶点i的根节点

int Find(int x)//查找并返回x的根节点
{
	while(x != f[x])
		x = f[x];
	return x;
}

void sort(Edge e[],MGraph G)   //对所有边权值进行排序
{
	int i, j;
	for (i = 1; i <=G.arcnum ; i++)
	{
		for (j = 1; j <= G.arcnum - i; j++)
		{
			if (e[j].w > e[j + 1].w)
			{
				Edge t = e[j];
				e[j] = e[j + 1];
				e[j + 1] = t;
			}
		}
	}
}

//Kruskal算法求最小生成树
void Kruskal(MGraph G)
{
	cout << "如下为Kruskal算法结果:"<<endl;
	float s=0;   //记录生成树权值之和
	for (int i = 1; i <= G.vexnum; i++)  //初始化发f[]数组，将根节点设为自己
		f[i] = i;
	sort(e, G); //对边的权值从小到大排序
	cout << "选择的边有:"<<endl;
	for (int i = 1; i <= G.arcnum; i++)
	{
		if (Find(e[i].v1) != Find(e[i].v2))   //如果一条边的两个顶点根节点不同，则被选中，然后输出
		{
			cout <<'<' << e[i].v1 << ',' << e[i].v2 << '>' << e[i].w<<endl;
			s += e[i].w;
			f[Find(e[i].v1)] = Find(e[i].v2);  //将两点根节点设为同一个点
		}
	}
	cout << "权值之和为:" << s;
}


int main()
{
	MGraph G;
	CreateGraph(G);
	Prim(G);
	Kruskal(G);
}