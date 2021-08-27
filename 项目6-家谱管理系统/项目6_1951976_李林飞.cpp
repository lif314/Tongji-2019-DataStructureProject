/*
* FileName:  FamilyTree.cpp
* Author:    李林飞
* StuNumber: 1951976
* Date:      2020.12.15
*/


#include <iostream>
#include <string>
using namespace std;

// 使用子女-兄弟链表法

/*=================== 个人结点类 =======================*/
struct PersonNode
{
    string personName;                // 人名
    PersonNode* firstChild = NULL;    // 左子女
    PersonNode* nextSibling = NULL;   // 右兄弟
    PersonNode(string value, PersonNode * first = NULL, PersonNode * next = NULL)
            : personName(value), firstChild(first), nextSibling(next) {}    // 构造函数
};


/*=================== 家谱树类 ===========================*/
class FamilyTree
{
private:
    PersonNode* root = nullptr;       // 根结点——祖先
    PersonNode* current = nullptr;    // 当前结点

    bool Find(string name);                                  // 遍历搜索name
    bool Find(PersonNode* p, string name);                   // 在以p为根的树中搜索name
    bool AddPerson(string familyName, string childName);     // 添加子女
    void PrintFamily(PersonNode * p);                        // 以广义表的形式打印家谱
    bool Update(string originName, string updateName);       // 更新名字
    bool DeletePerson(string name);                          // 删除家庭
    void Destroy(PersonNode* delbranch);                     // 销毁分支

public:
    FamilyTree() : root(nullptr) {}                         // 构造函数
    ~FamilyTree() { Destroy(root); }                        // 析构函数
    bool AddAncestor(string rootAncestor);                  // 添加祖先
    string GetAncestor();                                   // 获取祖先姓名
    void Switch(char choice);                               //功能选择函数 
};

// 功能选择函数
void FamilyTree::Switch(char choice)
{
    switch (choice)
    {
    // 添加新的子代
    case 'A':    
    {
        cout << "请输入要建立家庭的人的姓名： ";
        string fatherName;
        cin >> fatherName;
        cout << "请输入" << fatherName << "的儿女人数： ";
        int kidNum;
        cin >> kidNum;
        while (kidNum < 0)
        {
            cout << "人数必须为正整数，请重新输入：";
            cin >> kidNum;
        }
        cout << "请依次输入" << fatherName << "的儿女的姓名： ";
        string kidName;
        int IsExist = 1;
        while (kidNum--)
        {
            cin >> kidName;
            if (!AddPerson(fatherName, kidName))
                IsExist = 0;
        }
        if (IsExist == 1)
        {
            cout << fatherName << "的家庭是： ";
            PrintFamily(current);
        }
        else
            cout << "家谱中无此人！" << endl;
        break;
    }

    // 为某家庭添加子女
    case 'B':
    {
        cout << "请输入要添加子女的人的姓名： ";
        string fatherName;
        cin >> fatherName;
        cout << "请输入" << fatherName << "的子女的姓名： ";
        string kidName;
        cin >> kidName;
        if (AddPerson(fatherName, kidName))
        {
            cout << fatherName << "的家庭是： ";
            PrintFamily(current);
        }
        else
            cout << "家谱中无此人！" << endl;
        break;
    }

    // 解散家庭
    case 'C':
    {
        cout << "请输入要解散家庭人的姓名： ";
        string fatherName;
        cin >> fatherName;
        if (DeletePerson(fatherName))
            cout << "此时完整家谱是： ";
        else
            cout << "家谱中无此人！" << endl;

        cout << "此时完整家谱是： ";
        PrintFamily(root);
        break;
    }

    // 更改家庭成员姓名
    case 'D':
    {
        cout << "请输入要更改姓名的人的目前姓名： ";
        string originName;
        cin >> originName;
        cout << "请输入更改后的姓名： ";
        string newName;
        cin >> newName;
        if (Update(originName, newName))
            cout << originName << "已更名为" << newName << endl;
        else
            cout << "家谱中无此人！" << endl;

        cout << "此时完整家谱是： ";
        PrintFamily(root);
        break;
    }

    // 查询某家庭中成员
    case 'E':
    {
        cout << "请输入你要查询的家庭家谱： ";
        string fatherName;
        cin >> fatherName;
        cout << "该家庭家谱为： ";
        if (Find(fatherName))
            PrintFamily(current);
        else
            cout << "家谱中无此人！" << endl;
        break;
    }

    // 查询家谱所有成员
    case 'F': 
    {
        cout << "该大家庭完整家谱是（广义表形式： ";
        PrintFamily(root);
        break;
    }

    // 退出程序
    case 'G':
    {
        exit(1);
        break;
    }

    default: break;
    }
}

