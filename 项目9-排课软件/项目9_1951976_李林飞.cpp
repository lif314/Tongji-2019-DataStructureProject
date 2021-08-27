/*
* FileName: CoursseScheduling.cpp
* Author : 李林飞 
* StuNumber: 1951976
* Date: 2020.12.20
*/


#define _CRT_SECURE_NO_WARNINGS    // 用到strcpy()、strncpy() 

#include <iostream>
#include <vector>    
#include <cstring>
#include <fstream> 
#include <iomanip>

using namespace std;



/*========================================== 队列类 =======================================*/
// 队列结点
template<class Type>
struct QueueNode
{
    Type data;
    QueueNode<Type>* link;
    QueueNode(QueueNode<Type>* next = NULL) :link(next) {}
    QueueNode(Type d, QueueNode<Type>* next = NULL) :data(d), link(next) {}
};

template<class Type>
class Queue
{
private:
    QueueNode<Type>* front;    // 队首指针
    QueueNode<Type>* rear;     // 队尾指针

public:
    Queue() :front(NULL), rear(NULL) {}  // 构造函数
    ~Queue();                            // 析构函数
    Type Front();                        // 返回队首元素
    Type Back();                         // 返回队尾元素
    void Push(const Type& item);         // 入队
    void Pop();                          // 出队
    bool Empty();                        // 判空
    int Size();                          // 长度
};

// 析构函数 —— 释放空间
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
    if (front == NULL)   // 空队列
    {
        front = rear = new QueueNode<Type>(item);
        if (front == NULL) {
            cout << "空间分配失败！" << endl;
            exit(1);
        }
    }
    else               // 非空队列
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

// 返回队列长度
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




/*========================================== 图类 ===========================================*/
class Graph
{
private:
    int numVertex;           // 顶点数
    int numEdge;             // 边数
    int** matrix;            // 邻接矩阵    
    int* mark;               // 标记数组
    int* inDegree;           // 入度

public:
    Graph(int n);            // 构造函数
    ~Graph();                // 析构函数
    int Vnum()const;         // 返回顶点数
    int Enum()const;         // 返回边数

    int first(int v)const;                // 返回顶点v的邻接点
    int next(int v, int w)const;          // 返回下一邻接点
    void setEdge(int v, int w, int wgt);  // 添加边
    void deletEdge(int v, int w);         // 删除边
    int weight(int v, int w)const;        // 返回边(v,w)的权值
    int getMark(int v)const;              // 返回结点v的标记状况
    void setMark(int v, int val);         // 将顶点v标记为val
    void subOneInDegree(int v);           // 将与V相邻的点入度减一
    int* getAllIndegree();                // 返回存储入度的数组
};

// 构造函数 —— 初始化顶点数、动态申请空间
Graph::Graph(int n)
{
    numVertex = n;
    numEdge = 0;

    // 标记数组
    mark = new int[numVertex];
    for (int i = 0; i < numVertex; i++)
        mark[i] = 0;
   
    // 邻接矩阵
    matrix = new int* [numVertex];
    for (int i = 0; i < numVertex; i++)
        matrix[i] = new int[numVertex];
    
    // 邻接矩阵初始化为0
    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
            matrix[i][j] = 0;
       
    // 入度数组初始化
    inDegree = new int[numVertex];
    for (int i = 0; i < numVertex; i++)
        inDegree[i] = 0;
   
}

// 析构函数 —— 销毁数组空间
Graph::~Graph()
{
    delete mark;
    for (int i = 0; i < numVertex; i++)
        delete matrix[i];
   
    delete matrix;
    delete inDegree;
}

// 返回顶点数
int Graph::Vnum()const
{
    return numVertex;
}

// 返回边数
int Graph::Enum()const
{
    return numEdge;
}

// 返回顶点v的邻接顶点
int Graph::first(int v)const
{
    for (int i = 0; i < numVertex; i++)
    {
        if (0 != matrix[v][i])
        {
            return i;
        }
    }

    return numVertex;
}

