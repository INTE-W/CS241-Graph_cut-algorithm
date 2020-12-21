#ifndef Graph_G
#define Graph_G

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>

const double inf = 9999999.0;
using namespace std;

struct tableBody
{
	int num2_node;
	double weight;
};

struct tableHead
{
	int num1_node;
	vector<tableBody> edge_arry;
};

struct edge
{
	int p_f;	  //出发节点
	int p_e;	  //结束节点
	double weigh; //权重

	bool operator<(const edge &x) const
	{
		return weigh < x.weigh;
	}
};

struct edge_inter
{
	int p_f;	  //出发节点
	int p_e;	  //结束节点
	double weigh; //权重
	string graph_num;
};

class Graph_solution
{
private:
	int num_line;
	string file_name;
	tableHead adj[6005];									  //原图邻接表（有向）
	tableHead anti_adj[6005];								  //对应的反向图的邻接链表
	vector<int> node;										  //标记存在节点
	int visit[6001] = {0};									  //dfs时判断是否访问过
	int connect[6001] = {0};								  //连通的节点的值相等
	vector<int> *subgraph_node = new vector<int>[6001];		  //子图节点存储
	vector<edge> *Inner = new vector<edge>[6000];			  //子图内部边
	vector<edge_inter> *Inter = new vector<edge_inter>[6000]; //子图交互边
	int cnt = 0;											  //所切子图的个数
	double ans = 0.0;
	vector<int> Reach_node;		 //可到达点集
	vector<edge> Graph_edge;	 //全边集
	vector<edge> Intersec;		 //交互边
	tableHead graph_table[6000]; //建立空图邻接表subgraph_table;
	tableHead anti_graph_table[6000];
	tableHead Reachable[6000];
	vector<int> subnode; //有向图中可到节点集
	double dis[6000] = {0};
	bool vis[6000] = {0};
	//dfs求连通分量+存连通子图内的边
	void dfs(int x, int u)
	{
		visit[x] = 1;
		connect[x] = u;
		subgraph_node[u].push_back(x);
		//正向图的dfs
		for (int i = 0; i < graph_table[x].edge_arry.size(); i++)
		{
			if (!visit[graph_table[x].edge_arry[i].num2_node])
			{
				dfs(graph_table[x].edge_arry[i].num2_node, u);
			}
		}
		//反向图的dfs
		for (int i = 0; i < anti_graph_table[x].edge_arry.size(); i++)
		{
			if (!visit[anti_graph_table[x].edge_arry[i].num2_node])
			{
				dfs(anti_graph_table[x].edge_arry[i].num2_node, u);
			}
		}
	}

public:
	Graph_solution()
	{
		num_line = 0;
		file_name = "";
		for (int i = 0; i < 6005; i++)
		{
			adj[i].num1_node = i;
			adj[i].edge_arry.clear();
			anti_adj[i].num1_node = i;
			anti_adj[i].edge_arry.clear();
		}

	} //约定节点数最多为6000个，编号范围0~5999

	void add_edge(int x, int y, double wi)
	{
		tableBody tmp;
		tmp.num2_node = y;
		tmp.weight = wi;
		adj[x].edge_arry.push_back(tmp);
		tmp.num2_node = x;
		anti_adj[y].edge_arry.push_back(tmp);
	}

	void Graph_Generator_build(int x, string str)
	{
		num_line = x;
		file_name = str;
		ofstream file;
		file.open(file_name);
		node.clear();
		srand(time(0));
		int r1;
		for (int i = 0; i < num_line; i++)
		{
			r1 = rand() % 2;
			//when r1=1, generate a node. Otherwise, generate an edge
			if (r1)
			{
				int number = (rand() % 6000) / 60;
				/*while (find(node.begin(), node.end(), number) != node.end())
				{
					number = rand() % 6000;
				}*/
				if (find(node.begin(), node.end(), number) == node.end())
					node.push_back(number);
				file << "<" << number << ">" << endl;
			}
			else
			{
				int p_derive, p_end;
				double weight = 0;
				p_derive = (rand() % 6000) / 60;
				p_end = (rand() % 6000) / 60;
				weight += rand() % 100 + 1; //每条边的权值范围1~100
				weight += double(rand() / (double)RAND_MAX);
				add_edge(p_derive, p_end, weight);
				if (find(node.begin(), node.end(), p_derive) == node.end())
					node.push_back(p_derive);
				if (find(node.begin(), node.end(), p_end) == node.end())
					node.push_back(p_end);
				file << "<" << p_derive << " " << p_end << " " << weight << ">" << endl;
			}
		}
		file.close();
	}

