/*
* FileName:  Sort.cpp
* Function:  排序算法比较
* Author:    李林飞
* StuNumber: 1951976
* Date:      2020.12.21
*/

#include <vector>    // 产生随机数算法vector
#include <iostream>
#include<algorithm>
#include <ctime>
#include <cstdlib>


using namespace std;


// 排序算法类
class SortAlgorithm
{
private:
    vector <int> RandNumbers;     // 随机数容器，每种排序操作使用相同的数据
    double SortTime;  // 排序所需时间
    long long ExchangeTimes; // 交换次数

    /*  辅助函数 */
    bool IsSorted(vector<int> & cur);              // 判断是否排序完成
    void Swap(vector<int>& randNum, int i, int j);   // 交换数据
    int partition(vector<int>& temp, int low, int high);    // 划分
    void quickSort(vector<int>& temp, int low, int high);
    void sink(vector<int>& temp, int k, int N);
    int getDigit(const vector<int>& temp);
    void merge(vector<int>& temp, vector<int>& aux, int lo, int mid, int hi);
    void mergeSort(vector<int>& temp, vector<int>& aux, int lo, int hi);

    /* 排序函数 */
    void BubbleSort();  // 冒泡排序
    void SelectSort();  // 选择排序
    void InsertSort();  // 直接插入排序
    void ShellSort();   // 希尔排序
    void QuickSort();   // 快速排序
    void HeapSort();    // 堆排序
    void MergeSort();   // 归并排序
    void RadixSort();   // 基数排序

public:
    SortAlgorithm(int Num);    //  构造函数，定义产生随机数的个数

    void Switch(int choice);   // 功能选择函数
};

// 构造函数
SortAlgorithm::SortAlgorithm(int Num)
{
    SortTime = 0.0;
    ExchangeTimes = 0;
    srand((unsigned)time(NULL));
    for (int i = 0; i < Num; i++)
        RandNumbers.push_back(rand() % Num);        // 产生随机数
}

// 功能选择函数
void SortAlgorithm::Switch(int choice)
{
    switch (choice)
    {
    case 1: BubbleSort(); break;
    case 2: SelectSort(); break;
    case 3: InsertSort(); break;
    case 4: ShellSort(); break;
    case 5: QuickSort(); break;
    case 6: HeapSort(); break;
    case 7: MergeSort(); break;
    case 8: RadixSort(); break;
    }
}

// 判断是否有序
bool SortAlgorithm::IsSorted(vector<int> & cur)
{
    for (int i = 0; i < cur.size() - 1; i++)
    {
        if (cur[i + 1] < cur[i])
        {
            return false;
        }
    }
    return true;
}

// 交换
void SortAlgorithm::Swap(vector<int> & randNum, int i , int j)
{
    int temp = randNum[i];
    randNum[i] = randNum[j];
    randNum[j] = temp;
}


 int SortAlgorithm::partition(vector<int>& temp, int low, int high)
    {
        int i = low, j = high + 1;
        int v = temp[low];
        while (true)
        {
            while (temp[++i] < v) if (i == high) break;
            while (v < temp[--j]) if (j == low) break;
            if (i >= j)
                break;
            Swap(temp, i, j);
            ExchangeTimes++;
        }
        Swap(temp, low, j);
        ExchangeTimes++;
        return j;
    }

void SortAlgorithm::quickSort(vector<int>& temp, int low, int high)
 {
    if (high <= low)
    {
        return;
    }
    int j = partition(temp, low, high);
    quickSort(temp, low, j - 1);
    quickSort(temp, j + 1, high);
 }

void SortAlgorithm::sink(vector<int>& temp, int k, int N)
{
    while (2 * k <= N)
    {
        int j = 2 * k;
        if (j + 1 <= N && temp[j + 1] > temp[j])
        {
            ++j;
        }
        if (temp[k] > temp[j])
        {
            break;
        }
        Swap(temp, k, j);
        k = j;
        ExchangeTimes++;
    }
}

int SortAlgorithm::getDigit(const vector<int>& temp)
{
    int digit = 1;
    int base = 10;
    for (int i = 0; i < temp.size(); ++i)
    {
        while (base <= temp[i])
        {
            base *= 10;
            ++digit;
        }
    }
    return digit;
}

void SortAlgorithm::merge(vector<int>& temp, vector<int>& aux, int lo, int mid, int hi)
{
    int i = lo, j = mid + 1;
    for (int k = lo; k <= hi; ++k)
    {
        aux[k] = temp[k];
    }
    for (int k = lo; k <= hi; ++k)
    {
        if (i > mid) temp[k] = aux[j++];
        else if (j > hi) temp[k] = aux[i++];
        else if (aux[j] < aux[i])
        {
            temp[k] = aux[j++];
            ExchangeTimes++;
        }
        else
        {
            temp[k] = aux[i++];
            ExchangeTimes++;
        }
    }
}