// 返回结点v相邻的下一个顶点
int Graph::next(int v, int w)const
{
    for (int i = w + 1; i < numVertex; i++)
    {
        if (0 != matrix[v][w])
        {
            return i;
        }
    }

    return numVertex;
}

// 添加边（v,w），权值为wgt
void Graph::setEdge(int v, int w, int wgt)
{
    if (wgt < 0)
    {
        std::cout << "权值不能为0";
    }

    if (0 == matrix[v][w])
    {
        numEdge++;        // 边数加1
        inDegree[w]++;    // w顶点入度加1
    }

    matrix[v][w] = wgt;
}

// 删除边（v,w）
void Graph::deletEdge(int v, int w)
{
    if (0 != matrix[v][w])
    {
        numEdge--;       // 边数减1
        inDegree[w]--;   // w顶点入度减1；
        matrix[v][w] = 0;
    }
}

// 返回边(v,w)的权值
int Graph::weight(int v, int w)const
{
    return matrix[v][w];
}

// 返回顶点v的标记状况
int Graph::getMark(int v)const
{
    if (v<0 || v>numVertex)
    {
        std::cout << "传入点无效";
    }
    return mark[v];
}

// 将顶点v标记为val
void Graph::setMark(int v, int val)
{
    if (v<0 || v>numVertex)
    {
        std::cout << "传入点无效";
    }

    mark[v] = val;
}

// 将与顶点v相邻接的顶点的入度减1
void Graph::subOneInDegree(int v)
{
    for (int i = 0; i < numVertex; i++)
    {
        if (0 != matrix[v][i])
        {
            inDegree[i]--;
        }
    }
}

// 返回入度信息数组
int* Graph::getAllIndegree()
{
    return inDegree;
}



/*================================================== 课程类 ============================================*/
//存储课程信息
class Course
{
private:
    string code;               // 课程编号
    string name;               // 课程名称
    short studyTime;           // 学时
    short term;                // 学期
    vector<string> condition;  // 先修课程

    bool isLimit;              //是否有先修课程

public:
    // 构造、析构函数
    Course(string code, string name, short studyTime, short term);
    Course() {}
    ~Course() {}

    //课程编号
    void setCode(string code);
    string getCode();

    //课程名称
    void setName(string mname);
    string getName();

    //课程学时
    void setStudyTime(short mstudyTime);
    short getStudyTime();

    //课程开始学期
    void setTerm(short mterm);
    short getTerm();

    // 先修课程
    void addCondition(string code);
    vector<string> getCondition();

    // 是否有先修课程
    bool IsLimit();   
};


// 构造函数 —— 初始化课程信息
Course::Course(string mcode, string mname, short mstudyTime, short mterm)
{
    isLimit = false;
    code = mcode;
    name = mname;
    studyTime = mstudyTime;
    term = mterm;
}

// 课程编号
void Course::setCode(string mcode)
{
    code = mcode;
}

string Course::getCode()
{
    return code;
}

// 课程名称
void Course::setName(string mname)
{
    name = mname;
}

string Course::getName()
{
    return name;
}

// 课程学时
void Course::setStudyTime(short mstudyTime)
{
    studyTime = mstudyTime;
}

short Course::getStudyTime()
{
    return studyTime;
}

// 课程学期
void Course::setTerm(short mterm)
{
    term = mterm;
}

short Course::getTerm()
{
    return term;
}

// 先修课程
void Course::addCondition(string code)
{
    isLimit = true;
    condition.push_back(code);
}

vector<string> Course::getCondition()
{
    return condition;
}

// 是否有先修课程
bool Course::IsLimit()
{
    return isLimit;
}






/*====================================== 排课类 ============================================*/
#define TermNum 8       // 学期数