	void Read_In(string file)
	{
		ifstream infile;
		file_name = file;
		infile.open(file.data());
		assert(infile.is_open());
		string str = "";
		while (getline(infile, str))
		{
			num_line++;
			int i = 1;
			string ss = "";
			while (isdigit(str[i]))
			{
				ss += str[i];
				i++;
			}
			int t1 = 0;
			for (int j = 0; j < ss.size(); j++)
			{
				t1 += pow(10, ss.size() - j - 1) * (ss[j] - '0');
			}
			if (find(node.begin(), node.end(), t1) == node.end())
				node.push_back(t1);
			if (str[i] == '>')
				continue;
			else
			{
				i++;
				ss = "";
				while (isdigit(str[i]))
				{
					ss += str[i];
					i++;
				}
				int t2 = 0;
				for (int j = 0; j < ss.size(); j++)
				{
					t2 += pow(10, ss.size() - j - 1) * (ss[j] - '0');
				}
				if (find(node.begin(), node.end(), t2) == node.end())
					node.push_back(t2);
				i++;
				ss = "";
				while (isdigit(str[i]))
				{
					ss += str[i];
					i++;
				}
				double wi = 0.0;
				for (int j = 0; j < ss.size(); j++)
				{
					wi += pow(10, ss.size() - j - 1) * (ss[j] - '0');
				}
				if (str[i] == '.')
				{
					int j = i + 1;
					while (isdigit(str[j]))
					{
						wi += pow(0.1, j - i) * (str[j] - '0');
						j++;
					}
				}
				add_edge(t1, t2, wi);
			}
		}
		infile.close();
	}

	void Add_file(string _file, int y)
	{
		Read_In(_file);
		num_line += y;
		ofstream file;
		file.open(file_name, ios::app);
		srand(time(0));
		int r1;
		for (int i = 0; i < y; i++)
		{
			r1 = rand() % 2;
			//when r1=1, generate a node. Otherwise, generate an edge
			if (r1)
			{
				int number = (rand() % 6000) / 60;
				/*while (find(node.begin(), node.end(), number) != node.end())
				{
					number = rand() % 6000;
				}*/
				if (find(node.begin(), node.end(), number) == node.end())
					node.push_back(number);
				file << "<" << number << ">" << endl;
			}
			else
			{
				int p_derive, p_end;
				double weight = 0;
				p_derive = (rand() % 6000) / 60;
				p_end = (rand() % 6000) / 60;
				weight += rand() % 100 + 1; //每条边的权值范围1~100
				weight += double(rand() / (double)RAND_MAX);
				add_edge(p_derive, p_end, weight);
				if (find(node.begin(), node.end(), p_derive) == node.end())
					node.push_back(p_derive);
				if (find(node.begin(), node.end(), p_end) == node.end())
					node.push_back(p_end);
				file << "<" << p_derive << " " << p_end << " " << weight << ">" << endl;
			}
		}
		file.close();
	}

