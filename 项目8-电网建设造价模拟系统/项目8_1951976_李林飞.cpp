/*
* FileName:  MinimunCostTree.cpp
* Function:  最小生成树，Prim算法求最短路径
* Author:    李林飞
* StuNumber: 1951976
* Date:      2020.12.14
*/

#include <iostream>
#include <string.h>

using namespace std;


/* 边结点 */
struct EdgeNode
{
	int weight;
	char start;
	char end;
	EdgeNode(){}        // 构造函数
	EdgeNode(char s, char e, int w)
	{
		start = s;
		end = e;
		weight = w;
	}
};


/* 图类 */
class Graph
{
private:
	int VerticesNum = 0;                     // 顶点数
	int EdgesNum = 0;                        // 边数
	int** EdgeMatrix = NULL;                 // 邻接矩阵，图的关系存储矩阵
	char * VerticesList = NULL;              // 顶点表：字符顶点与数组下标相对应  
	const int MaxWeight = INT_MAX;           // 整数最大值——代表无穷大
	EdgeNode *MiniSpanTree;                  // 最小生成树，存储边

	int IndexVertices(char nodeName);        // 通过结点名称寻找对应的下标
	void AddNode();                          // 创建电网顶点
	void AddEdge();                          // 添加电网的边
	void CreateMiniSpanningTree();           // 构造最小生成树
	void PrintTree();                        // 显示最小生成树
	void Destroy();                          // 退出程序，删除图

public:          
	void Switch(char choice);                // 选择操作函数
};

// 功能选择
void Graph::Switch(char choice)
{
	switch (choice)
	{
	case 'A': AddNode(); break;
	case 'B': AddEdge(); break;
	case 'C': CreateMiniSpanningTree(); break;
	case 'D': PrintTree(); break;
	case 'E': Destroy(); break;
	}
}

// 返回字符顶点在邻接矩阵中的下标
int Graph::IndexVertices(char nodeName)
{
	for (int i = 0; i < VerticesNum; i++)
		if (VerticesList[i] == nodeName)
			return i;
	return -1;
}


// 建立结点，为方便操作，讲结点字符与与数字对应
void Graph::AddNode()
{
	cout << "请输入顶点的个数： ";
	int nodeNum;
	cin >> nodeNum;

	while (nodeNum < 2)                 // 检测顶点个数的合法性
	{
		cout << "顶点数应是正整数且大于2，请重新输入： ";
		cin >> nodeNum;
	}

	VerticesNum = nodeNum;
	VerticesList = new char[VerticesNum + 3];
	cout << "请输入各顶点的名称：\n";
	for (int i = 0; i < VerticesNum; i++)
		cin >> VerticesList[i];              // 存储结点，建立下标与字符一一对应的关系
}

// 增添边
void Graph::AddEdge()
{
	if (VerticesNum == 0)
	{
		cout << "无顶点，创建边关系！请创建顶点：\n";
		Switch('A');
	}

	// 动态申请二维数组-- 邻接矩阵
	EdgeMatrix = new int* [VerticesNum];
	for (int i = 0; i < VerticesNum; i++)
		EdgeMatrix[i] = new int[VerticesNum];

	for (int i = 0; i < VerticesNum; i++)     // 邻接矩阵初始化
		for (int j = 0; j < VerticesNum; j++)
			EdgeMatrix[i][j] = (i == j) ? 0 : MaxWeight;

	char start, end;        // 边的起点和终点
	int weight;            // 边的权值
	int v1, v2;            // 邻接矩阵行列坐标
	while (1)
	{
		cout << "请输入两个顶点及边（结束请输入0 0 0）： ";
		cin >> start >> end >> weight;
		if (start == '?' || end == '?' || weight == 0)
			break;
		v1 = IndexVertices(start);
		v2 = IndexVertices(end);

		while (v1 == -1 || v2 == -1)             // 边合法性检测
		{
			if(v1 == -1)
				cout << "你输入的" << start << "结点不存在！\n";
			else if(v2 == -1)
				cout << "你输入的" << end << "结点不存在！\n";
			else if(v1 == -1 && v2 == -1)
				cout << "你输入的" << start << "、" << end << "结点不存在！\n";

			cout << "请重新输入两个顶点及边(结束请输入0 0 0)： ";
			cin >> start >> end >> weight;
			v1 = IndexVertices(start);
			v2 = IndexVertices(end);
		}

		if (EdgeMatrix[v1][v2] != MaxWeight && EdgeMatrix[v1][v2] > 0)
			cout << "你重复输入边关系，此边信息将被忽略！请继续输入：\n";

		if (v1 > -1 && v1 < VerticesNum && v2 > -1 && v2 < VerticesNum
			&& EdgeMatrix[v1][v2] == MaxWeight)           // 插入条件
		{
			EdgeMatrix[v1][v2] = EdgeMatrix[v2][v1] = weight;
			EdgesNum++;                                   // 边数加1
		}
	}
}