class CourseScheduling
{
private:
    /*=== 数据成员 ===*/
    char course_table[TermNum + 1][11][6][100];   // 每门课程的课表安排：学期、节次、星期、课程名
    int numsOfTerm[TermNum];                      // 每学期课程数
    string InFileName;                            // 课程信息文件名 —— 读
    string OutFileName;                           // 课表信息文件名 —— 写
    Queue<int>  q[TermNum + 1];                   // 度为0顶点对应的学期队列

    /*==== 辅助函数 ==== */
    // 读文件
    int readFile(fstream& in, int* nums, Course* courses);
    // 初始化课程信息 —— 返回课程数目
    int GetAllCourseFromFile(Course* courses, string fileName, int* nums);

    // 创建图
    void CreateGraph(Graph& gra, Course* courses, int numOfCourses);

    // 拓扑排序
    bool TopologicalOrder(Graph& gra, Course* courses);
    // 拓扑排序辅助函数 —— 将入度为0的顶点入对应的队列
    void zero_indgree_push(Graph& gra, Course* courses);
 
    //  排课
    bool range(int num, int term, char* name);
    // 排课辅助函数 —— 排第一、二、三节课
    bool range_one(int& week_day, int term, char* course_name);
    bool range_two(int& week_day, int term, char* course_name);
    bool range_three(int& week_day, int term, char* course_name);
    // 查找空课，并返回第一节课的节次
    int find_one(int week_day, int term);
    int find_two(int week_day, int term);
    int find_three(int week_day, int term);


public:
    void Interface();                           // 初始化系统界面
    void ReadInFileName(string fileName);       // 读取课程信息文件名
    bool CreateCourseTable();                   // 创建课表
    void ReadOutFileName(string fileName);      // 读取课表文件名
    void PrintCourseTable();                    // 输出课表
};


// 读文件
int CourseScheduling::readFile(fstream& in, int* nums, Course* courses)
{
    while (1)
    {
        cout << "课程总数为38,请输入八个学期的课程分布（空格隔开）：";
        int total = 0;
        for (int i = 0; i < 8; i++)
        {
            cin >> nums[i];
            total += nums[i];
        }

        if (total == 38)
            break;
        else
            cout << "\n课程分布错误！请重新输入： ";
    }
   
    int count = 0;
    char* chstemp = new char[50];
    char temp;
    short numtemp;
    while (!in.eof())
    {
        in >> chstemp;
        // 文件末尾为空格
        if (in.eof())
        {
            break;
        }
        courses[count].setCode(chstemp);
        in >> chstemp;
        courses[count].setName(chstemp);
        in >> numtemp;
        courses[count].setStudyTime(numtemp);
        in >> numtemp;
        courses[count].setTerm(numtemp);

        while ('\n' != (temp = in.get()))
        {
            if (' ' != temp && '\t' != temp)
            {
                in.putback(temp);
                in >> chstemp;
                courses[count].addCondition(chstemp);
            }
        }
        count++;
    }

    //释放空间
    delete [] chstemp;

    return count;
}


//从文件中读取信息，并保存在Course类中,返回读取的课程总数
int CourseScheduling::GetAllCourseFromFile(Course* courses, string fileName, int* nums)
{
    fstream tempInFile(fileName, ios::in);
    if (!tempInFile)
    {
        cout << "In打开文件失败！" << fileName.data();
        return -1;
    }
    int numberOfCourse;
    numberOfCourse = readFile(tempInFile, nums, courses);
    tempInFile.close();

    return numberOfCourse;
}


// 初始化界面
void CourseScheduling::Interface()
{
    cout << "******************************************************\n";
    cout << "**                                                  **\n";
    cout << "**                软件学院排课系统                  **\n";
    cout << "**                                                  **\n";
    cout << "**                      8个学期38节课任你来选！！   **\n";
    cout << "**                                                  **\n";
    cout << "******************************************************\n\n";
}

// 读取课程信息文件名
void CourseScheduling::ReadInFileName(string fileName)
{
    InFileName = fileName;
}

