#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <stack>
#define DefaultVertexNum 50
using namespace std;

template <class weightType>
class edge
{
private:
    int nexVex;        //后继节点
    weightType weight; //权值
public:
    edge() { nexVex = -1; }
    edge(int nexVex, weightType weight)
    {
        this->nexVex = nexVex;
        this->weight = weight;
    }
    int get_nexVex() { return nexVex; }
    weightType get_weight() { return weight; }
};

template <class dataType, class weightType>
class vertex
{
private:
    dataType data;
    list<edge<weightType>> link;

public:
    //构造函数
    vertex()
    {
        link=list<edge<weightType>>(0);
    }
    vertex(dataType data)
    {
        this->data = data;
        link=list<edge<weightType>>(0);
    }

    dataType get_data() { return data; }
    void set_data(dataType data) { this->data = data; }
    list<edge<weightType>> &get_link() { return link; }
};

template <class dataType, class weightType>
class NetWork
{
private:
    vertex<dataType, weightType> *vex; //节点数组
    int vexNum, edgeNum;               //节点数和边数
    int *indegree;

public:
    NetWork(int vexNum);                                                //构造函数
    NetWork();                                                          //无参构造
    ~NetWork() { delete[] vex; }                                        //析构函数
    int first_adj(int u);                                               // u的第一个邻接点
    int next_adj(int u, int v);                                         // u相对于v的下一个邻接点
    void add_edge(int pre, int nex, int weight);                        //添加端点为pre和nxt的边
    dataType get_data(int v) const { return vex[v].get_data(); }        //得到顶点的值
    weightType get_weight(int u, int v);                                //得到第u个节点的与v相连的边的权值
    void set_data(int v, dataType *e) { vex[v].set_data(*e); }          //设置顶点的值
    int get_vexNum() const { return vexNum; }                           //得到顶点数
    int get_edgeNum() const { return edgeNum; }                         //得到边数
    void add_vex(vertex<dataType, weightType> v) { vex[vexNum++] = v; } //添加一个顶点
    void add_vex(dataType data);
    void add_vex(dataType *datas, int data_size);//添加多个顶点
    void get_indegree(); //构造入度数组
    void criticalPath(); //求关键路径
};

template <class dataType, class weightType>
NetWork<dataType, weightType>::NetWork(int vexNum_max)
{
    this->vexNum = 0;
    this->vex = new vertex<dataType, weightType>[vexNum_max];
    this->indegree = new int[vexNum_max];
}

template <class dataType, class weightType>
NetWork<dataType, weightType>::NetWork()
{
    this->vex = new vertex<dataType, weightType>[DefaultVertexNum];
    this->indegree = new int[DefaultVertexNum];
    vexNum = 0;
    edgeNum = 0;
}

template <class dataType, class weightType>
int NetWork<dataType, weightType>::first_adj(int u)
{
    list<edge<weightType>> &templist = vex[u].get_link();
    if (templist.size() > 0)
        return (*templist.begin()).get_nexVex();
    else
        return -1;
}

template <class dataType, class weightType>
weightType NetWork<dataType, weightType>::get_weight(int u, int v)
{
    list<edge<weightType>> &templist = vex[u].get_link();
    typename list<edge<weightType>>::iterator it = templist.begin();
    for (; it != templist.end(); it++)
    {
        if (it->get_nexVex() == v)
        {
            return it->get_weight();
        }
    }
    string s = "edge not fount";
    throw s;
}

template <class dataType, class weightType>
void NetWork<dataType, weightType>::add_vex(dataType data)
{
    vertex<dataType, weightType>* temp=new vertex<dataType,weightType>(data);
    add_vex(*temp);
}

template <class dataType, class weightType>
int NetWork<dataType, weightType>::next_adj(int u, int v)
{
    list<edge<weightType>> &templist = vex[u].get_link();
    typename list<edge<weightType>>::iterator it = templist.begin();
    for (; it != templist.end(); it++)
    {
        if (it->get_nexVex() == v)
        {
            if (++it != templist.end())
                return it->get_nexVex();
            else
                break;
        }
    }
    return -1;
}

