/*
* FileName: MathOperator.cpp
* Function: Evaluate the calculation expression (+,-,*,/,^,%)
* Author: 李林飞
* StuNumber: 1951976
* Date: 2020.12.9
*/


// 优先级表
/***************************** Precedure of operator *******************************************/
/*                                   Precedure                                                 */
/*    Symbol                  Input Precedure               Stack Precedure             Rank   */
/*     ^ %                                                                                     */
/*     + -                                                                                     */
/*    + - (binary)                    1                            1                      -1   */
/*    / *                             2                            2                      -1   */
/*    (                               3                            -1                      0   */
/*    )                               0                             0                      0   */


#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;


// 报错类型
#define OperatorExpected 0
#define OperandExpected  1
#define MissingLeftParenthesis 2
#define MissingRightParenthesis 3
#define InvalidInput 4
#define leftparenthesis '('
#define rightparenthesis ')'



/*================================= 栈模板类 =====================================*/
// stack node
template <class Type>
struct Node
{
    Type data;
    Node<Type>* link = NULL;
    Node<Type>(Type x) : data(x) {}
    Node<Type>(Type x, Node<Type>* next = NULL) : data(x), link(next) {}
};

// template satck
template <class Type>
class Stack
{
private:
    Node<Type>* top;           // 栈顶指针

public:
    Stack() :top(NULL) {};    // 构造函数
    ~Stack();                 // 析构函数

    void Push(const Type& x); // 进栈   
    Type Pop();               // 出栈
    Type Top() const;         // 返回栈顶元素
    // 判空
    bool Empty() const { return top == NULL ? true : false; }  
    int Size() const;         // 长度
};

// 析构函数 —— 释放空间
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

// 出栈
template<class Type>
void Stack<Type>::Push(const Type& x)
{
    top = new Node<Type>(x, top);
    if (top == NULL) 
    {
        cout << "The allocation of space failed!" << endl;
        exit(1);
    }
}

// 进栈
template<class Type>
Type Stack<Type>::Pop()
{
    if (top == NULL)
    {
        cerr << "Attempt to pop an empty stack!" << endl;
        exit(1);
    }

    // 记录栈顶元素值
    Type x = top->data;

    // 删除栈顶
    Node<Type>* q = top;
    top = top->link;
    delete q;
    return x;
}


// 返回栈顶元素
template<class Type>
Type Stack<Type>::Top() const
{
    return top->data;
}

// 返回栈长度
template<class Type>
int Stack<Type>::Size() const
{
    Node<Type>* p = top;
    int length = 0;
    while (p)
    {
        length++;
        p = p->link;
    }
    return length;
}



/*===================================== 运算类 ===================================*/
class MathOperator 
{
private:
    char op;                   // 运算符
    int inputprecedence;       // 出栈优先级
    int stackprecedence;       // 入栈优先级

public:
    // 构造、析构函数
    MathOperator() {}
    MathOperator(char ch);
    ~MathOperator() {}

    // 重载 >=
    int operator>= (MathOperator a) const;
    // 计算
    void Evaluate(Stack<float> & operandStack);
    // 取运算符
    char GetOp();
};


// 构造函数 —— 运算符赋值和优先级初始化
MathOperator::MathOperator(char ch) 
{
    op = ch; 
    switch (op)
    {
        // '+' and '-' have input/stack preceedence 1
    case '+':
    case '-':   inputprecedence = 1;
                stackprecedence = 1;
                break;

        // '*' and '/' have input/stack precedence 2
    case '^':
    case '%':
    case '*':
    case '/':   inputprecedence = 2;
                stackprecedence = 2;
                break;

        // '(' has input precedence 3, stack precedence -1
    case '(':   inputprecedence = 3;
                stackprecedence = -1;
                break;

        // ')' has input/stack precedence 0
    case ')':   inputprecedence = 0;
                stackprecedence = 0;
                break;
    }
}

// 重载 >=
int MathOperator::operator>= (MathOperator a) const
{
    return stackprecedence >= a.inputprecedence;
}


// 计算 —— 取栈顶两元素根据操作符计算，结果入栈
void MathOperator::Evaluate(Stack<float>& operandStack)
{
    float operand1 = operandStack.Pop(); // 右操作数
    float operand2 = operandStack.Pop(); // 左操作数

    // 据运算符进行相应的计算
    switch (op) 
    {
    case '+':   operandStack.Push(operand2 + operand1);
                break;

    case '-':   operandStack.Push(operand2 - operand1);
                break;

    case '*':   operandStack.Push(operand2 * operand1);
                break;

    case '/':   operandStack.Push(operand2 / operand1);
                break;

    case '%':   operandStack.Push(int(operand2) % int(operand1));
                break;

    case '^':   operandStack.Push(pow(operand2, operand1));
    }
}

// 返回运算符
char MathOperator::GetOp()
{
    return op;
}


/****************************************** Main Function***********************************************/
// 辅助函数
// 判断输入符号是数还是运算符
int isoperator(char ch) 
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == '^' || ch == '%')
        return 1;
    else
        return 0;
}