// 添加祖先
bool FamilyTree::AddAncestor(string rootAncestor)
{
    root = new PersonNode(rootAncestor);
    if (root)
        return true;
    return false;
}

// 返回祖先
string FamilyTree::GetAncestor()
{
    return root->personName;
}

// 添加子女
bool FamilyTree::AddPerson(string familyName, string childName)
{
    current = nullptr;                    // 从头遍历
    if (Find(root, familyName) == false)  // 没有找到
    {
        return false;
    }
    else
    {
        PersonNode* curChild = current->firstChild;  //判断双亲第一个孩子是否存在

        if (curChild == nullptr)                     // 第一个孩子不存在,直接成为第一个孩子
        {
            curChild = new PersonNode(childName);
            current->firstChild = curChild;
        }
        else                                 // 第一个孩子存在，加在该孩子的兄弟链上
        {
            PersonNode* p = current->firstChild;  // 指向当前结点的第一个孩子
            while (p->nextSibling != nullptr)
                p = p->nextSibling;

            p->nextSibling = new PersonNode(childName);  //加在兄弟链末尾
        }

        return true;
    }
}

// 从根结点遍历搜索名为name的人
bool FamilyTree::Find(string name)
{
    if (root == nullptr)
        return false;
    return Find(root, name);
}

// 在根为*p的树中找值为familyName的结点，找到使p成为当前结点，返回true，否则，当前结点不变，返回false
bool FamilyTree::Find(PersonNode* p, string name)
{
    bool result = false;
    if (p->personName == name)
    {
        result = true;
        current = p;
    }
    else
    {
        PersonNode* q = p->firstChild;
        while (q != nullptr && !(result = Find(q, name)))
            q = q->nextSibling;
    }
    return result;
}

// 更改姓名
bool FamilyTree::Update(string originName, string updateName)
{
    current = nullptr;            // 从根结点开始搜索
    if (Find(root, originName))
    {
        current->personName = updateName;
        return true;
    }
    else
        return false;
}

// 删除名为name的人的家庭
bool FamilyTree::DeletePerson(string name)
{
    current = nullptr;
    if (Find(root, name)) // 左子女-右兄弟法不能直接删除
    {
        current->personName = "#"; //延时实现删除操作
        PersonNode* p = current->firstChild;
        Destroy(current->firstChild);  // 销毁子代
        current->firstChild = nullptr;
        return true;
    }
    else
        return false;
}

// 销毁分支
void FamilyTree::Destroy(PersonNode* delbranch)
{
    if (delbranch != nullptr)
    {
        Destroy(delbranch->firstChild);
        Destroy(delbranch->nextSibling);
        delete delbranch;
    }
}

// 以广义表的形式打印家庭成员
void FamilyTree::PrintFamily(PersonNode * p)
{
    PersonNode * child;
    cout << p->personName;
    if (p->firstChild)
    { //有孩子
        cout << "(";
        for (child = p->firstChild; child->nextSibling; child = child->nextSibling)
        {
            PrintFamily(child);
            cout << ",";
        }
        PrintFamily(child);
        cout << ")";
    }
}

 // 初始化界面
void Interface() 
{
    cout << "********************************************" << endl;
    cout << "**            家谱管理系统                **" << endl;
    cout << "**                                        **" << endl;
    cout << "**         请选择要执行的操作             **" << endl;
    cout << "**            A---完善家谱                **" << endl;
    cout << "**            B---添加家庭成员            **" << endl;
    cout << "**            C---解散局部家庭            **" << endl;
    cout << "**            D---更改家庭成员姓名        **" << endl;
    cout << "**            E---查询小家庭家谱          **" << endl;
    cout << "**            F---查询完整家谱            **" << endl;
    cout << "**            G---退出程序                **" << endl;
    cout << "**                                        **" << endl;
    cout << "**          本系统使用广义表形式输出家谱！**" << endl;
    cout << "********************************************" << endl;
}

/*=========================== 主函数 =========================================*/
int main()
{
    Interface();                // 初始化界面
    FamilyTree family;
    cout << "\n首先请建立一个家谱！\n\n";
    cout << "请输入祖先的姓名： ";
    string name;
    cin >> name;
    family.AddAncestor(name);  // 添加祖先
    cout << "此家谱的祖先是： " << family.GetAncestor() << endl;

    char choice;
    while (1)
    {
        cout << "\n\n请选择要执行的操作： ";
        cin >> choice;

        while (choice < 'A' || choice > 'G')  // 操作码检测
        {
            cout << "无操作，请重新选择： ";
            cin >> choice;
        }

        family.Switch(choice);   // 功能选择
    }

    system("pause");    // 便于.exe文件退出前显示结果
    return 0;
}



