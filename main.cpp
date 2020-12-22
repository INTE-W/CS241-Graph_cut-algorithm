#include <iostream>
#include<fstream>
#include<string>
#include"Graph_algorithm.hpp"



using namespace std;


int main()
{
	int x;
	string ss;
	int option;
	Graph_solution example;
	cout << "Please choose the function:" << endl;
	cout << "       1.Create a new Graph (Press 1)" << endl;
	cout << "       2.Add lines in an exsited Graph (Press 2)" << endl;
	cout << "       3.Graph_cut Algorithm (Press 3)" << endl;
	cout << "       4.Accessible nodes (Press 4)" << endl;
	cout << "       5.Shortest path (Press 5)" << endl;

	cin >> option;
	if (option == 1)
	{
		cout << "please input the total lines of the Graph(an Integer range 1~6000)" << endl;
		cin >> x;
		cout << "please input the name of file(With the suffix: .txt)" << endl;
		cin >> ss;
		example.Graph_Generator_build(x, ss);
		cout << endl;
		cout << "Generate Successfully! Please check " << ss << " for the data" << endl;
	}
	else if (option == 2)
	{
		cout << "please input the name of file(With the suffix: .txt)" << endl;
		cin >> ss;
		cout << "Please input the number of lines you want to add" << endl;
		cin >> x;
		example.Add_file(ss, x);
		cout << endl;
		cout << "Add Successfully! Please check " << ss << " for the data" << endl;
	}
	else if (option == 3)
	{
		cout << "please input the name of file(With the suffix: .txt)" << endl;
		cin >> ss;
		cout << "Please input the maximum number of nodes in subgraph" << endl;
		cin >> x;
		example.Graph_cut(ss, x);
		cout << "The minimum answer I can find is: ";
		cout << example.Intersetion() << endl;
	}
	else if (option == 4)
	{
		int ty1,ty2;
		int fl = 0;
		cout << "Please input the largest number of subgraph" << endl;
		cin >> ty1;
		cout << "Please input the number of the node " << endl;
		cin >> ty2;
		example.Shortest_path1(ty2,ty1);
		cin >> fl;
		if (fl == 1)
		{
			cout << "Please input the information above of this node" << endl;
			cin >> ss;
			cout << "This node can reach the other node below:" << endl;
			example.Accessible(ss);
			example.show_accessible();
		}
	}
	else if (option == 5)
	{
		int st, en;
		int tot;
		string str1, str2;
		cout << "Please input the largest number of subgraph" << endl;
		cin >> tot;
		cout << "Please input the number of two node to Search the information of them" << endl;
		cin >> st;
		cin >> en;
		cout << "Here are the information about these two nodes:" << endl;
		example.Shortest_path2(st,en,tot);
		cout << endl;
		int flg = 0;
		cout << "Moreover, if you want to get the shortest path of the two nodes, please press 1(otherwise, press 0 to end)" << endl;
		cin >> flg;
		if (flg == 1)
		{
			cout << "Please input the information of the two nodes" << endl;
			cin >> str1 >> str2;
			example.Read_In2(str1);
			example.Dijkstra(str1, str2);
			cout<<endl;
			int pri[6000];
			int _=0;
			int r=en;
			int s=st;
			pri[_++]=en;
			while(r!=s)
			{
				pri[_++]=way[r];
				r=way[r];
			}
			for(int i=_-1;i>=0;i--)
			{
				if(i!=_-1)
					cout<<"->";
				cout<<pri[i];
			}
		}

	}
	return 0;
}