// 构造最小生成树
void Graph::CreateMiniSpanningTree()
{
	if (VerticesNum == 0)
	{
		cout << "无顶点和边关系，无法创建最小生成树！请创建顶点和边关系：\n";
		Switch('A');
		Switch('B');
	}
	else if (EdgeMatrix == NULL)
	{
		cout << "没有边关系，无法创建最小生成树！请添加边关系：\n";
		Switch('B');
	}

	int* lowcost = new int[VerticesNum];
	int* nearVex = new int[VerticesNum];

	
	
	cout << "请输入最小生成树的起始顶点： ";
	char tagStart;     // 起始点
	cin >> tagStart;
    
	while (IndexVertices(tagStart) == -1)     // 起始顶点测试
	{
		cout << "你输入的起始顶点不存在，请重新输入：";
		cin >> tagStart;
	}

	int startU = IndexVertices(tagStart);   // 起始顶点号
	lowcost[startU] = 0;
	for (int i = 0; i < VerticesNum; i++) 
	{
		if (i != startU)
		{
			lowcost[i] = EdgeMatrix[startU][i];  // 起始顶点到各边的代价
			nearVex[i] = startU;                 // 最短带权路径
		}
	}

	nearVex[startU] = -1;      // 顶点startU加到生成树顶点集合
	int count = 0;             // 计数
	MiniSpanTree = new EdgeNode[VerticesNum - 1];

	for(int i = 1; i < VerticesNum; i++)
	{
		int min = MaxWeight;
		int v = startU;
		for(int j = 0; j < VerticesNum; j++)
			if (nearVex[j] != -1 && lowcost[j] < min)
			{
				v = j;
				min = lowcost[j];
			}

		if (v != startU)
		{
			EdgeNode e(VerticesList[nearVex[v]], VerticesList[v], lowcost[v]);
			MiniSpanTree[count++] = e;    // 选出边加入生成树
			nearVex[v] = -1;    
			for(int k = 0; k < VerticesNum; k++)
				if (nearVex[k] != -1 && EdgeMatrix[v][k] < lowcost[k])
				{
					lowcost[k] = EdgeMatrix[v][k];
					nearVex[k] = v;
				}
		}
	}
	cout << "生成Prim最小生成树！\n";
}

// 打印最小生成树
void Graph::PrintTree()
{
	if (VerticesNum == 0)
	{
		cout << "无顶点和边关系，无内容可打印！请创建最小生成树：\n";
		Switch('A');
		Switch('B');
		Switch('C');
	}
	else if (EdgeMatrix == NULL)
	{
		cout << "没有边关系，无法创建最小生成树！ 请添加边关系：\n";
		Switch('B');
		Switch('C');
	}
	else if (MiniSpanTree == NULL)
	{
		cout << "未创建最小生成树，无内容可打印！请创建最小生成树：\n";
		Switch('C');
	}

	cout << "最小生成树的顶点及边为：\n";
	for (int i = 0; i < VerticesNum - 1; i++)
		cout << MiniSpanTree[i].start << " --<" 
			 << MiniSpanTree[i].weight << ">--> "
			 << MiniSpanTree[i].end << "     ";
	cout << endl;
	
}

// 释放空间
void Graph::Destroy()
{
	for (int i = 0; i < VerticesNum; i++)
		delete[] EdgeMatrix[i];
	delete[] EdgeMatrix;

	delete [] VerticesList;                 // 顶点表：字符顶点与数组下标相对应  
	delete [] MiniSpanTree;                 // 最小生成树
	exit(1);
}

// 初始化界面
void Interface()
{
	cout << "***********************************************\n";   
	cout << "**                                           **\n";
	cout << "**              电网造价模拟系统             **\n";
	cout << "**                                           **\n";
	cout << "***********************************************\n";
	cout << "**              A --- 创建电网顶点           **\n";
	cout << "**              B --- 添加电网的边           **\n";
	cout << "**              C --- 构造最小生成树         **\n";
	cout << "**              D --- 显示最小生成树         **\n";
	cout << "**              E --- 退出程序               **\n";
	cout << "**                                           **\n";
	cout << "***********************************************\n\n";
}


/*============================================= 主函数 ========================================*/
int main()
{
	Interface();
	char choice;
	Graph netCost;
	while (1)
	{
		cout << "\n请选择操作： ";
		cin >> choice;

		while (choice < 'A' || choice > 'E')     // 操作码检测
		{
			cout << "输入操作不存在！请重新输入： ";
			cin >> choice;
		}

		netCost.Switch(choice);
	}
	return 0;
}