	void get_subgraph()
	{
		for (int i = 1; i <= cnt; i++)
		{
			for (int j = 0; j < subgraph_node[i].size(); j++)
			{
				for (int k = 0; k < adj[subgraph_node[i][j]].edge_arry.size(); k++)
				{
					edge tmp1;
					edge_inter tmp2;
					tmp1.p_f = subgraph_node[i][j];
					tmp1.p_e = adj[tmp1.p_f].edge_arry[k].num2_node;
					tmp1.weigh = adj[tmp1.p_f].edge_arry[k].weight;
					if (connect[tmp1.p_f] != connect[tmp1.p_e])
					{
						tmp2.p_f = subgraph_node[i][j];
						tmp2.p_e = adj[tmp2.p_f].edge_arry[k].num2_node;
						tmp2.weigh = adj[tmp2.p_f].edge_arry[k].weight;
						string ss = "G";
						ss += to_string(connect[tmp2.p_e]);
						tmp2.graph_num = ss;
						Inter[i].push_back(tmp2);
					}
					else
					{
						Inner[i].push_back(tmp1);
					}
				}
			}
		}
	}
	void Graph_cut(string file, int n)
	{
		int temp2 = 0;
		Read_In(file); //读取到adj，anti_adj与node中
		for (int i = 0; i < 6001; i++)
		{
			if (find(node.begin(), node.end(), i) != node.end())
			{
				edge tem;

				for (int j = 0; j < adj[i].edge_arry.size(); j++)
				{
					tem.p_f = i;
					tem.p_e = adj[i].edge_arry[j].num2_node;
					tem.weigh = adj[i].edge_arry[j].weight;
					bool flag = true;
					for (int k = 0; k < Graph_edge.size(); k++) //合并同向边
					{
						if (tem.p_e == Graph_edge[k].p_e && tem.p_f == Graph_edge[k].p_f)
						{
							temp2++;
							Graph_edge[k].weigh += tem.weigh;
							flag = false;
							break;
						}
					}
					if (flag)
						Graph_edge.push_back(tem);
				}
			}
		}

		priority_queue<edge> edge_order;
		edge tmp;
		bool *flag2 = new bool[Graph_edge.size()];
		for (int i = 0; i < Graph_edge.size(); i++)
		{
			flag2[i] = true;
		}
		//合并方向相反的重边（自环没有影响）并压入优先队列（大根堆）
		for (int j = 0; j < Graph_edge.size(); j++)
		{
			tmp.p_e = Graph_edge[j].p_e;
			tmp.p_f = Graph_edge[j].p_f;
			tmp.weigh = Graph_edge[j].weigh;
			//检查方向相反的重边
			for (int k = j + 1; k < Graph_edge.size(); k++)
			{
				if (Graph_edge[j].p_e == Graph_edge[k].p_f && Graph_edge[j].p_f == Graph_edge[k].p_e)
				{

					tmp.weigh = Graph_edge[j].weigh + Graph_edge[k].weigh;
					flag2[k] = false;
					break;
				}
			}
			if (flag2[j])
				edge_order.push(tmp);
		}
		delete[] flag2;
		//简单无向图（无重边）的分割（贪心算法） O(n)

		for (int j = 0; j < 6000; j++)
		{
			graph_table[j].num1_node = j;
			graph_table[j].edge_arry.clear();
			anti_graph_table[j].num1_node = j;
			anti_graph_table[j].edge_arry.clear();
		}
		int count = edge_order.size();
		while (count--)
		{
			int _visit[6000] = {0};
			edge current = edge_order.top();
			edge_order.pop();
			tableBody tb;
			tb.num2_node = current.p_e;
			tb.weight = current.weigh;
			graph_table[current.p_f].edge_arry.push_back(tb);
			tb.num2_node = current.p_f;
			anti_graph_table[current.p_e].edge_arry.push_back(tb);
			//检查新加入的边所在连通支节点的个数
			stack<int> _stack;
			int judge = 0;
			_stack.push(current.p_f);
			judge++;
			_visit[current.p_f] = 1;
			while (!_stack.empty())
			{
				int cur = _stack.top();
				_stack.pop();
				for (int j = 0; j < graph_table[cur].edge_arry.size(); j++)
				{
					if (!_visit[graph_table[cur].edge_arry[j].num2_node])
					{
						_stack.push(graph_table[cur].edge_arry[j].num2_node);
						_visit[graph_table[cur].edge_arry[j].num2_node] = 1;
						judge++;
					}
				}
				for (int j = 0; j < anti_graph_table[cur].edge_arry.size(); j++)
				{
					if (!_visit[anti_graph_table[cur].edge_arry[j].num2_node])
					{
						_stack.push(anti_graph_table[cur].edge_arry[j].num2_node);
						_visit[anti_graph_table[cur].edge_arry[j].num2_node] = 1;
						judge++;
					}
				}
			}
			if (judge > n) //删除新加入的边
			{
				graph_table[current.p_f].edge_arry.pop_back();
				anti_graph_table[current.p_e].edge_arry.pop_back();
				Intersec.push_back(current);
			}
		}
		for (int i = 0; i < 6001; i++)
		{
			if (find(node.begin(), node.end(), i) != node.end())
			{
				if (!connect[i])
				{
					cnt++;
					dfs(i, cnt);
				}
			}
		}
		for (int i = 0; i < Intersec.size(); i++)
		{
			ans += Intersec[i].weigh;
		}
		//优化：O(n^3)判断交换两个子图中两点，交互是否减小
		int p1, p2, p3, p4;
		bool flag;
		double minus = 0.0;
		double plus = 0.0;
		for (int tt = 0; tt < 100; tt++)
		{
			p1 = 0;
			p2 = 0;
			p3 = 0;
			p4 = 0;
			flag = false;
			double gap = 0.0;
			for (int i = 1; i <= cnt; i++)
			{
				for (int k = 0; k < subgraph_node[i].size(); k++)
				{
					for (int j = i + 1; j <= cnt; j++)
					{
						for (int q = 0; q < subgraph_node[j].size(); q++)
						{
							//判断交换subgraph_node[i][k]和subgraph_node[j][q]边权值是否变小
							minus = 0;
							plus = 0;
							for (int r = 0; r < adj[subgraph_node[i][k]].edge_arry.size(); r++)
							{
								if (connect[adj[subgraph_node[i][k]].edge_arry[r].num2_node] == i && adj[subgraph_node[i][k]].edge_arry[r].num2_node != subgraph_node[i][k]) //自环不增加权重
								{
									plus += adj[subgraph_node[i][k]].edge_arry[r].weight;
								}
								if (connect[adj[subgraph_node[i][k]].edge_arry[r].num2_node] == j && adj[subgraph_node[i][k]].edge_arry[r].num2_node != subgraph_node[j][q])
								{
									minus += adj[subgraph_node[i][k]].edge_arry[r].weight;
								}
							}
							for (int r = 0; r < anti_adj[subgraph_node[i][k]].edge_arry.size(); r++)
							{
								if (connect[anti_adj[subgraph_node[i][k]].edge_arry[r].num2_node] == i && anti_adj[subgraph_node[i][k]].edge_arry[r].num2_node != subgraph_node[i][k])
								{
									plus += anti_adj[subgraph_node[i][k]].edge_arry[r].weight;
								}
								if (connect[anti_adj[subgraph_node[i][k]].edge_arry[r].num2_node] == j && anti_adj[subgraph_node[i][k]].edge_arry[r].num2_node != subgraph_node[j][q])
								{
									minus += anti_adj[subgraph_node[i][k]].edge_arry[r].weight;
								}
							}
							for (int r = 0; r < adj[subgraph_node[j][q]].edge_arry.size(); r++)
							{
								if (connect[adj[subgraph_node[j][q]].edge_arry[r].num2_node] == j && adj[subgraph_node[j][q]].edge_arry[r].num2_node != subgraph_node[j][q])
								{
									plus += adj[subgraph_node[j][q]].edge_arry[r].weight;
								}
								if (connect[adj[subgraph_node[j][q]].edge_arry[r].num2_node] == i && adj[subgraph_node[j][q]].edge_arry[r].num2_node != subgraph_node[i][k])
								{
									minus += adj[subgraph_node[j][q]].edge_arry[r].weight;
								}
							}
							for (int r = 0; r < anti_adj[subgraph_node[j][q]].edge_arry.size(); r++)
							{
								if (connect[anti_adj[subgraph_node[j][q]].edge_arry[r].num2_node] == j && anti_adj[subgraph_node[j][q]].edge_arry[r].num2_node != subgraph_node[j][q])
								{
									plus += anti_adj[subgraph_node[j][q]].edge_arry[r].weight;
								}
								if (connect[anti_adj[subgraph_node[j][q]].edge_arry[r].num2_node] == i && anti_adj[subgraph_node[j][q]].edge_arry[r].num2_node != subgraph_node[i][k])
								{
									minus += anti_adj[subgraph_node[j][q]].edge_arry[r].weight;
								}
							}

							if (gap < minus - plus)
							{
								flag = true;
								//cout << plus << " " << -minus << endl;
								gap = minus - plus;
								p1 = i;
								p2 = k;
								p3 = j;
								p4 = q;
							}
						}
					}
				}
			}
			if (flag)
			{
				//cout << tt << " " << ans - gap << endl;
				//cout << p1 <<" " << p2 << " " << p3 << " " << p4 << endl;
				//cout << subgraph_node[p1][p2] << " " << subgraph_node[p3][p4] << endl;
				ans -= gap;
				connect[subgraph_node[p1][p2]] = p3;
				connect[subgraph_node[p3][p4]] = p1;
				int tp1 = subgraph_node[p1][p2];
				for (int i = 0; i < subgraph_node[p1].size(); i++)
				{
					if (subgraph_node[p1][i] == subgraph_node[p1][p2])
					{
						subgraph_node[p1].erase(subgraph_node[p1].begin() + i);
						subgraph_node[p1].push_back(subgraph_node[p3][p4]);
						break;
					}
				}
				for (int i = 0; i < subgraph_node[p3].size(); i++)
				{
					if (subgraph_node[p3][i] == subgraph_node[p3][p4])
					{
						subgraph_node[p3].erase(subgraph_node[p3].begin() + i);
						subgraph_node[p3].push_back(tp1);
						break;
					}
				}
			}
		}
		//输出验证
		/*minus = 0;
		int trmp1 = 0;
		int trmp2 = 0;
		
			for (int i = 1; i <= cnt; i++)
			{
				cout << subgraph_node[i].size() << endl;
				for (int k = 0; k < subgraph_node[i].size(); k++)
				{
					cout << subgraph_node[i][k] << " ";
					for (int r = 0; r < adj[subgraph_node[i][k]].edge_arry.size(); r++)
					{
						if (connect[adj[subgraph_node[i][k]].edge_arry[r].num2_node] != i )
						{
							minus += adj[subgraph_node[i][k]].edge_arry[r].weight;
							trmp1++;
						}
						else
						{
							trmp2++;
						}
					}
				}
				cout << endl;
			}
			cout << trmp1 << " " << trmp2 << endl;
			cout << minus << endl;*/

		get_subgraph();
		//cout << p1 << " " << p3 << endl;

		for (int i = 1; i <= cnt; i++)
		{
			string file_name = "G" + to_string(i) + ".txt";
			ofstream file;
			file.open(file_name);
			file << "----------The inner situation of the subgraph:----------- " << endl;
			if (Inner[i].size() == 0)
			{
				for (int j = 0; j < subgraph_node[i].size(); j++)
					file << "<" << subgraph_node[i][j] << ">" << endl;
			}
			else
			{
				for (int j = 0; j < Inner[i].size(); j++)
				{
					file << "<" << Inner[i][j].p_f << " " << Inner[i][j].p_e << " " << Inner[i][j].weigh << ">" << endl;
				}
			}
			file << endl;
			file << "----------The Interactive edge of the subgraph:-----------" << endl;
			for (int j = 0; j < Inter[i].size(); j++)
			{
				file << "<" << Inter[i][j].p_f << " " << Inter[i][j].graph_num << ":" << Inter[i][j].p_e << " " << Inter[i][j].weigh << ">" << endl;
			}
			file.close();
		}
	}

