/*
* FileName:  BankBusiness.cpp  
* Author:    李林飞
* StuNumber: 1951976
* Date:      2020.12.13
*/

#include <iostream>
using namespace std;

/*
用数组就可以很好的实现，但为了练习，写一个队列
*/

/*========================================= 队列模板类 ============================================*/
/*==== 队列结点类 ===*/
template<class Type>
struct QueueNode
{
	Type data;
	QueueNode<Type>* link;
	QueueNode(QueueNode<Type>* next = NULL):link(next){}
	QueueNode(Type d, QueueNode<Type>* next = NULL):data(d),link(next){}
};

/*==== 队列模板类 ====*/
template<class Type>
class Queue
{
private:
	QueueNode<Type>* front;    // 队头
	QueueNode<Type>* rear;     // 队尾
public:
	Queue() :front(NULL), rear(NULL) {}  // 构造函数
	~Queue();                            // 析构函数
	Type Front();                        // 返回队首元素
	Type Back();                         // 返回队尾元素
	void Push(const Type & item);        // 入队
	void Pop();                          // 出队
	bool Empty();                        // 判空
	int Size();                          // 长度
};

// 析构函数
template<class Type>
Queue<Type>::~Queue()
{
	QueueNode<Type>* p;
	while (front)
	{
		p = front;
		front = front->link;
		delete p;
	}
}

// 返回队首元素
template<class Type>
Type Queue<Type>::Front()
{
	return front->data;
}

// 返回队尾元素
template<class Type>
Type Queue<Type>::Back()
{
	return rear->data;
}

// 入队
template<class Type>
void Queue<Type>::Push(const Type& item)
{
	if (front == NULL)  // 空队列
	{
		front = rear = new QueueNode<Type>(item);
		if (front == NULL) {
			cout << "空间分配失败！" << endl;
			exit(1);
		}
	}
	else  // 非空队列
	{
		rear->link = new QueueNode<Type>(item);
		if (rear->link == NULL) {
			cout << "空间分配失败！" << endl;
			exit(1);
		}
		rear = rear->link;    // 更新尾指针
	}
}

// 出队
template<class Type>
void Queue<Type>::Pop()
{
	if (Empty())
	{
		cerr << "Attempt to pop the empty queue!" << endl;
		exit(1);
	}
	QueueNode<Type>* q = front;
	front = front->link;
	delete q;
}

// 判空
template<class Type>
bool Queue<Type>::Empty()
{
	return front == NULL;
}

// 计算长度
template<class Type>
int Queue<Type>::Size()
{
	int length = 0;
	QueueNode<Type>* p = front;
	while (p)
	{
		length++;
		p = p->link;
	}
	return length;
}


/*========================================== 主函数 ==============================================*/
int main()
{
	cout << "********************************\n";
	cout << "**                            **\n";
	cout << "**         银行业务系统       **\n";
	cout << "**                            **\n";
	cout << "********************************\n\n";

	char tag;
	do
	{
		Queue<int> QEven, QOdd;
		// 入队
		cout << "顾客人数为： ";
		int n;
		cin >> n;
		while (n < 0)
		{
			cout << "输入错误，顾客人数应为正整数，请重新输入： ";
			cin >> n;
		}

		// 输入编号序列
		cout << "顾客编号为： ";
		int x;
		while (n--)
		{
			cin >> x;
			while (x < 0)
			{
				cout << "输入错误，顾客编号应为正整数，请重新输入： ";
				cin >> x;
			}

			if (x % 2)
				QOdd.Push(x);
			else
				QEven.Push(x);
		}

		// 出队
		if (QOdd.Empty())        // 序列中没有奇数
			while (!QEven.Empty())
			{
				cout << QEven.Front() << " ";
				QEven.Pop();
			}
		else if (QEven.Empty())  // 序列中没有偶数
			while (!QOdd.Empty())
			{
				cout << QOdd.Front() << " ";
				QOdd.Pop();
			}
		else                   // 序列中既有奇数也有偶数
		{
			int i;
			cout << "办理业务顺序为： \n";
			while (!QOdd.Empty())
			{
				int cnt = 2;
				i = 0;
				while (cnt-- && !QOdd.Empty())    //弹出两个奇数
				{
					if (i++) cout << " ";
					cout << QOdd.Front();
					QOdd.Pop();
				}
				if (!QEven.Empty())             //弹出一个偶数
				{
					cout << " " << QEven.Front() << " ";
					QEven.Pop();
				}
			}

			// 
			i = 0;
			while (!QEven.Empty())     //把多余的偶数弹出
			{
				if (i++) cout << " ";
				cout << QEven.Front();
				QEven.Pop();
			}

			i = 0;
			while (!QOdd.Empty())      //把多余的偶数弹出
			{
				if (i++) cout << " ";
				cout << QOdd.Front();
				QOdd.Pop();
			}
		}
		cout << endl << endl;
		cout << "继续测试？ (y or n): ";
		cin >> tag;
	} while (tag == 'y');

	return 0;
}
