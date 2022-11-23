#include<iostream>
#include<string>
#include<fstream>
#define MAX_V 10000
using namespace std;
int visited[MAX_V] = { 0 };

struct Bus
{
	int No;   //���������
	int PassNum;   //����վ�������
	int route[100] = {0}; //��������·��
};

struct Station
{
	string name;  //��վ�������
	int bus[30];  //������վ�㹫�������
	int PassNum;  //������վ�㹫����������
};

struct Map
{
	Bus bus[1000];    //�Ͼ��Ĺ�������Ϣ����990·����
	int BusNum;      //����������
	Station station[10000];    //�Ͼ�������վ����Ϣ
	int StationNum;     //վ������
};

//������еĸ���Ԫ��
struct element
{
	int bus;    //�õ�Ĺ��������
	int before;   //�����վ��ǰһվ��ı��
	int cur;     //��վ��ı��
};

//���нڵ�ṹ��
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

//��������
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
		cout << "��Ҫ�˳�ϵͳ�����롮�˳���!"<<endl;
		cout << "�����������:";
		cin >> qd;
		if (qd == "�˳�")
			break;
		if (!locatestation(M, qd))
		{
			cout << "������������Ƿ�����!";
			continue;
		}
		cout << "�������յ���;";
		cin >> zd;
		if (zd == "�˳�")
			break;
		if (!locatestation(M, qd))
		{
			cout << "�����յ������Ƿ�����!";
			continue;
		}
		cout << "����ת��·��:" << endl;
		MinTransfer(M, qd, zd);
		cout << "����վ��·��:" << endl;
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

//Ѱ��xxx·�������ı��
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

//Ѱ��nameվ�ı��
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

//Ѱ��tվ�ı�ţ���δ�ҵ�tվ����tվ��Ŵ���Map�͵�ǰ��������·��վ���У����ظ�վ���
int GetBusStation(Map& M, string t)
{
	int i = locatestation(M, t);
	if (!i)     //û���ҵ���վ������վ��Ϣ����Map
	{
		M.station[++M.StationNum].name = t;
		i = M.StationNum;
	}
	M.bus[M.BusNum].route[M.bus[M.BusNum].PassNum] = i;   //����վ���뵱ǰ������·����
	M.station[i].bus[++M.station[i].PassNum] = M.bus[M.BusNum].No;  //���ù��������뵱ǰվ�����Ĺ�������
	t = "";
	return i;
}

//��ȡ�ļ�����
void CreateMap(Map& M)
{
	int k = 0;
	string temp;  //��ʱ�洢һ��վ���ı���
	char ch='0';
	ifstream in;
	in.unsetf(ios::skipws);
	in.open("�Ͼ�������·.txt");
	if (!in)
	{
		cout << "���ļ� �Ͼ�������·.txt ʧ��";
		exit(1);
	}
	for (int i = 0; i < 1000; i++)
	{
		M.bus[i].PassNum = 1;   //�����й���������վ������Ϊ1
	}
	in >> M.bus[++M.BusNum].No; //������Ϊ1�Ĺ�����·��
	in >> ch;   //��ȡ�����������֡�·��
	in >> ch;
	while (1)
	{
		in >> ch;
		if (ch == ',')     //��ȡ����һ���ַ�Ϊ����˵��ǰ��һ��վ���Ѿ���ȡ���
		{
			GetBusStation(M, temp);
			M.bus[M.BusNum].PassNum++;
			temp = "";
			continue;
		}
		if (ch=='\n')   //��ȡ������ʱ��˵��ǰ��һ��վ���Ѿ���ȡ���
		{
			GetBusStation(M, temp);
			temp = "";
			if (in.eof())
			{
				break;
			}
			in >> M.bus[++M.BusNum].No;   //���к��ȡ��Ӧ��Ź�����·��
			in >> ch;  //��ȡ�����������֡�·��
			in >> ch;
			continue;
		}
		if (in.eof())   //�����ļ�β����˵����ȡ��һ��վ�����
		{
			GetBusStation(M, temp);
			break;
		}
		if (ch == ' ')
		{
			continue;
		}
		temp += ch;  //����ȡ��ÿ�����ۼ���һ��վ��
	}
	in.close();
}

//����ļ�����
void out(Map M)
{
	for (int i = 1; i <= M.BusNum; i++)
	{
		cout << M.bus[i].No << "·\t";
		for (int j = 1; j <= M.bus[i].PassNum; j++)
		{
			cout << M.station[M.bus[i].route[j]].name<<' ';
		}
		cout << endl;
	}
}

//��ʼ������
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

//����
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