	double Intersetion()
	{
		return ans;
	}

	void Accessible(string ss)
	{
		string sub_file_name = "";
		int pos = 0;
		while (ss[pos] != ':')
		{
			sub_file_name += ss[pos];
			pos++;
		}
		string ttp = sub_file_name;
		sub_file_name += ".txt";
		int num = 0;
		for (int i = pos + 1; i < ss.size(); i++)
		{
			num += (ss[i] - '0') * pow(10, ss.size() - i - 1);
		}
		if (find(Reach_node.begin(), Reach_node.end(), num) == Reach_node.end())
		{
			Reach_node.push_back(num);
		}
		ifstream infile;
		file_name = sub_file_name;
		infile.open(sub_file_name.data());
		assert(infile.is_open());
		string str = "";
		while (getline(infile, str))
		{
			if (str[0] != '<' || str == "")
				continue;
			else
			{
				int pp = 1;
				while (str[pp] != ' ' && str[pp] != '>')
					pp++;
				if (str[pp] == '>')
					continue;
				int tmp = 0;
				for (int i = 1; i < pp; i++)
					tmp += (str[i] - '0') * pow(10, pp - i - 1);
				if (tmp == num)
				{
					pp++;
					int tp = pp;
					if (str[pp] != 'G') //内部边
					{
						while (str[pp] != ' ')
							pp++;
						int tmp2 = 0;
						for (int i = tp; i < pp; i++)
							tmp2 += (str[i] - '0') * pow(10, pp - i - 1);
						if (find(Reach_node.begin(), Reach_node.end(), tmp2) == Reach_node.end())
						{
							Reach_node.push_back(tmp2);
							string con = ttp + ":" + to_string(tmp2);
							Accessible(con);
						}
					}
					else
					{
						string dp = "";
						while (str[pp] != ' ')
						{
							dp += str[pp];
							pp++;
						}
						int pos2 = 0;
						while (dp[pos2] != ':')
						{
							pos2++;
						}
						int num2 = 0;
						for (int i = pos2 + 1; i < dp.size(); i++)
						{
							num2 += (dp[i] - '0') * pow(10, dp.size() - i - 1);
						}
						if (find(Reach_node.begin(), Reach_node.end(), num2) == Reach_node.end())
						{
							Reach_node.push_back(num2);
							Accessible(dp);
						}
					}
				}
			}
		}
		infile.close();
	}

