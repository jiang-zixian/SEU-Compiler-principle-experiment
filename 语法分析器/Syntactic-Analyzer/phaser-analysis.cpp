// lr1语法分析.cpp: 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <tchar.h>
#include<iostream>
#include<string> 
#include<fstream>
#include<vector>
#include<map>
#include<iomanip>
using namespace std;

#define syntactic_nonterminal_FILE_PATH "./语法非终结符.txt"   
#define syntactic_terminal_FILE_PATH "./语法终结符.txt"
#define syntactic_grammar_FILE_PATH "./语法文法.txt"
#define grammar_input_FILE_PATH "./语法输入.txt"

struct wf         //定义文法  
{				  //e.g. 文法(0)E->S -- left = E || right = S || num = 0 || length = 1 
	string left;  //文法左部
	string right; //文法右部
	int num;      //文法序号
	int length;   //归约长度
};

struct pro            //一条文法式子+圆点后会跟的终结符
{					  //e.g. I0包括E->·S,# || num = 0 || position = 0 || search = '#'
	int num;      	  //第几条文法
	int position;     //圆点位置
	vector<char>  search;  //原点后会跟的终结符
};

struct itemset      //pro的集合，一个状态（里面有很多个pro，GOTO图中框起来的东西）
{
	string name;    //状态名称
	vector<pro> t;  //定义状态中的项目t
};

bool comp(vector<pro> v1, vector<pro> v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		if (find(v1.begin(), v1.end(), v2[i]) == v1.end())
			return false;
	}
	return true;
}
bool comp(vector<char> v1, vector<char> v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		if (find(v1.begin(), v1.end(), v2[i]) == v1.end())
			return false;
	}
	return true;
}

//比较两个pro是否相等
bool operator == (const pro& p1, const pro& p2)
{
	if (p1.num == p2.num && p1.position == p2.position && comp(p1.search, p2.search) && comp(p2.search, p1.search))
		return true;
	return false;
}
//比较两个pro集（即状态）是否相等
bool operator == (const itemset& c1, const itemset& c2)
{
	if (comp(c1.t, c2.t) && comp(c2.t, c1.t))
		return true;
	else
		return false;
}


vector<char> getFirst(vector<wf> phaser, string s, vector<string> Vt)    //得到First集
{
	vector<char> search;
	if (s[0]) //如果s[0]不为空
	{
		vector<string> toEmpty;   //能推出空的语法左部
		for (int i = 0; i < phaser.size(); i++)
		{
			if (phaser[i].right == "$")
				toEmpty.push_back(phaser[i].left);
		}

		string t;
		t = s[0];
		vector<string> ::iterator it = find(Vt.begin(), Vt.end(), t);   //判断第一个是不是终结符
		if (it != Vt.end())       	//如果是终结符，将它加入search
		{
			search.push_back(s[0]);
		}
		else						//如果不是终结符，即是非终结符
		{
			it = find(toEmpty.begin(), toEmpty.end(), t);
			if (it == toEmpty.end())   //不能推出空
			{
				for (int i = 0; i < phaser.size(); i++)
				{
					if (t == phaser[i].left) //如果t是语法文法的左部非终结符
					{
						vector<char> s1 = getFirst(phaser, phaser[i].right, Vt); //递归求得所有的语法文法左部非终结符的first集
						for (int j = 0; j < s1.size(); j++)
						{
							if (find(search.begin(), search.end(), s1[j]) == search.end())  //如果first集中没有s1，则将s1加入first集
								search.push_back(s1[j]);
						}
					}
				}
			}
			else   //能推出空
			{
				for (int i = 0; i < phaser.size(); i++)
				{
					if (t == phaser[i].left)  //t和语法文法的左部相等
					{
						if (phaser[i].right[0] != '$')  //且该语法文法的右部不等于空
						{
							vector<char> s1 = getFirst(phaser, phaser[i].right, Vt);//递归求得所有的语法文法左部非终结符的first集
							for (int j = 0; j < s1.size(); j++)
							{
								if (find(search.begin(), search.end(), s1[j]) == search.end())//如果first集中没有s1，则将s1加入first集
									search.push_back(s1[j]);
							}

						}
					}
				}
				string ss;
				for (int i = 1; i < s.length(); i++)
					ss += s[i];
				vector<char> s1 = getFirst(phaser, ss, Vt);
				for (int j = 0; j < s1.size(); j++)
				{
					if (find(search.begin(), search.end(), s1[j]) == search.end()) //如果s1不在search中则加入
						search.push_back(s1[j]);
				}
			}
		}
	}

	return search;
}

