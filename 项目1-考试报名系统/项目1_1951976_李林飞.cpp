/*
* Author:    李林飞
* StuNumber: 1951976
* Date:      2020.11.20
*/


#include <iostream>
#include <string>
#include<iomanip>

using namespace std;


/*=========================== 学生信息结构体 ==============================*/
struct StudentInfo
{
    string stuNumber;    // 学号
    string stuName;      // 姓名
    string stuGender;    // 性别
    string stuAge;       // 年龄
    string stuCategory;  // 类别
};

// 输入重载
istream & operator>>(istream & in, StudentInfo & stu)
{
    in >> stu.stuNumber >> stu.stuName >> stu.stuGender >> stu.stuAge >> stu.stuCategory;
    return in;
}

// 输出重载
ostream & operator<<(ostream & out, const StudentInfo & stu)
{
    out << setiosflags(ios::left)
        << setw(12) << stu.stuNumber
        << setw(12) << stu.stuName 
        << setw(12) << stu.stuGender
        << setw(12) << stu.stuAge
        << setw(12) << stu.stuCategory << endl;
    return out;
}


/*============================== 学生结点 =================================*/
struct StudentNode
{
    StudentInfo data;    // 学生信息
    StudentNode * link;  // 指针域
    StudentNode(StudentNode * next = NULL) : link(next) {}
    StudentNode(StudentInfo d, StudentNode * next = NULL) :data(d), link(next) {}
};

// 输出学生信息
ostream& operator<<(ostream& out, const StudentNode& stuNode)
{
    out << stuNode.data;
    return out;
}


/*====================== 考生报名系统类 —— 单链表实现 =========================*/
class StudentSystem
{
private:
    StudentNode * headNode;                 // 链表的头指针
    StudentNode * rearNode;                 // 尾指针
    StudentNode * Locate(int i);            // 定位函数，搜索第i个元素
    StudentNode * Locate(string stuNumber); // 定位函数，按学号搜索元素 
    void Init(const StudentInfo & stu);     // 初始化函数
public:
    StudentSystem();                        // 构造函数
    ~StudentSystem();                       // 析构函数
    int Length() ;                          // 统计报考学生人数
    void Interface();                       // 初始化界面
    void Create();                          // 建立考生信息系统
    void Search();                          // 通过考号查找并输出信息
    void Insert();                          // 按位置插入学生信息
    void Delete();                          // 通过考号删除学生信息
    void Update();                          // 通过考号修改学生信息(学号不能改)
    void Output();                          // 输出所有考生信息
    void Switch(int choice);                // 功能选择函数
};

// 构造函数
StudentSystem::StudentSystem()
{
    rearNode = headNode = new StudentNode;
}
void StudentSystem::Init(const StudentInfo& stu)
{
    StudentNode* newNode = new StudentNode(stu);
    rearNode->link = newNode;
    rearNode = newNode;
}

// 析构函数
StudentSystem::~StudentSystem()
{
    StudentNode * q;
    while (headNode->link)
    {
        q = headNode->link;
        headNode->link = q->link;
        delete q;
    }
    delete headNode;     // 释放头节点
}

// 初始化界面
void StudentSystem::Interface()
{
    cout << "***************************************\n";   //标语
    cout << "**                                   **\n";
    cout << "**         欢迎进入考生信息系统      **\n";
    cout << "**           支持功能：              **\n";
    cout << "**              0：退出              **\n";
    cout << "**              1：插入              **\n";
    cout << "**              2：删除              **\n";
    cout << "**              3：查找              **\n";
    cout << "**              4：修改              **\n";
    cout << "**              5：统计              **\n";
    cout << "**                                   **\n";
    cout << "***************************************\n\n";
    cout << "请建立考生信息系统！\n";
}

// 创建系统
void StudentSystem::Create()
{
    int n;
    StudentInfo stu;
    cout << "请输入考生人数： ";
    cin >> n;
    if (n < 1)
    {
        cout << "考生人数应为正整数，请重新输入： ";
        cin >> n;
        
    }
    cout << "请依次输入考生的考号、姓名、性别、年龄及报考类别！\n" << endl;
    for (int i = 1; i <= n; i++)
     {
        cin >> stu;
        Init(stu);
     }
}

// 定位函数，搜索第i个元素
StudentNode * StudentSystem::Locate(int i)
{
    if (i < 0 || i > Length())
        return NULL;
    StudentNode * current = headNode;
    int k = 0;
    while (current && k < i)
    {
        current = current->link;
        k++;
    }
    return current;
}