	void show_accessible()
	{
		for (int i = 0; i < Reach_node.size(); i++)
			cout << Reach_node[i] << endl;
		return;
	}

	void Read_In2(string ss)
	{
		string sub_file_name = "";
		int pos = 0;
		while (ss[pos] != ':')
		{
			sub_file_name += ss[pos];
			pos++;
		}
		string tpp = sub_file_name;
		sub_file_name += ".txt";
		int num = 0;
		for (int i = pos + 1; i < ss.size(); i++)
		{
			num += (ss[i] - '0') * pow(10, ss.size() - i - 1);
		}
		ifstream infile;
		file_name = sub_file_name;
		infile.open(sub_file_name.data());
		assert(infile.is_open());
		string str = "";
		while (getline(infile, str))
		{
			if (str[0] != '<' || str == "")
				continue;
			else
			{
				int pp = 1;
				while (str[pp] != ' ' && str[pp] != '>')
					pp++;
				int tmp = 0;
				for (int i = 1; i < pp; i++)
					tmp += (str[i] - '0') * pow(10, pp - i - 1);
				if (str[pp] == '>')
					continue;
				tableBody adj_edge;
				if (tmp == num)
				{
					pp++;
					int tp = pp;
					if (str[pp] != 'G') //内部边
					{
						while (str[pp] != ' ')
							pp++;
						int tmp2 = 0;
						for (int i = tp; i < pp; i++)
							tmp2 += (str[i] - '0') * pow(10, pp - i - 1);
						adj_edge.num2_node = tmp2;
						pp++;
						string str2 = "";
						while (isdigit(str[pp]))
						{
							str2 += str[pp];
							pp++;
						}
						double wi = 0.0;
						for (int j = 0; j < str2.size(); j++)
						{
							wi += pow(10, str2.size() - j - 1) * (str2[j] - '0');
						}
						if (str[pp] == '.')
						{
							int j = pp + 1;
							while (isdigit(str[j]))
							{
								wi += pow(0.1, j - pp) * (str[j] - '0');
								j++;
							}
						}
						adj_edge.weight = wi;
						if (adj_edge.num2_node != num)
						{
							bool fg = true;
							int loc = 0;
							for (int i = 0; i < Reachable[num].edge_arry.size(); i++)
							{
								if (Reachable[num].edge_arry[i].num2_node == adj_edge.num2_node)
								{
									fg = false;
									loc = i;
									break;
								}
							}
							if (fg)
								Reachable[num].edge_arry.push_back(adj_edge);
							else
							{
								if (Reachable[num].edge_arry[loc].weight > adj_edge.weight)
									Reachable[num].edge_arry[loc].weight = adj_edge.weight;
							}
						}
						if (find(subnode.begin(), subnode.end(), num) == subnode.end())
							subnode.push_back(num);
						if (find(subnode.begin(), subnode.end(), adj_edge.num2_node) == subnode.end())
						{
							subnode.push_back(adj_edge.num2_node);
							string con = tpp + ":" + to_string(tmp2);
							Read_In2(con);
						}
					}
					else
					{
						string dp = "";
						while (str[pp] != ' ')
						{
							dp += str[pp];
							pp++;
						}
						int pos2 = 0;
						while (dp[pos2] != ':')
						{
							pos2++;
						}
						int num2 = 0;
						for (int i = pos2 + 1; i < dp.size(); i++)
						{
							num2 += (dp[i] - '0') * pow(10, dp.size() - i - 1);
						}
						adj_edge.num2_node = num2;
						string str3 = "";
						pp++;
						while (isdigit(str[pp]))
						{
							str3 += str[pp];
							pp++;
						}
						double wi = 0.0;
						for (int j = 0; j < str3.size(); j++)
						{
							wi += pow(10, str3.size() - j - 1) * (str3[j] - '0');
						}
						if (str[pp] == '.')
						{
							int j = pp + 1;
							while (isdigit(str[j]))
							{
								wi += pow(0.1, j - pp) * (str[j] - '0');
								j++;
							}
						}
						adj_edge.weight = wi;
						if (adj_edge.num2_node != num)
						{
							bool fg = true;
							int loc = 0;
							for (int i = 0; i < Reachable[num].edge_arry.size(); i++)
							{
								if (Reachable[num].edge_arry[i].num2_node == adj_edge.num2_node)
								{
									fg = false;
									loc = i;
									break;
								}
							}
							if (fg)
								Reachable[num].edge_arry.push_back(adj_edge);
							else
							{
								if (Reachable[num].edge_arry[loc].weight > adj_edge.weight)
									Reachable[num].edge_arry[loc].weight = adj_edge.weight;
							}
						}
						if (find(subnode.begin(), subnode.end(), num) == subnode.end())
							subnode.push_back(num);
						if (find(subnode.begin(), subnode.end(), adj_edge.num2_node) == subnode.end())
						{
							subnode.push_back(adj_edge.num2_node);
							Read_In2(dp);
						}
					}
				}
			}
		}
		infile.close();
	}

