#include<iostream>
#include<fstream>
#include <iomanip>
using namespace std;
#define MAX_V  20
int visited[MAX_V + 1];

//��űߵĽṹ��
struct Edge
{
	int v1, v2;  //�߹�������������
	float w;   //�ߵ�Ȩ��
};

Edge e[MAX_V * (MAX_V - 1) / 2];

struct VertexType
{
	int code;                                      //������
};

struct MGraph
{
	float arcs[MAX_V][MAX_V];                          //�ڽӾ���
	int vexnum,arcnum=0;                                 //������,����
	VertexType vexs[MAX_V];                          //������������
};

//����ͼ
void CreateGraph(MGraph& G)                          
{
	ifstream in;
	in.open("ͼ.txt");
	if (!in)
	{
		cout << "���ļ�ʧ��!";
		exit(1);
	}
	for (int i = 0; i <= G.vexnum; i++)
		visited[i] = 0;
	int i = 0, j = 0,k=1;
    in >> G.vexnum;
	for (i = 1; i <= G.vexnum; i++)                 //����ͼ�ж�����
	{
		in >> G.vexs[i].code;
	}
	for (i = 0; i < G.vexnum; i++)              //��ʼ������
	{
		for (j = 0; j < G.vexnum; j++)
		{
			G.arcs[i][j] = 0;
		}
	}                    
	while (!in.eof())                //������Ӿ���
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
	cout << "�ڽӾ���Ϊ:" << endl;
	for (i = 0; i < G.vexnum; i++)      //�������
	{
		for (j = 0; j < G.vexnum; j++)
			cout << setw(4) << G.arcs[i][j] << " ";
		cout << endl;
	}                                             
}

//Prim�㷨����С������
MGraph Prim(MGraph G)              
{
	MGraph T;                              //��С������
	T.vexnum = G.vexnum;                   //����������������ԭ��ͼ�еĶ�����
	visited[0] = 0;
	float s = 0;
	int vexcode[MAX_V + 1];                //��¼��Ӧ��Ŷ���������������
	float lowcost[MAX_V + 1];               //��¼��Ӧvexcode�ж������Ӧ��Ŷ������С����
	int i, j, k;
	for (i = 0; i < G.vexnum; i++)         //��ʼ����
		for (j = 0; j < G.vexnum; j++)
			T.arcs[i][j] = 0;
	for (j = 1; j <= G.vexnum; j++)         //��ʼ�����еĶ���
	{
		T.vexs[j].code = G.vexs[j].code;
	}
	for (j = 1; j <= G.vexnum; j++)       //��ʼ��Prim�㷨��������������
	{
		lowcost[j] = G.arcs[0][j - 1];
		visited[j] = 0;
		vexcode[j] =1;
	}
	visited[1] = 1;                     //Ĭ����1�Ŷ���Ϊ���������С��������������Ϊ�Ѿ�����
	for (i = 2; i <= G.vexnum; i++)     //��lowcost��Ѱ����Сֵ
	{
		float min = 999999;                  //�Ƚ���Сֵ��Ϊ��ֵ
		for (j = 1; j <= G.vexnum; j++)        //��lowcost��Ѱ��û�о�������С��ֵ
		{
			if (visited[j] != 1 && lowcost[j] < min)     //û�о�������С
			{
				min = lowcost[j];             //����Сֵ��ֵ����Ӧ��ŵ�lowcost����
				k = j;                        //��¼��Сֵ��λ��
			}
		}
		T.arcs[k - 1][vexcode[k]-1] = T.arcs[vexcode[k]-1][k - 1] = min;    //���ҵ�����С�߼�����ͼ��������
		visited[k] = 1;                                       //��Ƕ���k�Ѿ�ѡ��
		for (j = 1; j <= G.vexnum; j++)                       //����k���룬��lowcost������и���
		{
			if (G.arcs[k - 1][j - 1] < lowcost[j] && visited[j] != 1)//�������ϵĶ��㵽�¼��붥��k�ľ����ԭ����С
			{
				lowcost[j] = G.arcs[k - 1][j - 1];
				vexcode[j] =  k ;
			}
		}
	}
	cout << endl << "�����ڽӾ���Ϊ:" << endl;
	for (i = 0; i < T.vexnum; i++)
	{
		for (j = 0; j < T.vexnum; j++)
		{
			cout <<setw(4)<< T.arcs[i][j] << ' ';
			s += T.arcs[i][j];
		}
		cout << endl;
	}
	cout << "����Ϊprim�㷨���:"<<endl;
	cout << "��С������Ȩֵ֮��Ϊ:" << s/2<<endl;
	cout << "��С�������ı���:"<<endl;
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


int f[MAX_V + 1];//f[i]������i�ĸ��ڵ�

int Find(int x)//���Ҳ�����x�ĸ��ڵ�
{
	while(x != f[x])
		x = f[x];
	return x;
}

void sort(Edge e[],MGraph G)   //�����б�Ȩֵ��������
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

//Kruskal�㷨����С������
void Kruskal(MGraph G)
{
	cout << "����ΪKruskal�㷨���:"<<endl;
	float s=0;   //��¼������Ȩֵ֮��
	for (int i = 1; i <= G.vexnum; i++)  //��ʼ����f[]���飬�����ڵ���Ϊ�Լ�
		f[i] = i;
	sort(e, G); //�Աߵ�Ȩֵ��С��������
	cout << "ѡ��ı���:"<<endl;
	for (int i = 1; i <= G.arcnum; i++)
	{
		if (Find(e[i].v1) != Find(e[i].v2))   //���һ���ߵ�����������ڵ㲻ͬ����ѡ�У�Ȼ�����
		{
			cout <<'<' << e[i].v1 << ',' << e[i].v2 << '>' << e[i].w<<endl;
			s += e[i].w;
			f[Find(e[i].v1)] = Find(e[i].v2);  //��������ڵ���Ϊͬһ����
		}
	}
	cout << "Ȩֵ֮��Ϊ:" << s;
}


int main()
{
	MGraph G;
	CreateGraph(G);
	Prim(G);
	Kruskal(G);
}