//����
element DeQueue(LinkQueue& Q)
{
	element e;
	if (Q.front == Q.rear)
	{
		cout << "�����ѿ�!";
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

//���ٶ���
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

//Ѱ�һ�������·��
int MinTransfer(Map M, string startpoint, string destination)
{
	int v1 = locatestation(M, startpoint);  //����Ӧ���
	int v2 = locatestation(M, destination);   //�յ��Ӧ���
	int t;
	t = v1;    //������յ��Ż����������ڶ������ݻ�Ѱ·
	v1 = v2;
	v2 = t;
	for (int i = 1; i < MAX_V; i++)    //��ʼ���������
	{
		visited[i] = 0;
	}
	element e[8000];
	for (int i = 1; i < 8000; i++)
	{
		e[i].cur = i;         //������Ԫ�ص�ǰվ����Ϊ����
		e[i].bus = 0;         //��ʼ�������Ĺ���
	}
	element e0;    //���Ԫ�أ���⵽e0���ӱ�ʾ��ȱ�����һ��Ѱ·��ɣ���������ѭ��
	int count = 1;    //��¼����վ����������2��ʾת������
	LinkQueue Q;
	InitQueue(Q);
	EnQueue(Q, e[v1]);  //�յ����(Ϊ��һ�㣩
	visited[v1] = 1;    //�յ���Ϊ�Ѿ�����
	while (1)
	{
		int k = 1;
		count++;
		e0.cur = -1;
		EnQueue(Q, e0);
		while (Q.front != Q.rear)
		{
			v1 = DeQueue(Q).cur;  //����������һ������Ϊ�յ� 
			if (v1 == -1)
			{
				break;
			}
			for (int i = 1; i <= M.station[v1].PassNum; i++) //���Ծ����յ�Ĳ�ͬ����
			{
				int bus1 = locatebus(M, M.station[v1].bus[i]);  //����bus1Ϊ�����յ�ĵ�i�������ı��
				for (int j = 1; j <= M.bus[bus1].PassNum; j++) //���Ե�ǰ�����Ƿ��ܵ������
				{
					if (M.bus[bus1].route[j] == v2)        //Ѱ�ҵ��������ɴ�ת���������Ĺ���
					{
						int bus2;
						bus2 = locatebus(M, e[v1].bus);   //ѡ��bus2Ϊ�����о����յ������������
						cout << "����ת��" << count - 2 << "��" << endl;
						cout << M.station[v2].name << "(����" << M.station[v1].bus[i] << "·������)-->";
						count--;
						while (count--)
						{
							if (count == 0)
							{
								cout << M.station[v1].name << "(�յ�)" << endl;
								DestroyQueue(Q);
								return 1;
							}
							else
							{
								cout << M.station[v1].name << "(����" << M.bus[bus2].No << "·��������-->";
								v1 = e[v1].before;   //��v1��Ϊ���յ�Ѱ·����㵽��v1��ǰһ����������Ҳ�����������һ��������
								bus2 = locatebus(M, e[v1].bus);  //��bus2��Ϊ��һվ������Ĺ������Ա����
							}
						}
					}
					if (!visited[M.bus[bus1].route[j]])   //�����վ��δ������
					{
						e[M.bus[bus1].route[j]].bus = M.bus[bus1].No;  //�����õ�Ĺ�������Ϊbus1
						e[M.bus[bus1].route[j]].before = v1;          //�õ����һ����Ϊv1��Ҳ�����������һ����
						EnQueue(Q, e[M.bus[bus1].route[j]]);   //���������յ����ڵĵ���ӣ���Ϊ��һ���յ���й�ȱ�����ֱ���ҵ����	
						visited[M.bus[bus1].route[j]] = 1;     //����վ����Ϊ�Ѿ�����
					}
				}

			}
		}
	}
}

//Ѱ�Ҿ���վ������·��
int MinStation(Map M, string startpoint, string destination)
{
	int v1 = locatestation(M, startpoint);
	int v2 = locatestation(M, destination);
	int t;
	t = v1;      //����һ��������ͬ��������յ��Ż����������ڶ������ݻ�Ѱ·,�������
	v1 = v2;
	v2 = t;
	for (int i = 1; i < MAX_V; i++)
	{
		visited[i] = 0;
	}
	element e[8000];
	for (int i = 1; i < 8000; i++)
	{
		e[i].cur = i;         //������Ԫ�ص�ǰվ����Ϊ����
		e[i].bus = 0;         //��ʼ�������Ĺ���
	}
	element e0;    //���Ԫ�أ���⵽e0���ӱ�ʾ��ȱ�����һ��Ѱ·��ɣ���������ѭ��
	int count = 1;    //��¼������վ������
	LinkQueue Q;
	InitQueue(Q);
	EnQueue(Q, e[v1]);  //�յ����(Ϊ��һ�㣩
	visited[v1] = 1;    //�յ���Ϊ�Ѿ�����
	while (1)
	{
		int k = 1;
		count++;
		e0.cur = -1;
		EnQueue(Q, e0);
		while (Q.front != Q.rear)
		{
			v1 = DeQueue(Q).cur;    //����������һ������Ϊ�յ�
			if (v1 == -1)           //��ȱ�����ı�־
			{
				break;
			}
			for (int i = 1; i <= M.station[v1].PassNum; i++)    //�����о����յ�Ĺ�����ɸѡ
			{
				int bus1 = locatebus(M, M.station[v1].bus[i]);   //bus1Ϊ�����յ��ĳһ������
				for (int j = 1; j <= M.bus[bus1].PassNum; j++)   //��bus1·��������վ�����ɸѡ
				{
					if (M.bus[bus1].route[j] == v1)       //�ҵ�bus1·�����յ��λ��
					{
						if (M.bus[bus1].route[j - 1] == v2 || M.bus[bus1].route[j + 1] == v2)  //�����յ����ڵ�һվ�������
						{
							int bus2 = e[v1].bus;   //bus2��¼��ǰ�����յ�ĳ���,�������
							cout << "���پ���" << count << "��վ��" << endl;
							cout << M.station[v2].name << "(����" << M.bus[bus1].No << "·������)-->";
							count--;
							while (count--)
							{
								if (count == 0)
								{
									cout << M.station[v1].name << "(�յ�)" << endl;
									DestroyQueue(Q);
									return 1;
								}
								else
								{
									cout << M.station[v1].name << "(����" << bus2 << "·��������-->";
								}
								v1 = e[v1].before;  //��v1��Ϊ���յ�Ѱ·����㵽��v1��ǰһվ�㣬Ҳ�����������һվ��
								bus2 = e[v1].bus;  //bus2���¼�¼����һ���յ�Ĺ���
							}
							
						}
						//���뵱ǰ�յ����ڵĳ�վȫ�����
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