	void Dijkstra(string _start, string _end)
	{
		//memset(dis, inf, sizeof(dis));
		for (int ii = 0; ii < subnode.size(); ii++)
			dis[subnode[ii]] = inf;
		int pos = 0;
		while (_start[pos] != ':')
		{
			pos++;
		}
		int num = 0;
		for (int ii = pos + 1; ii < _start.size(); ii++)
		{
			num += (_start[ii] - '0') * pow(10, _start.size() - ii - 1);
		}
		dis[num] = 0.0;
		double min;
		int minp;
		for (int i = 0; i < 6000; i++)
		{
			if (find(subnode.begin(), subnode.end(), i) != subnode.end())
			{
				min = inf;
				for (int j = 0; j < 6000; j++)
				{
					if (find(subnode.begin(), subnode.end(), j) != subnode.end())
					{
						if (!vis[j] && dis[j] < min)
						{
							min = dis[j];
							minp = j;
						}
					}
				}

				vis[minp] = 1;
				for (int u = 0; u < Reachable[minp].edge_arry.size(); u++)
				{
					if (!vis[Reachable[minp].edge_arry[u].num2_node] && dis[Reachable[minp].edge_arry[u].num2_node] > dis[minp] + Reachable[minp].edge_arry[u].weight)
					{
						dis[Reachable[minp].edge_arry[u].num2_node] = dis[minp] + Reachable[minp].edge_arry[u].weight;
					}
				}
			}
		}
		pos = 0;
		while (_end[pos] != ':')
		{
			pos++;
		}

		num = 0;
		for (int ii = pos + 1; ii < _end.size(); ii++)
		{
			num += (_end[ii] - '0') * pow(10, _end.size() - ii - 1);
		}
		bool gg = false;
		for (int i = 0; i < subnode.size(); i++)
		{
			if (subnode[i] == num)
			{
				gg = true;
				break;
			}
		}
		if (gg)
		{
			cout << "The shortest path is: " << endl;
			cout << dis[num];
		}
		else
		{
			cout << "the two nodes are unreachable!" << endl;
		}
	}