// 读取课表文件名称
void CourseScheduling::ReadOutFileName(string fileName)
{
    OutFileName = fileName;
}


// 创建图
void CourseScheduling::CreateGraph(Graph& gra, Course* courses, int numOfCourses)
{
    vector<string> conditions;
    for (int i = 0; i < numOfCourses; i++)
    {
        if (courses[i].IsLimit())
        {
            conditions = courses[i].getCondition();
            for (int j = 0; j < (int)conditions.size(); j++)
            {
                for (int k = 0; k < numOfCourses; k++)
                {
                    if (courses[k].getCode() == conditions.at(j))
                    {
                        gra.setEdge(k, i, 1);
                        break;
                    }
                }
            }
        }
    }

    return;
}

// 拓扑排序
bool CourseScheduling::TopologicalOrder(Graph& gra, Course* courses)
{

    zero_indgree_push(gra, courses);
    //对term中对应的课程排课
    int v;
    int size;
    Course course;
    char name[100];
    for (int term = 1; term <= TermNum; term++)
    {
        size = (int)q[term].Size();
        for (int i = 0; i < size; i++)
        {
            v = q[term].Front();
            q[term].Pop();
            //排课
            course = courses[v];
            strncpy(name, course.getName().data(), 99);
            range(course.getStudyTime(), term, name);

            //所有越v相邻的顶点入度减一
            gra.subOneInDegree(v);

            //入队
            int* inDegrees = gra.getAllIndegree();
            for (int j = 0; j < gra.Vnum(); j++)
            {
                if (0 == inDegrees[j] && (0 == courses[j].getTerm() || courses[j].getTerm() > term) && 0 == gra.getMark(j))
                {
                    q[courses[j].getTerm()].Push(j);
                    gra.setMark(j, 1);//标记已经访问
                }
                else if (0 == inDegrees[j] && (courses[j].getTerm() > 0 && courses[j].getTerm() <= term) && 0 == gra.getMark(j))
                {
                    cout << "课程冲突，排课结束！" << endl;
                    return false;;
                }
              
            }

        }

        for (int i = 0; i < numsOfTerm[term - 1] - size; i++)
        {
            v = q[0].Front();
            q[0].Pop();
            //排课
            course = courses[v];
            strncpy(name, course.getName().data(), 99);
            range(course.getStudyTime(), term, name);
            // 所有越v相邻的顶点入度减一
            gra.subOneInDegree(v);

            //入队
            int* inDegrees = gra.getAllIndegree();
            for (int j = 0; j < gra.Vnum(); j++)
            {
                if (0 == inDegrees[j] && (0 == courses[j].getTerm() || courses[j].getTerm() > term) && 0 == gra.getMark(j))
                {
                    q[courses[j].getTerm()].Push(j);
                    gra.setMark(j, 1);//标记已经访问
                }
                else if (0 == inDegrees[j] && (0 < courses[j].getTerm() && courses[j].getTerm() <= term) && 0 == gra.getMark(j))
                {
                    cout << "课程冲突，排课结束！" << endl;
                    return false;
                }
            }

        }
    }

    for (int i = 0; i < gra.Vnum(); i++)
    {
        if (0 == gra.getMark(i))
        {
            cout << "并未排完所有课程，排课失败！";
            return false;
        }
    }

    return true;
}


void CourseScheduling::zero_indgree_push(Graph& gra, Course* courses)
{
    int* inDegrees = gra.getAllIndegree();
    // 将度为0的点入对应的队列
    for (int i = 0; i < gra.Vnum(); i++)
    {
        if (0 == inDegrees[i])
        {
            q[courses[i].getTerm()].Push(i);
            gra.setMark(i, 1);// 标记已经访问
        }
    }
}

