	#include<iostream>
	#include<string>
	#include<fstream>
	using namespace std;

	//�洢JOSN��ʽ���ݵĽṹ��
	struct object
	{
		string key;
		string value;
	};

	int main()
	{ 
		//f����Ƿ��ѯ������1��ʾ�ɹ���0��ʾʧ��
		//len��¼�������ݵĳ���
		int i, j, n, m,len,k=0,f;
		//��depth��¼{}�Ĳ�������flag1����Ǽ�ֵ��1Ϊ����2Ϊֵ��flag2���λ���ַ����ڻ����ַ����⣬0���⣬1���ڣ�
		int depth = 0, flag1 , flag2 = 0;
		//���ļ������������뵽allstr��
		string allstr;
		//s��ʱ�洢һ����Ϣ
		string s[101],s1 = "", st[101], searchkey;
		cout << "������Ҫ�������������n��Ҫ��ѯ����������m:";
		cin >> n >> m;
		getchar();
		ifstream in;
		in.open("JSON����.txt");
		if (!in)
		{
			cout << "���ļ�ʧ��!";
			exit(1);
		}
		for (i = 0; i < n; i++)
		{
			getline(in, s[i]);
			allstr = allstr+ s[i];
		}
		in.close();
		object JSON[10001];
		len = allstr.length();
		//���������ݵķ��Ž��д���
		for (i = 0; i < len; i++)
		{
			//����{������1���ҽ�����������Ϊ��
			if (allstr[i] == '{')
			{
				depth++;
				flag1 = 1;
				if (depth > 1)
				{
					//��k�������ֵ��һ������
					JSON[k].value = "-1";
					k++;
				}
			}
			//����}������1����������������ȻΪ��һ������ļ�
			else if (allstr[i] == '}')
			{
				depth--;
				flag1 = 1;
			}
			//����:����������Ϊֵ
			else if (allstr[i] == ':')
			{
				flag1 = 2;
			}
			//����,����������Ϊ��
			else if (allstr[i] == ',')
			{
				flag1 = 1;
			}
			//����˫����"��ǰһ���ַ���Ϊ��б��
			else if (allstr[i] == '"' && allstr[i-1]!='\\')
			{
				if (flag2 == 0)
				{
					//��"��ʾ�Ӵ��⵽����
					flag2 = 1;
					s1 = "";
				}
				//���������"��ʾ�Ӵ��ڵ�����
				else
				{
					flag2 = 0;
					//�����ǰΪ��
					if (flag1 == 1)
					{
						st[depth] = s1;
						//�����ǰ�ڵ�1��
						if (depth == 1)
						{
							JSON[k].key = s1;
						}
						//������ڵ�1��
						else
						{
							JSON[k].key += st[1];
							//Ϊÿһ���.��ʾ�ֲ�
							for (j = 2; j <= depth; j++)
							{
								JSON[k].key = JSON[k].key + '.'+st[j];
							}
						}
					}
					//�����ǰΪֵ
					else
					{
						JSON[k].value = s1;
						k++;
					}
				}
			}
			//������б��\��λ�ڴ���
			else if (allstr[i] == '\\' && flag2 == 1)
			{
				//���ڷ�б��λ�ڴ����ǳ�˫���֣������ѭ������ֱ�Ӽ�1��ʾɾȥһ����б��
				i++;
				s1 += allstr[i];
			}
			//�����ո�ͻ���
			else if (allstr[i] == ' ' || allstr[i] == '\n')
				continue;
			else if (flag2 == 1)
				s1 += allstr[i];
		}
		cout << "������" << m << "��Ҫ�����ļ�" << endl;
		for (i = 0; i < m; i++)
		{
			cin >> searchkey;
			f = 0;
			for (j = 0; j < k; j++)
			{
				if (JSON[j].key == searchkey)
				{
					f = 1;
					if (JSON[j].value == "-1")
					{
						cout << "OBJECT" << endl;
					}
					else
					{
						cout << "STRING" << ' ' << JSON[j].value << endl;
					}
				}
			}
			if (f == 0)
			{
				cout << "NOTEXIST" << endl;
			}
		}
		return 0;
	}