int main()
{
	fstream file;
	char buff[30];
	vector<wf> phaser;//存储文法
	vector<itemset> C;
	map<string, string> m;
	itemset clo;//一个状态
	vector<string> Vn, Vt;    //Vn中储存语法非终结符  Vt中储存语法终结符
	// 读取语法非终结符，存入Vn
	file.open(syntactic_nonterminal_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'语法非终结符.txt',please check!";
	}
	else
	{
		while (file >> buff)
			Vn.push_back(buff);
	}
	file.close();

	// 读取语法终结符，存入Vt
	file.open(syntactic_terminal_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'语法终结符.txt',please check!";
	}
	else
	{
		while (file >> buff)
			Vt.push_back(buff);
	}
	file.close();

	file.open(syntactic_grammar_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'语法文法.txt',please check!";
	}
	else
	{
		int line = 0;
		while (file.getline(buff, 30)) //从文件中读取一行文本，最多读取 29 个字符，存储在 buff 数组中
		{					 //e.g. 文法(0)E->S -- left = E || right = S || num = 0 || length = 1
							 //e.g. 文法(2)S->S+T  left = S || right = S+T || num = 2 || length = 3
			char* s = buff;
			wf t;
			int length = 0;//文法右边符号长度
			t.num = line++;//第几行文法
			t.left = *s++;//文法左边符号
			s++;
			s++;//跳过->
			if (*s == '$')
			{
				length = 0;
				t.right += *s;
			}
			else
				while (*s)
				{
					length++;
					t.right += *s++;//文法右边符号
				}
			t.length = length;
			phaser.push_back(t);
		}
	}
	file.close();

	pro temp;  //定义项目temp
	temp.num = 0;
	temp.position = 0;
	temp.search.push_back('#');
	clo.t.push_back(temp);
	clo.name = "0";
	for (int i = 0; i < clo.t.size(); i++) //已有核· ,求出closure
	{
		if (clo.t[i].position < phaser[clo.t[i].num].right.size()) //圆点位置不在最后
		{
			string first;
			first = phaser[clo.t[i].num].right[clo.t[i].position]; //圆点后面的字符
			if (find(Vn.begin(), Vn.end(), first) != Vn.end()) //如果圆点后面的字符为非终结符
			{
				string s;
				vector<char> search;
				for (int j = clo.t[i].position + 1; j < phaser[clo.t[i].num].right.length(); j++)
					s += phaser[clo.t[i].num].right[j];
				for (int j = 0; j < clo.t[i].search.size(); j++)
				{
					string t = s;
					t += clo.t[i].search[j];
					vector<char> search1 = getFirst(phaser, t, Vt);
					for (int k = 0; k < search1.size(); k++)
					{
						if (find(search.begin(), search.end(), search1[k]) == search.end())
							search.push_back(search1[k]);  //得到向前搜索符search集合

					}

				}
				temp.search = search;  //得到项目temp的向前搜索符search集合
				for (int j = 0; j < phaser.size(); j++)
				{
					if (phaser[clo.t[i].num].right[clo.t[i].position] == phaser[j].left[0]) //如果圆点后的字符等于语法左边的字符
					{
						temp.num = phaser[j].num;
						temp.position = 0;

						if (find(clo.t.begin(), clo.t.end(), temp) == clo.t.end()) //如果新加的项目是不重复的
						{
							int flag = 1;
							for (int k = 0; k < clo.t.size(); k++)
							{
								if (clo.t[k].num == temp.num && clo.t[k].position == temp.position) //如果是同心集
								{
									flag = 0;
									for (int n = 0; n < temp.search.size(); n++)
									{
										if (find(clo.t[k].search.begin(), clo.t[k].search.end(), temp.search[n]) == clo.t[k].search.end())//如果同心集的search中不含当前temp的字符
										{
											clo.t[k].search.push_back(temp.search[n]);//就将temp的search加入同心集中
										}
									}
								}
							}
							if (flag) //如果新加的项目不重复且不是同心集
								clo.t.push_back(temp);  //就将项目加入clo
						}

					}
				}

			}
		}
	}
	C.push_back(clo);//GOTO图初态
	clo.t.clear();

	for (int i = 0; i < C.size(); i++)//处理每个项目集
	{
		vector<string> used;//定义用过的输入符used
		for (int j = 0; j < C[i].t.size(); j++)//当前项目集中每个可能的输入符
		{
			string first;
			first = phaser[C[i].t[j].num].right[C[i].t[j].position]; //圆点后的字符
			if (C[i].t[j].position < phaser[C[i].t[j].num].right.size() && find(used.begin(), used.end(), first) == used.end())
				//如果圆点不在最后且该输入符没有使用过
			{
				used.push_back(first);  //将该输入符加入used
				for (int k = 0; k < C[i].t.size(); k++)//用该输入符遍历当前项目集
				{
					string cmp;
					cmp = phaser[C[i].t[k].num].right[C[i].t[k].position];//圆点后符号
					if (first == cmp) //如果圆点后的符号和该输入符相同
					{
						temp.num = C[i].t[k].num;
						temp.position = C[i].t[k].position + 1;
						temp.search = C[i].t[k].search;
						clo.t.push_back(temp);//把当前字符产生的核加入clo
					}
				}

				for (int i = 0; i < clo.t.size(); i++)//已有核 求出closure
				{
					if (clo.t[i].position < phaser[clo.t[i].num].right.size())//圆点位置不在最后
					{
						string first;
						first = phaser[clo.t[i].num].right[clo.t[i].position];//圆点后面的字符
						if (find(Vn.begin(), Vn.end(), first) != Vn.end())
							//如果圆点后面的字符为非终结符
						{
							string s;
							vector<char> search;
							for (int j = clo.t[i].position + 1; j < phaser[clo.t[i].num].right.length(); j++)
								s += phaser[clo.t[i].num].right[j];
							for (int j = 0; j < clo.t[i].search.size(); j++)
							{
								string t = s;
								t += clo.t[i].search[j];
								vector<char> search1 = getFirst(phaser, t, Vt);
								for (int k = 0; k < search1.size(); k++)
								{
									if (find(search.begin(), search.end(), search1[k]) == search.end())
										search.push_back(search1[k]);

								}

							}
							temp.search = search;
							for (int j = 0; j < phaser.size(); j++)
							{
								if (phaser[clo.t[i].num].right[clo.t[i].position] == phaser[j].left[0])
								{
									temp.num = phaser[j].num;
									temp.position = 0;

									if (find(clo.t.begin(), clo.t.end(), temp) == clo.t.end())
										//如果新加的项目是不重复的
									{
										int flag = 1;
										for (int k = 0; k < clo.t.size(); k++)
										{
											if (clo.t[k].num == temp.num && clo.t[k].position == temp.position)
												//如果是同心项目
											{
												flag = 0;
												for (int n = 0; n < temp.search.size(); n++)
												{
													if (find(clo.t[k].search.begin(), clo.t[k].search.end(), temp.search[n]) == clo.t[k].search.end())
														//如果同心项目的search中不含当前temp的字符
													{
														clo.t[k].search.push_back(temp.search[n]);

													}
												}
											}
										}

										if (flag)  //如果不是同心项目,则加入closure中
											clo.t.push_back(temp);
									}

								}
							}

						}
					}
				}

				if (clo.t.size() != 0)
				{
					string key, value;
					value = "S";
					key = C[i].name + first;
					vector<itemset>::iterator it = find(C.begin(), C.end(), clo);
					if (it == C.end())//如果clo在C中是不重复的
					{
						clo.name = clo.name[0] + 1;
						value += clo.name;
						C.push_back(clo);

					}
					else
						value += it->name;
					clo.t.clear();
					m.insert(pair<string, string>(key, value));
				}

			}

		}
		used.clear();
	}

	for (vector<itemset>::iterator it = C.begin(); it != C.end(); it++)//填入归约的位置
	{
		cout << "----------------------" << endl;
		cout << it->name[0] - '0' << endl;
		for (int i = 0; i < it->t.size(); i++)//项目集中每个项目t【i】
		{
			//cout << phaser[it->t[i].num].left << "->" << phaser[it->t[i].num].right << "  " << phaser[it->t[i].num].right[it->t[i].position];
			cout << phaser[it->t[i].num].left << "->";
			for (int j = 0; j < phaser[it->t[i].num].right.size(); j++) {
				if (j == it->t[i].position) {
					cout << "·";
				}
				cout << phaser[it->t[i].num].right[j];
			}
			if (it->t[i].position == phaser[it->t[i].num].right.size())
				cout << "·";

			cout << ",  ";
			for (int j = 0; j < it->t[i].search.size(); j++)
				cout << it->t[i].search[j] << " ";//输出向前搜索符search
			cout << endl;
			char t;
			t = phaser[it->t[i].num].right[it->t[i].position];//项目集中每个项目中圆点后的字符
			if (!t)//为空时
			{
				string key, value;
				value = "r";
				if (phaser[it->t[i].num].right[it->t[i].position - 1] == 'S')
					//规定S为语法文法的开始符号,归约到S时结束 
				{
					value = "acc";
					key = it->name;
					key += "#";
					m.insert(pair<string, string>(key, value));
				}
				else
				{
					value += to_string(it->t[i].num);
					key = it->name;
					for (int j = 0; j < it->t[i].search.size(); j++)
						m.insert(pair<string, string>(key + it->t[i].search[j], value));
				}
			}
		}
	}
	//输出GOTO图状态关系
	//用迭代器遍历m
	for (map<string, string>::iterator it = m.begin(); it != m.end(); ++it) {
		//string result = "S";
		//result += it->second[it->second.size()-1]-'0';
		std::cout << "状态S" << it->first[0] - '0' << "读入符号" << it->first[1] << "跳转到 " << it->second << std::endl;
	}
	cout << "-------------------------------------------------------------------------------------";
	cout << "\nLR(1)分析表:\n";
	cout << '\t';
	for (int i = 0; i < Vt.size(); i++)
		//cout << setw(5) << setfill(' ') << Vt[i];
		cout << Vt[i] << '\t';
	for (int i = 0; i < Vn.size(); i++)
		//cout << setw(5) << setfill(' ') << Vn[i];
		cout << Vn[i] << '\t';

	cout << endl;
	for (int i = 0; i < C.size(); i++)
	{
		cout << C[i].name[0] - '0' << '\t';

		for (int j = 0; j < Vt.size(); j++)
		{
			string value = "";
			string key;
			key += C[i].name[0];
			key += Vt[j];
			map<string, string> ::iterator it = m.find(key);
			if (it != m.end())
				value = m[key];
			if (value.size() == 2)
				//cout << setw(5) << setfill(' ') << value[0] << value[1] - '0' ;
				cout << value[0] << value[1] - '0'<<'\t';
			else
				//cout << setw(5) << setfill(' ') << value ;
				cout <<  value << '\t';
		}
		for (int j = 0; j < Vn.size(); j++)
		{
			string value = "";
			string key;
			key += C[i].name[0];
			key += Vn[j];
			map<string, string> ::iterator it = m.find(key);
			if (it != m.end())
				value = m[key];
			if (value.size() == 2)
				//cout << setw(5) << setfill(' ') << value[0] << value[1] - '0';
				cout << value[1] - '0'<< '\t';
			else
				//cout << setw(5) << setfill(' ') << value;
				cout << value << '\t';
		}
		cout << endl;
	}
	vector <string> states;   //状态栈
	vector <string> symbols;  //符号栈
	vector <string> in;		  //输入符
	states.push_back("0");
	symbols.push_back("#");
	file.open(grammar_input_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'语法输入.txt',please check!";
	}
	else
	{
		while (file >> buff)
			in.push_back(buff);
	}
	file.close();
	in.push_back("#");

	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "输入的文法为:\n";
	for (int i = 0; i < phaser.size(); i++)
		cout << i << "   " << phaser[i].left << "->" << phaser[i].right << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
	cout << "分析过程为:\n\n";
	cout << setw(3) << "步骤" << setw(25) << "状态栈" << setw(17) << "符号栈" << setw(26) << "输入符" << setw(8) << "动作" << endl << endl;

	int count = 1;//步骤数
	for (int i = 0; i < in.size(); i++)
	{
		cout << setw(3) << count++;
		string t1;
		for (int j = 0; j < states.size(); j++)
		{
			t1 += to_string(states[j][0] - '0');
			t1 += " ";
		}
		cout << setw(25) << t1;
		t1 = "";
		for (int j = 0; j < symbols.size(); j++)
		{
			t1 += symbols[j];

		}
		cout << setw(17) << t1;
		t1 = "";
		for (int j = i; j < in.size(); j++)
		{
			t1 += in[j];
		}
		cout << setw(26) << t1;
		string key;   //表中对应的行和列
		string action;//获取表中动作
		char ch;	  //识别进行移进还是归约
		key += states.back();
		key += in[i];
		map<string, string> ::iterator it = m.find(key);
		if (it == m.end())    //表中为空 -> 句子错误
		{
			cout << setw(8) <<"error，意外的+" << endl;
			system("pause");
			return 0;
		}
		action = it->second;
		if (action.size() == 2)
			cout << setw(5) << action[0] << action[1] - '0' << endl;
		else
			cout << setw(8) << action << endl;
		if (action == "acc")    //动作为acc -> 结束
		{
			cout << "语法识别成功\n";
			system("pause");
			return 0;
		}
		ch = action[0];
		if (ch == 'S')   //移进
		{
			char n = action[1];
			string state, symbol;//移入的状态 符号
			state = n;
			symbol = in[i];
			states.push_back(state);
			symbols.push_back(symbol);
		}
		else    //归约
		{
			i--;
			char n = action[1];
			int num = n - '0';
			for (int i = 0; i < phaser[num].length; i++)//移出产生式右边的符号 和对应状态
			{
				symbols.pop_back();
				states.pop_back();
			}
			symbols.push_back(phaser[num].left);//移入左部  并goto
			string key;
			string state;
			key += states.back();
			key += phaser[num].left;
			map<string, string> ::iterator it = m.find(key);
			if (it == m.end())    //表中为空  句子错误
			{
				cout << setw(8) << "error，意外的+" << endl;
				system("pause");
				return 0;
			}
			action = it->second;
			state = action[1];
			states.push_back(state);
		}
	}
	system("pause");
	return 0;
}
