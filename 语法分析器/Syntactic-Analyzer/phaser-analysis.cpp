// lr1�﷨����.cpp: �������̨Ӧ�ó������ڵ㡣
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

#define syntactic_nonterminal_FILE_PATH "./�﷨���ս��.txt"   
#define syntactic_terminal_FILE_PATH "./�﷨�ս��.txt"
#define syntactic_grammar_FILE_PATH "./�﷨�ķ�.txt"
#define grammar_input_FILE_PATH "./�﷨����.txt"

struct wf         //�����ķ�  
{				  //e.g. �ķ�(0)E->S -- left = E || right = S || num = 0 || length = 1 
	string left;  //�ķ���
	string right; //�ķ��Ҳ�
	int num;      //�ķ����
	int length;   //��Լ����
};

struct pro            //һ���ķ�ʽ��+Բ��������ս��
{					  //e.g. I0����E->��S,# || num = 0 || position = 0 || search = '#'
	int num;      	  //�ڼ����ķ�
	int position;     //Բ��λ��
	vector<char>  search;  //ԭ��������ս��
};

struct itemset      //pro�ļ��ϣ�һ��״̬�������кܶ��pro��GOTOͼ�п������Ķ�����
{
	string name;    //״̬����
	vector<pro> t;  //����״̬�е���Ŀt
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

//�Ƚ�����pro�Ƿ����
bool operator == (const pro& p1, const pro& p2)
{
	if (p1.num == p2.num && p1.position == p2.position && comp(p1.search, p2.search) && comp(p2.search, p1.search))
		return true;
	return false;
}
//�Ƚ�����pro������״̬���Ƿ����
bool operator == (const itemset& c1, const itemset& c2)
{
	if (comp(c1.t, c2.t) && comp(c2.t, c1.t))
		return true;
	else
		return false;
}


vector<char> getFirst(vector<wf> phaser, string s, vector<string> Vt)    //�õ�First��
{
	vector<char> search;
	if (s[0]) //���s[0]��Ϊ��
	{
		vector<string> toEmpty;   //���Ƴ��յ��﷨��
		for (int i = 0; i < phaser.size(); i++)
		{
			if (phaser[i].right == "$")
				toEmpty.push_back(phaser[i].left);
		}

		string t;
		t = s[0];
		vector<string> ::iterator it = find(Vt.begin(), Vt.end(), t);   //�жϵ�һ���ǲ����ս��
		if (it != Vt.end())       	//������ս������������search
		{
			search.push_back(s[0]);
		}
		else						//��������ս�������Ƿ��ս��
		{
			it = find(toEmpty.begin(), toEmpty.end(), t);
			if (it == toEmpty.end())   //�����Ƴ���
			{
				for (int i = 0; i < phaser.size(); i++)
				{
					if (t == phaser[i].left) //���t���﷨�ķ����󲿷��ս��
					{
						vector<char> s1 = getFirst(phaser, phaser[i].right, Vt); //�ݹ�������е��﷨�ķ��󲿷��ս����first��
						for (int j = 0; j < s1.size(); j++)
						{
							if (find(search.begin(), search.end(), s1[j]) == search.end())  //���first����û��s1����s1����first��
								search.push_back(s1[j]);
						}
					}
				}
			}
			else   //���Ƴ���
			{
				for (int i = 0; i < phaser.size(); i++)
				{
					if (t == phaser[i].left)  //t���﷨�ķ��������
					{
						if (phaser[i].right[0] != '$')  //�Ҹ��﷨�ķ����Ҳ������ڿ�
						{
							vector<char> s1 = getFirst(phaser, phaser[i].right, Vt);//�ݹ�������е��﷨�ķ��󲿷��ս����first��
							for (int j = 0; j < s1.size(); j++)
							{
								if (find(search.begin(), search.end(), s1[j]) == search.end())//���first����û��s1����s1����first��
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
					if (find(search.begin(), search.end(), s1[j]) == search.end()) //���s1����search�������
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
	vector<wf> phaser;//�洢�ķ�
	vector<itemset> C;
	map<string, string> m;
	itemset clo;//һ��״̬
	vector<string> Vn, Vt;    //Vn�д����﷨���ս��  Vt�д����﷨�ս��
	// ��ȡ�﷨���ս��������Vn
	file.open(syntactic_nonterminal_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'�﷨���ս��.txt',please check!";
	}
	else
	{
		while (file >> buff)
			Vn.push_back(buff);
	}
	file.close();

	// ��ȡ�﷨�ս��������Vt
	file.open(syntactic_terminal_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'�﷨�ս��.txt',please check!";
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
		cout << "Can not find the file--'�﷨�ķ�.txt',please check!";
	}
	else
	{
		int line = 0;
		while (file.getline(buff, 30)) //���ļ��ж�ȡһ���ı�������ȡ 29 ���ַ����洢�� buff ������
		{					 //e.g. �ķ�(0)E->S -- left = E || right = S || num = 0 || length = 1
							 //e.g. �ķ�(2)S->S+T  left = S || right = S+T || num = 2 || length = 3
			char* s = buff;
			wf t;
			int length = 0;//�ķ��ұ߷��ų���
			t.num = line++;//�ڼ����ķ�
			t.left = *s++;//�ķ���߷���
			s++;
			s++;//����->
			if (*s == '$')
			{
				length = 0;
				t.right += *s;
			}
			else
				while (*s)
				{
					length++;
					t.right += *s++;//�ķ��ұ߷���
				}
			t.length = length;
			phaser.push_back(t);
		}
	}
	file.close();

	pro temp;  //������Ŀtemp
	temp.num = 0;
	temp.position = 0;
	temp.search.push_back('#');
	clo.t.push_back(temp);
	clo.name = "0";
	for (int i = 0; i < clo.t.size(); i++) //���кˡ� ,���closure
	{
		if (clo.t[i].position < phaser[clo.t[i].num].right.size()) //Բ��λ�ò������
		{
			string first;
			first = phaser[clo.t[i].num].right[clo.t[i].position]; //Բ�������ַ�
			if (find(Vn.begin(), Vn.end(), first) != Vn.end()) //���Բ�������ַ�Ϊ���ս��
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
							search.push_back(search1[k]);  //�õ���ǰ������search����

					}

				}
				temp.search = search;  //�õ���Ŀtemp����ǰ������search����
				for (int j = 0; j < phaser.size(); j++)
				{
					if (phaser[clo.t[i].num].right[clo.t[i].position] == phaser[j].left[0]) //���Բ�����ַ������﷨��ߵ��ַ�
					{
						temp.num = phaser[j].num;
						temp.position = 0;

						if (find(clo.t.begin(), clo.t.end(), temp) == clo.t.end()) //����¼ӵ���Ŀ�ǲ��ظ���
						{
							int flag = 1;
							for (int k = 0; k < clo.t.size(); k++)
							{
								if (clo.t[k].num == temp.num && clo.t[k].position == temp.position) //�����ͬ�ļ�
								{
									flag = 0;
									for (int n = 0; n < temp.search.size(); n++)
									{
										if (find(clo.t[k].search.begin(), clo.t[k].search.end(), temp.search[n]) == clo.t[k].search.end())//���ͬ�ļ���search�в�����ǰtemp���ַ�
										{
											clo.t[k].search.push_back(temp.search[n]);//�ͽ�temp��search����ͬ�ļ���
										}
									}
								}
							}
							if (flag) //����¼ӵ���Ŀ���ظ��Ҳ���ͬ�ļ�
								clo.t.push_back(temp);  //�ͽ���Ŀ����clo
						}

					}
				}

			}
		}
	}
	C.push_back(clo);//GOTOͼ��̬
	clo.t.clear();

	for (int i = 0; i < C.size(); i++)//����ÿ����Ŀ��
	{
		vector<string> used;//�����ù��������used
		for (int j = 0; j < C[i].t.size(); j++)//��ǰ��Ŀ����ÿ�����ܵ������
		{
			string first;
			first = phaser[C[i].t[j].num].right[C[i].t[j].position]; //Բ�����ַ�
			if (C[i].t[j].position < phaser[C[i].t[j].num].right.size() && find(used.begin(), used.end(), first) == used.end())
				//���Բ�㲻������Ҹ������û��ʹ�ù�
			{
				used.push_back(first);  //�������������used
				for (int k = 0; k < C[i].t.size(); k++)//�ø������������ǰ��Ŀ��
				{
					string cmp;
					cmp = phaser[C[i].t[k].num].right[C[i].t[k].position];//Բ������
					if (first == cmp) //���Բ���ķ��ź͸��������ͬ
					{
						temp.num = C[i].t[k].num;
						temp.position = C[i].t[k].position + 1;
						temp.search = C[i].t[k].search;
						clo.t.push_back(temp);//�ѵ�ǰ�ַ������ĺ˼���clo
					}
				}

				for (int i = 0; i < clo.t.size(); i++)//���к� ���closure
				{
					if (clo.t[i].position < phaser[clo.t[i].num].right.size())//Բ��λ�ò������
					{
						string first;
						first = phaser[clo.t[i].num].right[clo.t[i].position];//Բ�������ַ�
						if (find(Vn.begin(), Vn.end(), first) != Vn.end())
							//���Բ�������ַ�Ϊ���ս��
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
										//����¼ӵ���Ŀ�ǲ��ظ���
									{
										int flag = 1;
										for (int k = 0; k < clo.t.size(); k++)
										{
											if (clo.t[k].num == temp.num && clo.t[k].position == temp.position)
												//�����ͬ����Ŀ
											{
												flag = 0;
												for (int n = 0; n < temp.search.size(); n++)
												{
													if (find(clo.t[k].search.begin(), clo.t[k].search.end(), temp.search[n]) == clo.t[k].search.end())
														//���ͬ����Ŀ��search�в�����ǰtemp���ַ�
													{
														clo.t[k].search.push_back(temp.search[n]);

													}
												}
											}
										}

										if (flag)  //�������ͬ����Ŀ,�����closure��
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
					if (it == C.end())//���clo��C���ǲ��ظ���
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

	for (vector<itemset>::iterator it = C.begin(); it != C.end(); it++)//�����Լ��λ��
	{
		cout << "----------------------" << endl;
		cout << it->name[0] - '0' << endl;
		for (int i = 0; i < it->t.size(); i++)//��Ŀ����ÿ����Ŀt��i��
		{
			//cout << phaser[it->t[i].num].left << "->" << phaser[it->t[i].num].right << "  " << phaser[it->t[i].num].right[it->t[i].position];
			cout << phaser[it->t[i].num].left << "->";
			for (int j = 0; j < phaser[it->t[i].num].right.size(); j++) {
				if (j == it->t[i].position) {
					cout << "��";
				}
				cout << phaser[it->t[i].num].right[j];
			}
			if (it->t[i].position == phaser[it->t[i].num].right.size())
				cout << "��";

			cout << ",  ";
			for (int j = 0; j < it->t[i].search.size(); j++)
				cout << it->t[i].search[j] << " ";//�����ǰ������search
			cout << endl;
			char t;
			t = phaser[it->t[i].num].right[it->t[i].position];//��Ŀ����ÿ����Ŀ��Բ�����ַ�
			if (!t)//Ϊ��ʱ
			{
				string key, value;
				value = "r";
				if (phaser[it->t[i].num].right[it->t[i].position - 1] == 'S')
					//�涨SΪ�﷨�ķ��Ŀ�ʼ����,��Լ��Sʱ���� 
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
	//���GOTOͼ״̬��ϵ
	//�õ���������m
	for (map<string, string>::iterator it = m.begin(); it != m.end(); ++it) {
		//string result = "S";
		//result += it->second[it->second.size()-1]-'0';
		std::cout << "״̬S" << it->first[0] - '0' << "�������" << it->first[1] << "��ת�� " << it->second << std::endl;
	}
	cout << "-------------------------------------------------------------------------------------";
	cout << "\nLR(1)������:\n";
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
	vector <string> states;   //״̬ջ
	vector <string> symbols;  //����ջ
	vector <string> in;		  //�����
	states.push_back("0");
	symbols.push_back("#");
	file.open(grammar_input_FILE_PATH, ios::in);
	if (!file.is_open())
	{
		cout << "Can not find the file--'�﷨����.txt',please check!";
	}
	else
	{
		while (file >> buff)
			in.push_back(buff);
	}
	file.close();
	in.push_back("#");

	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "������ķ�Ϊ:\n";
	for (int i = 0; i < phaser.size(); i++)
		cout << i << "   " << phaser[i].left << "->" << phaser[i].right << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
	cout << "��������Ϊ:\n\n";
	cout << setw(3) << "����" << setw(25) << "״̬ջ" << setw(17) << "����ջ" << setw(26) << "�����" << setw(8) << "����" << endl << endl;

	int count = 1;//������
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
		string key;   //���ж�Ӧ���к���
		string action;//��ȡ���ж���
		char ch;	  //ʶ������ƽ����ǹ�Լ
		key += states.back();
		key += in[i];
		map<string, string> ::iterator it = m.find(key);
		if (it == m.end())    //����Ϊ�� -> ���Ӵ���
		{
			cout << setw(8) <<"error�������+" << endl;
			system("pause");
			return 0;
		}
		action = it->second;
		if (action.size() == 2)
			cout << setw(5) << action[0] << action[1] - '0' << endl;
		else
			cout << setw(8) << action << endl;
		if (action == "acc")    //����Ϊacc -> ����
		{
			cout << "�﷨ʶ��ɹ�\n";
			system("pause");
			return 0;
		}
		ch = action[0];
		if (ch == 'S')   //�ƽ�
		{
			char n = action[1];
			string state, symbol;//�����״̬ ����
			state = n;
			symbol = in[i];
			states.push_back(state);
			symbols.push_back(symbol);
		}
		else    //��Լ
		{
			i--;
			char n = action[1];
			int num = n - '0';
			for (int i = 0; i < phaser[num].length; i++)//�Ƴ�����ʽ�ұߵķ��� �Ͷ�Ӧ״̬
			{
				symbols.pop_back();
				states.pop_back();
			}
			symbols.push_back(phaser[num].left);//������  ��goto
			string key;
			string state;
			key += states.back();
			key += phaser[num].left;
			map<string, string> ::iterator it = m.find(key);
			if (it == m.end())    //����Ϊ��  ���Ӵ���
			{
				cout << setw(8) << "error�������+" << endl;
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
