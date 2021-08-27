/*
* FileName:   Maze.cpp
* Author:     李林飞
* StuNumber:  1951976
* Date:       2020.12.06
*/

#include<iostream>
using namespace std;

/*====================================== 栈的模板类 ==============================================*/
// 结点类
template <class Type>
struct Node
{
	Type data;
	Node<Type> * link = NULL;
	Node<Type>(Type x) : data(x) {}
	Node<Type>(Type x, Node<Type>* next = NULL) : data(x), link(next){}
};

// 栈类
template <class Type>
class Stack
{
private:
	Node<Type> * top;                 // 栈顶指针

public:
	Stack():top(NULL) {};             //  构造函数
	~Stack();                         // 析构函数
	void Push(const Type & x);        // 进栈
	Type Pop();                       // 出栈
	Type Top() const;                 // 返回栈顶元素
	bool Empty() const { return top == NULL ? true : false; }  // 判空
	int Size() const;                 // 返回栈中元素个数
};

// 析构函数 —— 销毁栈
template<class Type>
Stack<Type>::~Stack()
{
	Node<Type>* q;
	while (top)
	{
		q = top;
		top = top->link;
		delete q;
	}
}

// 进栈
template<class Type>
void Stack<Type>::Push(const Type& x)
{
	top = new Node<Type>(x, top);
	if (top == NULL) {
		cout << "空间分配失败!"; exit(1);
	}
}

// 出栈
template<class Type>
Type Stack<Type>::Pop()
{
	Type x;
	Node<Type>* q = top;
	top = top->link;
	x = q->data;
	delete q;
	return x;
}

// 取栈顶元素
template<class Type>
Type Stack<Type>::Top() const
{
	return top->data;
}

// 计算栈的大小
template<class Type>
int Stack<Type>::Size() const
{
	Node<Type>* p = top;
	int length = 0;
	while(p)
	{
		length++;
		p = p->link;
	}
	return length;
}


/*============================== 坐标类 =========================================*/
struct Point
{
	int row = -1;  // 默认为-1
	int col = -1;  // 默认为-1

	Point(const Point & P)
	{
		row = P.row;
		col = P.col;
	}
	Point(int x = -1, int y = -1)
	{
		row = x;
		col = y;
	}
};

ostream& operator<<(ostream& os, Point& p)
{
	os << "(" << p.row << "," << p.col << ")";
	return os;
}

/*================================== 迷宫类 ======================================*/
class Maze
{
private:
	int Row;                             // 迷宫行数
	int Col;                             // 迷宫列数
	int ** maze = NULL;                  // 迷宫数组
	Point startP;                        // 迷宫起始坐标
	Point endP;                          // 终止坐标
	bool ** mark = NULL;                 // 访问标记数组：未访问置为0，访问过置为1  
	Point VisitAroundNode(Point point);   //获取相邻未被访问的节点
	Stack<Point> pointStack;              // 存储路径

public:
	Maze(int row, int col);               // 构造函数
	~Maze();                              // 析构函数
	void SetStartP(Point p) { startP.row = p.row; startP.col = p.col; } // 设置迷宫入口
	void SetEndP(Point p) { endP.row = p.row; endP.col = p.col; }       // 设置迷宫出口
	void InitMaze(void* array);           // 初始化路径
    void FindPath();                      // 路径搜索
	void PrintPath();                     // 输出路径
};

// 构造函数 —— 动态申请数组空间
Maze::Maze(int row, int col)
{
	Row = row;
	Col = col;

	// 动态申请迷宫数组
	maze = new int* [Row];   
	for (int i = 0; i < Row; i++)
		maze[i] = new int[Col];

	// 动态申请标记数组空间
	mark = new bool* [Row];
	for (int i = 0; i < Row; i++)
		mark[i] = new bool[Col];
}

// 析构函数 —— 释放空间
Maze::~Maze()
{
	for (int i = 0; i < Row; i++)
		delete maze[i];
	delete[] maze;

	for (int i = 0; i < Row; i++)
		delete mark[i];
	delete[] mark;
}

// 初始化迷宫数组
void Maze::InitMaze(void* array)
{
	//将给定的任意列数的二维数组还原为指针数组，以支持下标操作
	int** maze2d = new int* [Row];
	for (int i = 0; i < Row; ++i)
	{
		maze2d[i] = (int*)array + i * Col;
	}

	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			maze[i][j] = maze2d[i][j];     // 初始化迷宫数组
			mark[i][j] = maze2d[i][j];     // 初始化标记数组，墙壁1默认为已经访问，通路0默认未被访问
		}
	}
}

