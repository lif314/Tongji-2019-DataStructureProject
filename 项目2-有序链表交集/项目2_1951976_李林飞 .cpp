/*
* FileName: OrderedList.cpp
* Function: 求两个有序链表的交集、并集、差集
* Author: 李林飞
* StuNumber: 1951976
* Date: 2020.12.08
*/


#include <iostream>
using namespace std;

/*================================ 结点类 ==================================*/
struct ListNode
{
		int data;
		ListNode * link = NULL;
		ListNode():link(NULL){}        // 构造函数
		ListNode(const int & d, ListNode *l = NULL) : data(d), link(l){}  // 构造函数
};


/*=============================== 链表类声明 ==============================*/
class OrderedList
{
private:
	ListNode * first;       // 头节点标识有序链表
	ListNode *rear;         // 尾指针     
public:
	OrderedList();          // 构造函数
	~OrderedList();         // 析构函数

	// 重载函数：+ :求并集  * :求交集  - :求差集  = ：赋值 
	OrderedList & operator=(OrderedList & list1);
	void Create();          // 创建有序链表
	void Print();           // 打印有序链表
	friend OrderedList & operator+(OrderedList & list1, OrderedList & list2);  
	friend OrderedList & operator*(OrderedList & list1, OrderedList & list2);
	friend OrderedList & operator-(OrderedList & list1, OrderedList & list2);
};




/*================================ 主函数 ======================================*/
void Interface();     // 初始化界面函数声明
int main()
{
	Interface();
	char tag;    // 测试标志
	do
	{
		OrderedList S1, S2, S3, S4, S5;
		cout << "请输入非降序序列S1（以-1标志结尾）：\n";
		S1.Create();
		cout << "请输入非降序序列S2（以-1标志结尾）：\n";
		S2.Create();
		S3 = S1 + S2;
		cout << "两链表的并集S3：\n";
		S3.Print();
		S4 = S1 * S2;
		cout << "两链表的交集S4：\n";
		S4.Print();
		cout << "两链表的差集S5：\n";
		S5 = S1 - S2;
		S5.Print();

		cout << "\n继续测试？(y or n): ";
		cin >> tag;
	} while (tag == 'y');
	return 0;
}


/*=========================== 初始化界面函数 ===================================*/
void Interface()
{
	cout << "***************************************\n";   //标语
	cout << "**                                   **\n";
	cout << "**         非降序链表操作            **\n";
	cout << "**           支持功能：              **\n";
	cout << "**              +：求并集            **\n";
	cout << "**              -：求差集            **\n";
	cout << "**              *：求交集            **\n";
	cout << "**                                   **\n";
	cout << "***************************************\n\n";
}


/*=============================== 链表类操作函数 =================================*/
// 构造函数
OrderedList::OrderedList()
{
	first = rear = new ListNode;
	first->link = rear->link = NULL;
}

// 析构函数
OrderedList::~OrderedList()
{
	ListNode * q = nullptr;
	while (first->link)
	{
		q = first->link;
		first->link = q->link;
		delete q;
	}
	delete first;
}

// 重载运算符 = —— 赋值操作
OrderedList & OrderedList::operator=(OrderedList & list1)
{
	ListNode* p1 = list1.first->link;   // list1扫描指针
	ListNode* p = first = new ListNode; // 目标链表指针
	while (p1)
	{
		p->link = new ListNode(p1->data);
		p = p->link;
		p1 = p1->link;
	}
	p->link = NULL;
	rear = p;
	return *this;
}

// 创建有序链表
void OrderedList::Create()
{
	int x;     // 当x=-1时表示结束
	ListNode* newNode;
	first->link = rear;
	while (1)
	{
		cin >> x;
		if (x != -1)
		{
			newNode = new ListNode(x);
			rear->link = newNode;
			rear = newNode;
		}
		else
		{
			rear->link = NULL;
			break;
		}
	}
}

// 输出有序链表
void OrderedList::Print()
{
	ListNode* p = first->link;
	if (p == NULL) 
		cout << "NULL";
	else
		while (p)
		{
			cout << p->data << " ";
			p = p->link;
		}
	cout << endl;
}

// 重载运算符 + —— 求并集
OrderedList & operator+(OrderedList& list1, OrderedList& list2)
{
	if (list1.first->link == NULL)
		return list2;
	if (list2.first->link == NULL)
		return list1;

	ListNode* p1 = list1.first->link;          // list1有序链表的扫描指针
	ListNode* p2 = list2.first->link;          // list2有序链表的扫描指针
	static OrderedList ans;
	ListNode* p = ans.first;                  // 结果链表的扫描指针
	while (p1 && p2)                          // 两个有序链表同时扫描，小的加入新的链表中
	{
		if (p1->data == p2->data)
		{
			p->link = new ListNode(p1->data);
			p1 = p1->link;
			p2 = p2->link;
		}
		else if (p1->data < p2->data)
		{
			p->link = new ListNode(p1->data);
			p1 = p1->link;
		}
		else
		{
			p->link = new ListNode(p2->data);
			p2 = p2->link;
		}
		p = p->link;
	}

	// 处理未扫描到的链表
	ListNode* pr = p1 != NULL ? p1 : p2;  // pr指向有剩余的链表
	while (pr)
	{
		p->link = new ListNode(pr->data);
		pr = pr->link;
		p = p->link;
	}
	p->link = NULL;    // 尾指针处理
	ans.rear = p;
	return ans;
}

// 重载运算符 * —— 求交集
OrderedList& operator*(OrderedList& list1, OrderedList& list2)
{
	if (list1.first->link == NULL || list2.first->link == NULL)
		return list1;

	ListNode* p1 = list1.first->link;          // list1有序链表的扫描指针
	ListNode* p2 = list2.first->link;          // list2有序链表的扫描指针
	static OrderedList ans;
	ListNode* p = ans.first;                  // 结果链表的扫描指针
	while (p1 && p2)                          // 两个有序链表同时扫描，小的加入新的链表中
	{
		if (p1->data == p2->data)
		{
			p->link = new ListNode(p1->data);
			p = p->link;
			p1 = p1->link;
			p2 = p2->link;
		}
		else if (p1->data < p2->data)
			p1 = p1->link;
		else
			p2 = p2->link;
	}
	
	p->link = NULL;    // 尾指针处理
	ans.rear = p;
	return ans;
}

// 重载运算符 - —— 求差集
OrderedList& operator-(OrderedList& list1, OrderedList& list2)
{
	if (list1.first->link == NULL)
		return list1;
	if (list2.first->link == NULL)
		return list1;

	ListNode* p1 = list1.first->link;          // list1有序链表的扫描指针
	ListNode* p2 = list2.first->link;          // list2有序链表的扫描指针
	static OrderedList ans;
	ListNode* p = ans.first;                  // 结果链表的扫描指针
	while (p1 && p2)                          // 两个有序链表同时扫描，小的加入新的链表中
	{
		if (p1->data == p2->data)
		{
			p1 = p1->link;
			p2 = p2->link;
		}
		else if (p1->data < p2->data)
		{
			p->link = new ListNode(p1->data);
			p = p->link;
			p1 = p1->link;
		}
		else
			p2 = p2->link;
	}
	// 处理未扫描到的链表
	while (p1)
	{
		p->link = new ListNode(p1->data);
		p1 = p1->link;
		p = p->link;
	}
	p->link = NULL;    // 尾指针处理
	ans.rear = p;
	return ans;
}