void SortAlgorithm::mergeSort(vector<int>& temp, vector<int>& aux, int lo, int hi)
{
    if (hi <= lo)
    {
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergeSort(temp, aux, lo, mid);
    mergeSort(temp, aux, mid + 1, hi);

    merge(temp, aux, lo, mid, hi);
}


// 冒泡排序
void SortAlgorithm::BubbleSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    for (int i = 1; i < temp.size(); ++i)
    {
        for (int j = temp.size() - 1; j >= i; --j)
        {
            if (temp[j - 1] > temp[j])
            {
                Swap(temp, j, j - 1);
                ExchangeTimes++;
            }
        }
    }
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;
    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "冒泡排序所用时间： " << SortTime << endl;
        cout << "冒泡排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 选择排序
void SortAlgorithm::SelectSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    for (int i = 0; i < temp.size() - 1; ++i)
    {
        int min = i;
        for (int j = i + 1; j < temp.size(); ++j)
        {
            if (temp[min] > temp[j])
            {
                min = j;
            }
        }
        Swap(temp, min, i);
        ExchangeTimes++;
    }
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;

    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "选择排序所用时间： " << SortTime << endl;
        cout << "选择排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 直接插入排序
void SortAlgorithm::InsertSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    for (int i = 1; i < temp.size(); ++i)
    {
        for (int j = i; j - 1 >= 0 && temp[j] < temp[j - 1]; --j)
        {
            Swap(temp, j - 1, j);
            ExchangeTimes++;
        }
    }
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;

    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "直接插入排序所用时间： " << SortTime << endl;
        cout << "直接插入排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 希尔排序
void SortAlgorithm::ShellSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    int gap = 1;
    while (gap < temp.size() / 3)
    {
        gap = 3 * gap + 1;
    }

    while (gap >= 1)
    {
        for (int i = gap; i < temp.size(); ++i)
        {
            for (int j = i; j - gap >= 0 && temp[j] < temp[j - gap]; j -= gap)
            {
                Swap(temp, j - gap, j);
                ExchangeTimes++;
            }
        }
        gap /= 3;
    }
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;
    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "希尔排序所用时间： " << SortTime << endl;
        cout << "希尔排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 快速排序
void SortAlgorithm::QuickSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    quickSort(temp, 0, temp.size() - 1);
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;
    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "快速排序所用时间： " << SortTime << endl;
        cout << "快速排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 堆排序
void SortAlgorithm::HeapSort()
{
    vector<int> temp(RandNumbers);
    int N = temp.size();

    clock_t start = clock();
    temp.insert(temp.begin(), -1);//temp[0]不用
    //构造堆
    for (int k = N / 2; k >= 1; --k)
    {
        sink(temp, k, N);
    }
    //排序
    while (N > 1)
    {
        Swap(temp, 1, N--);
        ExchangeTimes++;
        sink(temp, 1, N);
    }
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;
    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "堆排序所用时间： " << SortTime << endl;
        cout << "堆排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 归并排序
void SortAlgorithm::MergeSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    vector<int> aux(temp.size());//辅助数组
    mergeSort(temp, aux, 0, temp.size() - 1);
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;
    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "归并排序所用时间： " << SortTime << endl;
        cout << "归并排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}

// 基数排序
void SortAlgorithm::RadixSort()
{
    vector<int> temp(RandNumbers);

    clock_t start = clock();
    vector<int> aux(temp);
    int N = temp.size();
    int R = 10;            //一共只有0-9
    int digit = getDigit(temp);
    int base = 1;         //目的是得到一个数的某一位的数,如得到123中的2
    //0表示个位
    for (int d = 0; d < digit; ++d)
    {
        vector<int> count(R + 1);//计算出现的频率
        for (int i = 0; i < N; ++i)
        {
            //就算有一些的位数不够也没有关系,因为这样会是0,还是会排序
            int index = temp[i] / base % 10;//第一次得到是个位,第二次是十位..
            count[index + 1]++;
        }
        for (int r = 0; r < R; ++r)//将频率转换为索引
        {
            count[r + 1] += count[r];
        }
        for (int i = 0; i < N; ++i)//将元素分类
        {
            int index = temp[i] / base % 10;
            aux[count[index]++] = temp[i];
        }
        for (int i = 0; i < N; ++i)//回写
        {
            temp[i] = aux[i];
        }
        base *= 10;//不断处理高位
    }
    clock_t end = clock();

    SortTime = ((double)end - (double)start) / CLOCKS_PER_SEC;
    if (!IsSorted(temp))
        cout << "排序失败!" << endl;
    else
    {
        cout << "基数排序所用时间： " << SortTime << endl;
        cout << "基数排序交换次数: " << ExchangeTimes << endl;
    }
    SortTime = 0.0;     // 清零
    ExchangeTimes = 0;
}


// 初始化界面
void Interface()  
{
    cout << "*****************************************" << endl;
    cout << "**            排序算法比较             **" << endl;
    cout << "**                                     **" << endl;
    cout << "**            1---冒泡排序             **" << endl;
    cout << "**            2---选择排序             **" << endl;
    cout << "**            3---直接插入排序         **" << endl;
    cout << "**            4---希尔排序             **" << endl;
    cout << "**            5---快速排序             **" << endl;
    cout << "**            6---堆排序               **" << endl;
    cout << "**            7---归并排序             **" << endl;
    cout << "**            8---基数排序             **" << endl;
    cout << "**            0---结束当前测试循环     **" << endl;
    cout << "*****************************************" << endl;
}


/*======================== 主函数 ==========================*/
int main()
{
    Interface();       // 初始化界面

    char tag;

    do
    {
        cout << "请输入要产生的随机数的个数（输入0选择指定下一个随机个数）：";
        int N;
        cin >> N;

        while (N < 0)
        {
            cout << "请输入正整数： ";
            cin >> N;
        }

        SortAlgorithm sort(N);
        int choice;
        while (1)
        {
            cout << "\n请选择排序算法： ";
            cin >> choice;
            sort.Switch(choice);
            
            if (choice == 0)       // 结束当前测试循环
                break;
        }

        cout << "\n继续测试？(y or n)： ";
        cin >> tag;
    } while (tag == 'y');

    return 0;
}