// 排课
bool CourseScheduling::range(int num, int term, char* name)
{
    int static week_day = 1;

    if (num == 1)
    {
        if (range_one(week_day, term, name) == false)
        {
            return false;
        }
        else
        {
            week_day = (week_day + 2 -5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
            return true;
        }
    }
    else if (num % 3 == 0)
    {
        for (int i = 0; i < num / 3; i++)
        {
            if (range_three(week_day, term, name) == false)
            {
                return false;
            }
            else
            {
                week_day = (week_day + 2 - 5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
                return true;
            }
        }
    }
    else if (num % 2 == 0)
    {
        for (int i = 0; i < num / 2; i++)
        {
            if (range_two(week_day, term, name) == false)
            {
                return false;
            }
            else
            {
                week_day = (week_day + 2 - 5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
                return true;
            }
        }
    }
    else if (5 == num)
    {
        if (range_three(week_day, term, name) == false)
        {
            return false;
        }
        else
        {
            week_day = (week_day + 2 - 5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
            if (range_two(week_day, term, name) == false)
            {
                return false;
            }
            else
            {
                week_day = (week_day + 2 - 5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
                return true;
            }
        }
    }
    return false;
}


// 辅助排课，排一节课
bool CourseScheduling::range_one(int& week_day, int term, char* course_name)
{
    int local;
    for (int i = 0; i < 5; i++)
    {
        local = find_one(week_day, term);
        if (-1 == local)
        {
            week_day = (week_day + 2 - 5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
        }
        else
        {
            strcpy(course_table[term][local][week_day], course_name);
            return true;
        }
    }
    return false;
}


// 辅助排课，排两节课
bool CourseScheduling::range_two(int& week_day, int term, char* course_name)
{
    int local;
    for (int i = 0; i < 5; i++)
    {
        local = find_two(week_day, term);
        if (-1 == local)
        {
            week_day = (week_day + 2 - 5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
        }
        else
        {
            strcpy(course_table[term][local][week_day], course_name);
            strcpy(course_table[term][local + 1][week_day], course_name);
            return true;
        }
    }
    return false;

}

// 辅助排课，排三节课
bool CourseScheduling::range_three(int& week_day, int term, char* course_name)
{
    int local;
    for (int i = 0; i < 5; i++)
    {
        local = find_three(week_day, term);
        if (-1 == local)
        {
             week_day = (week_day + 2 -5) > 0 ? (week_day + 2 - 5) : (week_day + 2);
        }
        else
        {
            strcpy(course_table[term][local][week_day], course_name);
            strcpy(course_table[term][local + 1][week_day], course_name);
            strcpy(course_table[term][local + 2][week_day], course_name);
            return true;
        }
    }
    return false;
}

// 查找一节空课，并返回第一节课的节次
int CourseScheduling::find_one(int week_day, int term)
{
    for (int i = 1; i <= 10; i++)
    {
        if (strlen(course_table[term][i][week_day]) == 0)
        {
            return i;
        }

    }
    return -1;
}

// 查找两节空课，并返回第一节课的节次
int CourseScheduling::find_two(int week_day, int term)
{
    if (strlen(course_table[term][1][week_day]) == 0 && strlen(course_table[term][2][week_day]) == 0)
    {
        return 1;
    }
    if (strlen(course_table[term][6][week_day]) == 0 && strlen(course_table[term][7][week_day]) == 0)
    {
        return 6;
    }
    if (strlen(course_table[term][3][week_day]) == 0 && strlen(course_table[term][4][week_day]) == 0)
    {
        return 3;
    }
    if (strlen(course_table[term][4][week_day]) == 0 && strlen(course_table[term][5][week_day]) == 0)
    {
        return 4;
    }
    if (strlen(course_table[term][8][week_day]) == 0 && strlen(course_table[term][9][week_day]) == 0)
    {
        return 8;
    }
    if (strlen(course_table[term][9][week_day]) == 0 && strlen(course_table[term][10][week_day]) == 0)
    {
        return 9;
    }
    return -1;
}

// 查找三节空课，并返回第一节课的节次
int CourseScheduling::find_three(int week_day, int term)
{
    if (strlen(course_table[term][3][week_day]) == 0 && strlen(course_table[term][4][week_day]) == 0 &&
        strlen(course_table[term][5][week_day]) == 0)
    {
        return 3;
    }
    if (strlen(course_table[term][8][week_day]) == 0 && strlen(course_table[term][9][week_day]) == 0 &&
        strlen(course_table[term][10][week_day]) == 0)
    {
        return 8;
    }
    return -1;
}

// 创建课表
bool CourseScheduling::CreateCourseTable()
{
    Course* temp = new Course[100];
    int allCourse;
    allCourse = GetAllCourseFromFile(temp, InFileName, numsOfTerm);
    if (allCourse < 0)
    {
        return false;
    }

    // 建图
    Graph gra(allCourse);
    CreateGraph(gra, temp, allCourse);

    // 清空课表
    for (int i = 1; i <= TermNum; i++)                       //学期数
        for (int j = 1; j <= 10; j++)                        //节次
            for (int k = 1; k <= 5; k++)                     //星期   
                strcpy(course_table[i][j][k], "");

    // 拓扑排序
    if (!TopologicalOrder(gra, temp))
    {
        //释放空间
        delete[]temp;
        return false;
    }

    delete[] temp;
    return true;
}


// 输出课表到文件中
void CourseScheduling::PrintCourseTable()
{
    fstream outToFile(OutFileName, ios::out);
    if (!outToFile)
    {
        cout << "打开文件失败!" << endl;
        return;
    }

    for (int i = 1; i <= 8; i++)
    {

        outToFile << '\t' << '\t' << '\t' << '\t' << "软件工程专业第" << i << "学期课表" << endl;
        outToFile << endl;
        outToFile << setw(20) << "名称";
        outToFile << setw(20) << "星期一";
        outToFile << setw(20) << "星期二";
        outToFile << setw(20) << "星期三";
        outToFile << setw(20) << "星期四";
        outToFile << setw(20) << "星期五";
        outToFile << endl;
        for (int j = 1; j <= 10; j++)
        {
            outToFile << setw(20) << "第" << j << "节";
            for (int k = 1; k <= 5; k++)
            {
                outToFile << setw(20) << course_table[i][j][k];
            }
            outToFile << endl;
            outToFile << endl;
        }

        outToFile << endl;
        outToFile << endl;
        outToFile << endl;
        outToFile << endl;
        outToFile << endl;
        outToFile << endl;
        outToFile << endl;
    }
    outToFile.close();
}



/*======================================== 主函数 ====================================*/
int main()
{
    
    CourseScheduling CS;    // 课表系统
    string fileName;        //文件名

    // 初始化系统界面
    CS.Interface();

    // 辅助测试信息
    cout << "辅助测试信息： \n";
    cout << "1.课程信息文件名： courseInformation.txt  \n";
    cout << "2.课表文件可任意命名的 .txt文件等\n";
    cout << "3.请保证8学期课程数之和等于课程总数！\n\n";

    // 读取课程信息
    cout << "请输入需读入的课程信息文件名： ";
    cin >> fileName;
    CS.ReadInFileName(fileName);

    while (1)
    {
        // 创建课表
        if (CS.CreateCourseTable())
            cout << "排课成功！请指定课表存放文件： \n";
        else
            cout << "排课失败！\n";

        // 课表存放文件名
        cin >> fileName;
        CS.ReadOutFileName(fileName);

        // 输出课表
        CS.PrintCourseTable();
        cout << "输出完毕！\n\n";

        cout << "继续排课？( y or n)";
        char tag = 'y';
        cin >> tag;
        if (tag == 'n')
            break;
    }

    cout << "谢谢使用！祝您每天开心！！！Bye~~\n\n";
    system("pause");   // 便于.exe文件退出前显示信息
    return 0;
}

/*====================================== ENDING ===================================================*/