// 访问当前结点的四周结点
Point Maze::VisitAroundNode(Point point)
{
	Point resP(-1,-1);
	if (point.row - 1 >= 0 && mark[point.row - 1][point.col] == false) {//上节点满足条件
		resP.row = point.row - 1;
		resP.col = point.col;
		return resP;
	}
	if (point.col + 1 < Col && mark[point.row][point.col + 1] == false) {//右节点满足条件
		resP.row = point.row;
		resP.col = point.col + 1;
		return resP;
	}
	if (point.row + 1 < Row && mark[point.row + 1][point.col] == false) {//下节点满足条件
		resP.row = point.row + 1;
		resP.col = point.col;
		return resP;
	}
	if (point.col - 1 >= 0 && mark[point.row][point.col - 1] == false) {//左节点满足条件
		resP.row = point.row;
		resP.col = point.col - 1;
		return resP;
	}
	return resP;
}


// 路径搜索 —— DFS算法
void Maze::FindPath()
{

	//起点和终点必须为无障碍结点，否则输入错误
	if (maze[startP.row][startP.col] == 1 || maze[endP.row][endP.col] == 1)
	{
		cout << "迷宫起始点或终点输入错误！\n";
		return;
	}

	//将起点入栈，并标记为访问
	pointStack.Push(startP);
	mark[startP.row][startP.col] = true;

	//栈不空并且栈顶元素不为结束节点
	while (pointStack.Empty() == false && (pointStack.Top().row != endP.row || pointStack.Top().col != endP.col))
	{
		Point NotVisitNode = VisitAroundNode(pointStack.Top());
		if (NotVisitNode.row == -1) 
		{     //没有未被访问的相邻节点
			pointStack.Pop();             //回溯到上一个节点
			continue;
		}

		//入栈并设置访问标志为true
		pointStack.Push(NotVisitNode);
		mark[NotVisitNode.row][NotVisitNode.col] = true;
	}
}

// 打印路径
void Maze::PrintPath()
{
	if (pointStack.Empty() == true)
	{
		cout << "无有效路径！\n" << endl;
	}
	else
	{
		Stack<Point> tmpStack;
		cout << "\n迷宫路径:\n";
		while (pointStack.Empty() == false)
		{
			tmpStack.Push(pointStack.Top());
			pointStack.Pop();
		}

		// 输出格式控制 
		if (tmpStack.Empty() == false) {
			cout << "(" << tmpStack.Top().row << "," << tmpStack.Top().col << ")";
			tmpStack.Pop();
		}
		while (tmpStack.Empty() == false)
		{
			cout  << " --> " << "(" << tmpStack.Top().row << "," << tmpStack.Top().col << ")";
			tmpStack.Pop();
		} 
	}
	cout << endl  << endl << endl;
}