// 判断空格
int iswhitespace(char ch)
{
    if (ch == ' ' || ch == '\t' || ch == '\n')
        return 1;
    else
        return 0;
}

// 错误提示函数 
void Error(int n)
{
    static char errormsgs[5][30] = {
                    "Operator expected",
                    "Operand expected",
                    "Missing left parenthesis",
                    "Missing right parenthsis",
                    "Invalid input"
    };
    cerr << errormsgs[n] << endl;
    exit(1);
}

// 初始化界面函数
void Interface()
{
    cout << "**************************************************\n";
    cout << "**                                              **\n";
    cout << "**                                              **\n";
    cout << "**                算术运算系统                  **\n";
    cout << "**                                              **\n";
    cout << "**                                              **\n";
    cout << "**                   支持运算符：+ - * / . ^ %  **\n";
    cout << "**                   支持负数、小数的运算       **\n";
    cout << "**                   请勿输入中文括号！！！     **\n";
    cout << "**                                              **\n";
    cout << "**                                              **\n";
    cout << "**************************************************\n\n";
}


/*====================================== 主函数 ====================================*/
int main()
{
    Interface();        // 初始化界面
   
    while (1)
    {
        cout << "\n请输入表达式( = 表示结束):\n ";
        // 运算符栈
        Stack<MathOperator> OperatorStack;

        // 操作数栈
        Stack<float> OperandStack;

        // 操作符
        MathOperator opr1, opr2;

        int rank = 0;   // 读取表达式优先级，若操作数与运算符匹配，则最终应为1
        float number;   // 输入操作数
        char ch;        // 依次读入表达式

        // 负数处理辅助量
        int Flag = 0;   // 标志符号，处理负数
        int Num_Add_Sub = 0;      // 读取 + - 个数，判断第一个操作数是否为负数
        int Num_operand = 0;      // 读取数字个数


        // 当读入 = 号时计算结果
        while (cin.get(ch) && ch != '=')
        {
            // +-运算符计数
            if (ch == '+' || ch == '-')
                Num_Add_Sub++;

            // 小数点处理
            if (isdigit(ch) || ch == '.')
            {
                Flag = 0;
                // 如果时小数，退回输入
                cin.putback(ch);
                cin >> number;
                Num_operand++;

                // 操作数优先级为1
                rank++;
                if (rank > 1)
                    Error(OperatorExpected);
                // 操作数入栈
                OperandStack.Push(number);
            }

            // 运算符处理
            else if (isoperator(ch))
            {
                if (ch != '(' && ch != ')' && ch != '=')
                    Flag++;

                // 处理-、+、（符号开头的情况
                // 第一种情况：操作数个数为0时出现 + 或 -，则该数为正数或负数
                // 第二种情况：连续出现两次-+运算符，则后者为正数或负数
                if ((Num_operand == 0 && Num_Add_Sub == 1) || (Flag == 2 && ch == '-') || (Flag == 2 && ch == '+'))
                {
                    cin.putback(ch);
                    cin >> number;

                    // 操作数个数加1
                    Num_operand++;
                    // 操作数优先级为1
                    rank++;
                    if (rank > 1)
                        Error(OperatorExpected);
                    // 操作数入栈
                    OperandStack.Push(number);
                    Flag = 0;
                }

                else
                {
                    // （ 优先级为0
                    if (ch != '(')
                        rank--;
                    if (rank < 0)
                        Error(OperandExpected);


                    // 据优先级判断运算符栈的进栈与出栈
                    opr1 = MathOperator(ch);
                    while (!OperatorStack.Empty() && (opr2 = OperatorStack.Top()) >= opr1)
                    {
                        opr2 = OperatorStack.Pop();
                        opr2.Evaluate(OperandStack);
                    }
                    OperatorStack.Push(opr1);
                }
            }

            // 右括号处理
            else if (ch == rightparenthesis)
            {
                opr1 = MathOperator(ch);
                while (!OperatorStack.Empty() && (opr2 = OperatorStack.Top()) >= opr1) {
                    opr2 = OperatorStack.Pop();
                    opr2.Evaluate(OperandStack);
                }
                if (OperatorStack.Empty())
                    Error(MissingLeftParenthesis);
                opr2 = OperatorStack.Pop(); // 左括号出栈
            }

            // 输入错误处理
            else if (!iswhitespace(ch))
                Error(InvalidInput);
        }



        // 表达式读取结束式优先级应为1，否则操作出匹配错误
        if (rank != 1)
            Error(OperandExpected);

        // 判断运算符栈与操作数栈是否为空，并显示相应的错误
        while (!OperatorStack.Empty())
        {
            opr1 = OperatorStack.Pop();
            if (opr1.GetOp() == leftparenthesis)
                Error(MissingRightParenthesis);
            opr1.Evaluate(OperandStack);
        }

        // 输出结果
        cout << "The value is " << OperandStack.Pop() << endl;

        // 结束运行判断
        cout << "\n任意空格键继续，输入 n 退出： ";
        if (getchar() == 'n')
        {
            break;
        }
    }

    system("pause"); // 方便测试时.exe文件显示错误信息
    return  0;
 }