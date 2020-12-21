（一）功能主页面：

Please choose the function:
	1.Create a new Graph (Press 1)
	2.Add lines in an exsited Graph (Press 2)
	3.Graph_cut Algorithm (Press 3)
	4.Accessible nodes (Press 4)
	5.Shortest path (Press 5)

（二）功能操作介绍
1.Create a new Graph (Press 1)
	a.输入随机生成函数
	b.输入输出文件的文件名（带后缀.txt）

2.Add lines in an exsited Graph (Press 2)
	a.输入追加文件的文件名（带后缀.txt）
	b.输入追加随机生成的行数

3.Graph_cut Algorithm (Press 3)
	a.输入原图的文件名（带后缀.txt）
	b.输入子图中最多包含的节点数n
	(切割生成子图存储在同一目录下G1.txt~Gk.txt中）

4.Accessible nodes (Press 4)
	a.输入切割产生的子图最大的编号
	b.输入想要查询的节点编号（若不在任何子图内，会报错）
	c.输入上述查询得到的节点信息（格式如：G1:5）

5.Shortest path (Press 5)
	a.输入切割产生的子图最大的编号
	b.输入想要查询的节点编号（若不在任何子图内，会报错）
	c.输入上述查询得到的节点信息（格式如：G1:5  G3:8）