	void Shortest_path1(int x, int num_gra)
	{
		bool flag1 = false;
		string ss1 = "";
		for (int i = 1; i <= num_gra; i++)
		{
			ifstream infile;
			string ss = "G" + to_string(i) + ".txt";
			infile.open(ss.data());
			assert(infile.is_open());
			string str = "";
			while (getline(infile, str))
			{
				if (flag1)
				{
					cout << ss1 << " (please press 1 to search the nodes it can reach)" << endl;
					return;
				}
				if (str == "")
					break;
				else if (str[0] != '<')
					continue;
				else
				{
					int pp = 1;
					while (str[pp] != ' ' && str[pp] != '>')
						pp++;
					int tmp1 = 0;
					for (int k = 1; k < pp; k++)
						tmp1 += (str[k] - '0') * pow(10, pp - k - 1);

					if (str[pp] == '>')
					{
						if (!flag1 && x == tmp1)
						{
							ss1 += ("G" + to_string(i) + ":" + to_string(x));
							flag1 = true;
						}
					}
					else if (str[pp] != '>')
					{
						pp++;
						int tmp2 = 0;
						int tp = pp;
						while (str[pp] != ' ')
							pp++;
						for (int k = tp; k < pp; k++)
							tmp2 += (str[k] - '0') * pow(10, pp - k - 1);

						if (!flag1 && (x == tmp1 || x == tmp2))
						{
							ss1 += ("G" + to_string(i) + ":" + to_string(x));
							flag1 = true;
						}
					}
				}
			}
			infile.close();
		}
		if (!flag1)
		{
			cout << "This node is not in the graph! Please 0 to end the program!" << endl;
			return;
		}
	}

