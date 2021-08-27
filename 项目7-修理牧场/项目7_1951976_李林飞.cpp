/*
* FileName: OptimalBinaryTree.cpp
* Function: 构建最优二元树，输出最佳前缀编码
* Author: 李林飞
* StuNumber: 1951976
* Date: 2020.12.07
*/

#include <iostream>
using namespace std;

/******* Huffman结点类 ******/
struct HuffmanNode
{
	int weight;                         // 存储权值
	HuffmanNode* leftChild;            // 左孩子
	HuffmanNode* rightChild;           // 右孩子
	HuffmanNode* parent;               // 父结点
};


/****** 哈夫曼树最小堆 ******/
class MinHeap
{
private:
	const int DefaultSize = 100;         // 最大容量
	HuffmanNode* heap;                  // 动态数组存储最小堆
	int CurrentSize;                     // 当前结点数
	void ShiftUp(int start);             // 向上调整
	void ShiftDown(int start, int m);    // 向下调整
public:
	MinHeap();                           // 构造函数
	~MinHeap();                          // 析构函数
	void Insert(HuffmanNode* current);  // 插入
	HuffmanNode* getMin();              // 获取最小结点
};

// 构造函数
MinHeap::MinHeap()
{
	heap = new HuffmanNode[DefaultSize]; // 创建堆空间
	CurrentSize = 0;
}

// 析构函数
MinHeap::~MinHeap()
{
	delete[] heap; // 释放空间
}

// 插入
void MinHeap::Insert(HuffmanNode* current)
{
	if (CurrentSize == DefaultSize)
	{
		cout << "堆已满" << endl;
		return;
	}
	// 把current的数据复制到“数组末尾”
	heap[CurrentSize] = *current;
	// 向上调整堆
	ShiftUp(CurrentSize);
	CurrentSize++;
}

// 获取最小结点并在堆中删除该结点
HuffmanNode* MinHeap::getMin()
{
	if (CurrentSize == 0)
		return NULL;

	HuffmanNode* newNode = new HuffmanNode();
	if (newNode == NULL)
	{
		cerr << "存储空间分配失败！" << endl;
		exit(1);
	}

	*newNode = heap[0];              // 将最小结点的数据复制给newNode
	heap[0] = heap[CurrentSize - 1]; // 用最后一个元素填补
	CurrentSize--;
	ShiftDown(0, CurrentSize - 1);   // 从0位置开始向下调整
	return newNode;
}

// 向上调整
void MinHeap::ShiftUp(int start) {
	// 从start开始，直到0或者当前值大于双亲结点的值时，调整堆
	int j = start, i = (j - 1) / 2; // i是j的双亲

	HuffmanNode temp = heap[j];
	while (j > 0) 
	{
		if (heap[i].weight <= temp.weight)
			break;
		else 
		{
			heap[j] = heap[i];
			j = i;
			i = (i - 1) / 2;
		}
	}
	heap[j] = temp;
}

// 向下调整
void MinHeap::ShiftDown(int start, int m) {
	int i = start, j = 2 * i + 1;    // j是i的左子女

	HuffmanNode temp = heap[i];
	while (j <= m) {
		if (j < m && heap[j].weight > heap[j + 1].weight)
			j++;                     // 选两个子女中较小者
		if (temp.weight <= heap[j].weight)
			break;
		else {
			heap[i] = heap[j];
			i = j;
			j = 2 * j + 1;
		}
	}
	heap[i] = temp;
}


/****** 哈夫曼树 ******/
class HuffmanTree
{
private:
	HuffmanNode* root;                                  // 根结点
	int total;                                          // 花费量
	void Destroy(HuffmanNode* current);                 // 销毁哈夫曼树
	void PreOrder(HuffmanNode* current); // 前序遍历Huffman树
public:
	HuffmanTree() { root = new HuffmanNode; root = NULL; }            // 构造函数
	~HuffmanTree() { Destroy(root); }                    // 析构函数
	void Create(int w[], int n);                         // 创建哈夫曼树
	void Merge(HuffmanNode* first, HuffmanNode* second, HuffmanNode* parent); // 合并      
	int MiniCost();                                      // 计算最小花费，即非叶子结点的值之和
};

// 销毁哈夫曼树
void HuffmanTree::Destroy(HuffmanNode* current)
{
	if (current != NULL)      // 不为空
	{ 
		Destroy(current->leftChild);  // 递归销毁左子树
		Destroy(current->rightChild); // 递归销毁右子树
		delete current;               // 释放空间
		current = NULL;               // 释放后的指针赋值NULL后可以继续使用
	}
}

// 创建哈夫曼树
void HuffmanTree::Create(int w[], int n) {
	int i;
	MinHeap hp; // 使用最小堆存放森林
	HuffmanNode* first, * second, * parent = NULL;
	HuffmanNode* work = new HuffmanNode();

	if (work == NULL)
	{
		cerr << "存储空间分配失败！" << endl;
		exit(1);
	}

	for (i = 0; i < n; i++)
	{
		work->weight = w[i];
		work->leftChild = work->rightChild = work->parent = NULL;
		hp.Insert(work); // 插入到最小堆中
	}

	if (n == 1)
	{
		root = new HuffmanNode;
		root->weight = w[0];
		return;
	}
	
	for (i = 0; i < n - 1; i++)
	{ // 做n-1趟，形成Huffman树
		first = hp.getMin(); // 获取权值最小的树
		second = hp.getMin(); // 获取权值次小的树
		parent = new HuffmanNode();
		if (parent == NULL)
		{
			cerr << "存储空间分配失败！" << endl;
			exit(1);
		}
		Merge(first, second, parent); // 合并
		if(parent)
			hp.Insert(parent); // 重新插入到最小堆中
	}
	root = parent; // 根结点
}
// 合并
void HuffmanTree::Merge(HuffmanNode* first, HuffmanNode* second, HuffmanNode* parent)
{
	parent->leftChild = first; // 左子树
	parent->rightChild = second; // 右子树
	parent->weight = first->weight + second->weight; // 父结点权值
	first->parent = second->parent = parent; // 父指针
}

// 前序遍历计算最小花费
void HuffmanTree::PreOrder(HuffmanNode* current)
{
	if (current->leftChild == NULL && current->rightChild == NULL)
		return;
	else
		total += current->weight;

	PreOrder(current->leftChild);
	PreOrder(current->rightChild);

}

int HuffmanTree::MiniCost()
{
	PreOrder(root);
	return total;
}

/******* 主函数 ******/
int a[10005];
int main()
{
	cout << "******************************************\n";
	cout << "**                                      **\n";
	cout << "**              修理牧场系统            **\n";
	cout << "**                                      **\n";
	cout << "******************************************\n\n";

	char flag;
	do {
		cout << "请输入木头的块数(N <= 10^4)： ";
		int N;
		cin >> N;
		while (N <= 0)   // N <= 0
		{
			cout << "段数应为正整数，请重新输入： ";
			cin >> N;
		}

		cout << "请输入每块木头的长度： ";
		for (int i = 0; i < N; i++)
		{
			cin >> a[i];
			if (a[i] <= 0)
			{
				cout << "木块长度应为正整数，你已经没有机会了！\n";
				exit(1);
			}
		}
		
		int cost;
		if (N == 1)
			cost = 0;
		else
		{
			HuffmanTree tree;
			tree.Create(a, N);
			cost = tree.MiniCost();
		}
		cout << "最小花费为： " << cost << endl;

		cout << "\n继续测试？（y or another char）: ";
		cin >> flag;
	} while (flag == 'y');

	cout << "欢迎再次使用！" << endl;

	return 0;
}
