	#include<iostream>
	#include<string>
	#include<fstream>
	using namespace std;

	//存储JOSN格式数据的结构体
	struct object
	{
		string key;
		string value;
	};

	int main()
	{ 
		//f标记是否查询到键，1表示成功，0表示失败
		//len记录所有数据的长度
		int i, j, n, m,len,k=0,f;
		//用depth记录{}的层数，用flag1来标记键值；1为键，2为值；flag2标记位于字符串内还是字符串外，0是外，1是内；
		int depth = 0, flag1 , flag2 = 0;
		//将文件所有内容输入到allstr中
		string allstr;
		//s临时存储一行信息
		string s[101],s1 = "", st[101], searchkey;
		cout << "请输入要输入的数据行数n和要查询的数据行数m:";
		cin >> n >> m;
		getchar();
		ifstream in;
		in.open("JSON查找.txt");
		if (!in)
		{
			cout << "打开文件失败!";
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
		//对所有数据的符号进行处理
		for (i = 0; i < len; i++)
		{
			//遇到{层数加1，且接下来的内容为键
			if (allstr[i] == '{')
			{
				depth++;
				flag1 = 1;
				if (depth > 1)
				{
					//第k个对象的值是一个对象
					JSON[k].value = "-1";
					k++;
				}
			}
			//遇到}层数减1，接下来的内容依然为下一个对象的键
			else if (allstr[i] == '}')
			{
				depth--;
				flag1 = 1;
			}
			//遇到:接下来内容为值
			else if (allstr[i] == ':')
			{
				flag1 = 2;
			}
			//遇到,接下来内容为键
			else if (allstr[i] == ',')
			{
				flag1 = 1;
			}
			//遇到双引号"且前一个字符不为反斜杠
			else if (allstr[i] == '"' && allstr[i-1]!='\\')
			{
				if (flag2 == 0)
				{
					//该"表示从串外到串内
					flag2 = 1;
					s1 = "";
				}
				//否则该引号"表示从串内到串外
				else
				{
					flag2 = 0;
					//如果当前为键
					if (flag1 == 1)
					{
						st[depth] = s1;
						//如果当前在第1层
						if (depth == 1)
						{
							JSON[k].key = s1;
						}
						//如果不在第1层
						else
						{
							JSON[k].key += st[1];
							//为每一层加.表示分层
							for (j = 2; j <= depth; j++)
							{
								JSON[k].key = JSON[k].key + '.'+st[j];
							}
						}
					}
					//如果当前为值
					else
					{
						JSON[k].value = s1;
						k++;
					}
				}
			}
			//遇到反斜杠\且位于串内
			else if (allstr[i] == '\\' && flag2 == 1)
			{
				//由于反斜杠位于串内是成双出现，因此用循环次数直接加1表示删去一个反斜杠
				i++;
				s1 += allstr[i];
			}
			//遇到空格和换行
			else if (allstr[i] == ' ' || allstr[i] == '\n')
				continue;
			else if (flag2 == 1)
				s1 += allstr[i];
		}
		cout << "请输入" << m << "行要搜索的键" << endl;
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