/*=================================== 主函数 =====================================================*/
int main()
{
	int Row, Col; // 行列数
	Point startP; // 起点
	Point endP;   // 终点


	/* === 小型迷宫 === */
	// 迷宫地图输出
	int smallmaze[5][5] = {
			{0,0,0,0,0},
			{0,1,0,1,0},
			{0,1,1,0,0},
			{0,1,1,0,1},
			{0,0,0,0,0}
	};
	cout << "小迷宫地图：\n";
	for (int i = 0; i < 5; i++)
		cout << "   " << i << "列";
	cout << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << i << "行" << " ";
		for (int j = 0; j < 5; j++)
		{
			cout << smallmaze[i][j] << "     ";
			if (j == 4) cout << endl << endl;
		}
	}

	
	cout << "请据迷宫输入行数、列数： ";
	cin >> Row >> Col;
	while(Row != 5 || Col != 5)
	{
		cout << "行列数输入错误，请重新输入：";
		cin >> Row >> Col;
	}

	cout << "请输入起点(先行后列)： ";
	cin >> startP.row >> startP.col;
	cout << "请输入终点(先行后列)： ";
	cin >> endP.row >> endP.col;

	// 初始化迷宫
	Maze map1(Row, Col);       // 迷宫定义
	map1.InitMaze(smallmaze);   // 初始化迷宫数组
	map1.SetStartP(startP);  // 设置迷宫入口
	map1.SetEndP(endP);    // 设置迷宫出口
	map1.FindPath();       // 路径搜索
	map1.PrintPath();      // 打印路径




	/* ==== 大型迷宫 ==== */
	// 迷宫地图输出
	int bigmaze[10][10] = {
			{0,0,0,0,0,1,0,0,0,0},
			{1,1,0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,1,0,1,0},
			{0,1,1,1,1,0,1,0,0,0},
			{0,0,0,0,0,1,1,1,0,1},
			{0,0,0,0,0,1,0,0,0,0},
			{1,1,0,1,0,0,0,1,1,1},
			{0,0,0,0,0,1,1,0,1,0},
			{0,0,1,1,1,0,0,0,1,0},
			{1,0,0,0,0,0,1,0,0,0},
	};
	cout << "大迷宫地图：\n";
	for (int i = 0; i < 10; i++)
		cout << "   " << i << "列";
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << i << "行" << " ";
		for (int j = 0; j < 10; j++)
		{
			cout << bigmaze[i][j] << "     ";
			if (j == 9) cout << endl << endl;
		}
	}


	cout << "请据迷宫输入行数、列数： ";
	cin >> Row >> Col;
	while (Row != 10 || Col != 10)
	{
		cout << "行列数输入错误，请重新输入：";
		cin >> Row >> Col;
	}

	cout << "请输入起点(先行后列)： ";
	cin >> startP.row >> startP.col;
	cout << "请输入终点(先行后列)： ";
	cin >> endP.row >> endP.col;

	// 初始化迷宫
	Maze map2(Row, Col);       // 迷宫定义
	map2.InitMaze(bigmaze);   // 初始化迷宫数组
	map2.SetStartP(startP);  // 设置迷宫入口
	map2.SetEndP(endP);    // 设置迷宫出口
	map2.FindPath();       // 路径搜索
	map2.PrintPath();      // 打印路径
	

	/* === 入口出口输入错误测试 ===*/
	// 迷宫地图输出
	int maze[10][10] = {
			{1,0,0,0,0,1,0,0,0,0},
			{1,1,0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,1,0,1,0},
			{0,1,1,1,1,0,1,0,0,0},
			{0,0,0,0,0,1,1,1,0,1},
			{0,0,0,0,0,1,0,0,0,0},
			{1,1,0,1,0,0,0,1,1,1},
			{0,0,0,0,0,1,1,0,1,0},
			{0,0,1,1,1,0,0,0,1,0},
			{1,0,0,0,0,0,1,0,0,0},
	};
	cout << "入口出口输入错误测试：\n";
	cout << "大迷宫地图：\n";
	for (int i = 0; i < 10; i++)
		cout << "   " << i << "列";
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << i << "行" << " ";
		for (int j = 0; j < 10; j++)
		{
			cout << maze[i][j] << "     ";
			if (j == 9) cout << endl << endl;
		}
	}


	cout << "请据迷宫输入行数、列数： ";
	cin >> Row >> Col;
	while (Row != 10 || Col != 10)
	{
		cout << "行列数输入错误，请重新输入：";
		cin >> Row >> Col;
	}

	cout << "请输入起点(先行后列)： ";
	cin >> startP.row >> startP.col;
	cout << "请输入终点(先行后列)： ";
	cin >> endP.row >> endP.col;
	// 初始化迷宫
	Maze map3(Row, Col);       // 迷宫定义
	map3.InitMaze(maze);   // 初始化迷宫数组
	map3.SetStartP(startP);  // 设置迷宫入口
	map3.SetEndP(endP);    // 设置迷宫出口
	map3.FindPath();       // 路径搜索
	map3.PrintPath();      // 打印路径


	/* === 无有效路径测试 ==== */
	// 迷宫地图输出
	int errormaze[10][10] = {
			{0,0,0,0,0,1,0,0,0,0},
			{1,1,0,1,0,0,0,1,0,0},
			{0,1,1,0,0,1,1,0,1,0},
			{0,1,1,1,1,0,1,0,0,0},
			{0,0,0,0,1,1,1,1,0,1},
			{0,0,0,0,0,1,0,0,0,0},
			{1,1,0,1,1,0,1,1,1,1},
			{0,0,0,0,0,1,1,1,1,0},
			{0,0,1,1,1,0,0,0,1,0},
			{1,0,0,0,0,0,1,0,0,0},
	};
	cout << " 无有效路径测试：\n";
	cout << "无路径迷宫地图：\n";
	for (int i = 0; i < 10; i++)
		cout << "   " << i << "列";
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << i << "行" << " ";
		for (int j = 0; j < 10; j++)
		{
			cout << errormaze[i][j] << "     ";
			if (j == 9) cout << endl << endl;
		}
	}


	cout << "请据迷宫输入行数、列数： ";
	cin >> Row >> Col;
	while (Row != 10 || Col != 10)
	{
		cout << "行列数输入错误，请重新输入：";
		cin >> Row >> Col;
	}

	cout << "请输入起点(先行后列)： ";
	cin >> startP.row >> startP.col;
	cout << "请输入终点(先行后列)： ";
	cin >> endP.row >> endP.col;

	// 初始化迷宫
	Maze map4(Row, Col);        // 迷宫定义
	map4.InitMaze(errormaze);   // 初始化迷宫数组
	map4.SetStartP(startP);    // 设置迷宫入口
	map4.SetEndP(endP);        // 设置迷宫出口
	map4.FindPath();           // 路径搜索
	map4.PrintPath();          // 打印路径

	system("pause"); // 为了生成的.exe文件查看结果
	return 0;

}