	void Shortest_path2(int x, int y, int num_gra)
	{
		bool flag1 = false;
		bool flag2 = false;
		string ss1 = "", ss2 = "";
		for (int i = 1; i <= num_gra; i++)
		{
			ifstream infile;
			string ss = "G" + to_string(i) + ".txt";
			infile.open(ss.data());
			assert(infile.is_open());
			string str = "";
			while (getline(infile, str))
			{
				if (flag1 && flag2)
				{
					cout << ss1 << "  " << ss2;
					return;
				}
				if (str == "")
					break;
				else if (str[0] != '<')
					continue;
				else
				{
					int pp = 1;
					while (str[pp] != ' ' && str[pp] != '>')
						pp++;
					int tmp1 = 0;
					for (int k = 1; k < pp; k++)
						tmp1 += (str[k] - '0') * pow(10, pp - k - 1);
					if (str[pp] == '>')
					{
						if (!flag1 && x == tmp1)
						{
							ss1 += ("G" + to_string(i) + ":" + to_string(x));
							flag1 = true;
						}
						if (!flag2 && y == tmp1)
						{
							ss2 += ("G" + to_string(i) + ":" + to_string(y));
							flag2 = true;
						}
					}
					else
					{
						pp++;
						int tp = pp;
						while (str[pp] != ' ')
							pp++;
						int tmp2 = 0;
						for (int k = tp; k < pp; k++)
							tmp2 += (str[k] - '0') * pow(10, pp - k - 1);
						if (!flag1 && (x == tmp1 || x == tmp2))
						{
							ss1 += ("G" + to_string(i) + ":" + to_string(x));
							flag1 = true;
						}
						if (!flag2 && (y == tmp1 || y == tmp2))
						{
							ss2 += ("G" + to_string(i) + ":" + to_string(y));
							flag2 = true;
						}
					}
				}
			}
			infile.close();
		}
		if (!flag1 && !flag2)
		{
			cout << "These two nodes are not in the graph!" << endl;
			return;
		}
		else if (!flag1 && flag2)
		{
			cout << "The first node is not in the graph!" << endl;
			return;
		}
		else if (flag1 && !flag2)
		{
			cout << "The second node is not in the graph!" << endl;
			return;
		}
	}

	~Graph_solution()
	{
		delete[] subgraph_node;
		delete[] Inner;
		delete[] Inter;
	};
};

#endif