// 定位函数，按学号搜索元素 
StudentNode* StudentSystem::Locate(string stuNumber)
{
    StudentNode * current = headNode->link;
    while (current)
    {
        if (current->data.stuNumber == stuNumber)
        {
            return current;
            break;
        }
        current = current->link;
    }
    return NULL;
}

// 长度
int StudentSystem::Length()
{
    int length = 0;
    StudentNode* p = headNode->link;
    while (p)
    {
        p = p->link;
        length++;
    }
    cout << "目前考生人数为： " << length << endl;
    return length;
}

// 通过考号查找并输出信息
void StudentSystem::Search()  
{
    cout << "请输入要查找的考生的考号： ";
    string stuNumber;
    cin >> stuNumber;
    StudentNode* current = Locate(stuNumber);
    if (!current)
        cout << "该考生不存在！\n";
    else
    {
        cout << setiosflags(ios::left)
            << setw(12) << "考号"
            << setw(12) << "姓名"
            << setw(12) << "性别"
            << setw(12) << "年龄"
            << setw(12) << "报考类别";
        cout << endl;
        cout << current->data;
    }
}

// 插入学生信息
void StudentSystem::Insert()  
{
    cout << "请输入您要插入考生的位置： ";
    int i;
    cin >> i;
    StudentNode * precurrent = Locate(i - 1);
    if (!precurrent)
        cout << "您输入的位置不存在！\n";
    else
    { 
        cout << "请依次输入要插入考生的考号、姓名、性别、年龄及报考类别!\n";
        StudentInfo stu;
        cin >> stu;
        StudentNode* newNode = new StudentNode(stu);
        newNode->link = precurrent->link;
        precurrent->link = newNode;
        Output();              // 输出结果
    }
}

// 通过考号删除学生信息
void StudentSystem::Delete()   
{
    cout << "请输入你要删除的考生考号： ";
    string stuNumber;
    cin >> stuNumber;
    StudentNode * del = Locate(stuNumber);
    if (!del)
        cout << "您输入的考生不存在！\n";   // 要删除的学生不存在
    else
    {
        cout << "你删除的考生信息是： " << del->data << endl;
        if (headNode->link == del)     // 要删除结点时第一个结点
        {
            headNode->link = rearNode = NULL;
            delete del;
        }
        else
        {
            StudentNode* q;
            if (del->link == NULL)
                q = new StudentNode;   // 要删除结点是第一个结点
            else
                q = del->link;     // 与当前结点的下一个结点交换数据后删除下一个结点
            del->data = q->data;           // 用下一个结点数据覆盖当前数据
            del->link = q->link;
            delete q;
        }
    }
    Output();
}

// 通过考号修改学生信息
void StudentSystem::Update()                  
{
    cout << "请输入您要修改的考生的学号： ";
    string stuNumber;
    cin >> stuNumber;
    StudentNode * current = Locate(stuNumber);
    if (!current)
        cout << "您要修改的考生不存在！\n";
    else
    {
        cout << "请依次输入更改后考生的考号、姓名、性别、年龄及报考类别!\n";
        StudentInfo stu;
        cin >> stu;
        current->data = stu;
        Output();
    }
}

// 功能选择函数
void StudentSystem::Switch(int choice)
{
    switch (choice)
    {
    case 0: exit(1); break;
    case 1: Insert(); break;
    case 2: Delete(); break;
    case 3: Search(); break;
    case 4: Update(); break;
    case 5: Length(); break;
    }
}

// 输出函数
void StudentSystem::Output()
{
    if (headNode->link)
    {
        cout << setiosflags(ios::left)
            << setw(12) << "考号"
            << setw(12) << "姓名"
            << setw(12) << "性别"
            << setw(12) << "年龄"
            << setw(12) << "报考类别";
        cout << endl;
        StudentNode* current = headNode->link;
        while (current)
        {
            cout << current->data;
            current = current->link;
        }
    }
    else
        cout << "无考生！" << endl;
}


/*================================= 主函数 ===================================*/
int main()
{
    StudentSystem stuSystem;
    stuSystem.Interface();             // 初始化界面
    stuSystem.Create();                // 创建系统
    stuSystem.Output();                // 输出刚创建的系统
    int choice;                        // 功能选择
    while (1)
    {
        cout << "\n请选择您要进行的操作： ";
        cin >> choice;
        if (choice < 0 || choice > 5)
        {
            cout << "无此操作码，请重新输入： ";
            cin >> choice;
        }
        stuSystem.Switch(choice);
    }
    return 0;
}