template <class dataType, class weightType>
void NetWork<dataType, weightType>::add_edge(int pre, int nex, int weight)
{
    if (pre < 0 || nex >= vexNum)
    {
        string s = "Out of bounds "; //抛出异常
        throw s;
    }

    list<edge<weightType>> &templist = vex[pre].get_link();
    edge<weightType> *tempedge = new edge<weightType>(nex, weight);
    templist.push_back(*tempedge);
    edgeNum++;
}

template <class dataType, class weightType>
void NetWork<dataType, weightType>::add_vex(dataType *datas, int data_size)
{
    for (int i = 0; i < data_size; i++)
    {
        add_vex(datas[i]);
    }
}
template <class dataType, class weightType>
void NetWork<dataType, weightType>::get_indegree()
{
    for (int i = 0; i < vexNum; i++)
    {
        indegree[i] = 0; //先初始化
    }
    for (int i = 0; i < vexNum; i++)
    {
        for (int j = first_adj(i); j != -1; j = next_adj(i, j))
        {
            indegree[j]++;
        }
    }
}

template <class dataType, class weightType>
void NetWork<dataType, weightType>::criticalPath()
{
    int *ve = new int[vexNum]; //最早时刻数组
    queue<int> q;              //存储入度为0的节点
    stack<int> s;              //实现逆拓扑的栈
    int count = 0;

    for (int i = 0; i < vexNum; i++)
    {
        ve[i] = 0; //初始化最早开始时间数组
    }
    indegree=new int[vexNum];
    get_indegree();
    for (int i = 0; i < vexNum; i++)
    {
        if (indegree[i] == 0)
            q.push(i);
    }
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        s.push(u);
        count++;
        //对出队的节点所有的邻接点入度减一，如果为0就入队
        for (int v = first_adj(u); v != -1; v = next_adj(u, v))
        {
            if (--indegree[v] == 0)
                q.push(v);
            if (ve[u] + get_weight(u, v) > ve[v]) //修改ve
                ve[v] = ve[u] + get_weight(u, v);
        }
    }
    if (count < vexNum)
    {
        delete[] ve;
        string s = "can't sort by TOP";
        throw s; //抛出异常不能拓扑排序
    }

    int *vl = new int[vexNum];
    int u = s.top();
    for (int i = 0; i < vexNum; i++)
    {
        vl[i] = ve[u]; //最迟开始时间都初始化为汇点的时间
    }
    while (!s.empty())
    {
        int u = s.top();
        s.pop();
        for (int v = first_adj(u); v != -1; v = next_adj(u, v))
        {
            //修改vl[u]
            if (vl[v] - get_weight(u, v) < vl[u])
            {
                vl[u] = vl[v] - get_weight(u, v);
            }
        }
    }

    //求关键路径
    for (int u = 0; u < vexNum; u++)
    {
        for (int v = first_adj(u); v != -1; v = next_adj(u, v))
        {
            int ee = ve[u];
            int el = vl[v] - get_weight(u, v);
            if (ee == el)
            {
                cout << "<" << u << "," << v << ">" << endl;
            }
        }
    }
    delete[] ve;
    delete[] vl;
}
int main()
{
    int *vex = new int[9];
    for (int i = 0; i < 9; i++)
        vex[i] = 0;
    //在求关键路径的时候顶点的值并不重要，全部初始化为0；
    
    NetWork<int, int> net;
    net.add_vex(vex,9);
    try
    {
        net.add_edge(0, 1, 6);
        net.add_edge(0, 2, 4);
        net.add_edge(0, 3, 5);
        net.add_edge(1, 4, 1);
        net.add_edge(2, 4, 1);
        net.add_edge(3, 5, 2);
        net.add_edge(4, 6, 9);
        net.add_edge(4, 7, 7);
        net.add_edge(5, 7, 4);
        net.add_edge(6, 8, 2);
        net.add_edge(7, 8, 3);
    }
    catch (string s)
    {
        cout << s << endl;
    }
    net.criticalPath(); //输出关键路径;
    return 0;
}
