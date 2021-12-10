#include<iostream>
using namespace std;
class My_node
{
private:
    double coefficient;//系数
    int index;//指数
    My_node* link;
public:
    friend class MyList;
    My_node();
    My_node(double a,int b){coefficient=a;index=b;link=NULL;}//构造函数
};
My_node* temp;
class MyList
{
private:
    My_node* first;
    My_node* last;
public:
    MyList(){last=first=new My_node(0,0);}//构造，不妨让头结点的指数表示长度，注意我表示的是除了头结点之外的长度
    void push_back(My_node* p);//从末尾添加节点
    void pre_insert(int now,My_node* p);//从第now个节点的前面插入
    void lat_insert(int now,My_node* p);//从第now个节点的后面插入,注意头结点是第0个，理论不能在头结点前面插入
    void print();
    void sum(MyList& B);
    My_node* get_first(){return first;}
    My_node* get_last(){return last;}
    int get_length(){return first->index;}
};
void MyList::push_back(My_node* p)
{
    last->link=p;
    last=p;
    first->index++;//长度加一
}
void MyList::pre_insert(int now,My_node* p)
{
    My_node* q=first;
    int t=now-1;
    while(t>0){q=q->link;t++;}
    p->link=q->link;
    q->link=p;
    first->index++;//长度加一
}
void MyList::lat_insert(int now,My_node* p)
{
    My_node* q=first;
    while(now--) {q=q->link;}
    p->link=q->link;
    q->link=p;
    first->index++;//长度加一
}
void MyList::print()//打印
{
    int m=first->index;
    My_node* p=first->link;
    for(int i=0;i<m;i++)
    {
        if(p==first->link) 
        {
            cout<<p->coefficient;
            if(p->index>1) cout<<"x^"<<p->index;
            else cout<<"x";
        }
        else 
        {
            cout<<" "<<(p->coefficient>0?"+":"-")<<" "<<p->coefficient;
            if(p->index>1) cout<<"x^"<<p->index;
            else cout<<"x";
        }
        p=p->link;
    }
}
void MyList::sum(MyList& B)//求和操作
{
    My_node* a=first;    
    My_node* b=B.first;
    while((a!=last)&&(b!=B.last))
    {
        if(a->link->index==b->link->index)
        {
            a->link->coefficient+=b->link->coefficient;
            a=a->link;b=b->link;
        }
        else if(a->link->index<b->link->index) 
        {
            a=a->link;
        }
        else 
        {
            My_node* p=first;
            while(p!=a) p=p->link;
             temp=new My_node(b->coefficient,b->index);
            temp->link=p->link;
            p->link=temp;
            first->link++;
            b=b->link;
        }
    }
    while(b!=B.last)
    {
        push_back(b->link);
        b=b->link;
    }
}
int main()
{
    MyList A,B;
    int n,coe,ind;
    cin>>n;
    for(int i=0;i<n;i++)//读入A
    {
        cin>>coe>>ind;
        temp=new My_node(coe,ind);
        A.push_back(temp);
    }
    cin>>n;
    for(int i=0;i<n;i++)//读入B
    {
        cin>>coe>>ind;
        temp=new My_node(coe,ind);
        B.push_back(temp);
    }
    cout<<"print A: ";
    A.print();
    cout<<endl<<"print B: ";
    B.print();
    cout<<endl<<"print A again: ";
    A.sum(B);//对AB求和
    A.print();//打印A
    delete temp;
    return 0;
}
/*测试样例：
6
2 1 4 2 10 3 23 5 4 7 5 8
4
3 3 5 5 7 9 8 10
运行结果：
print A: 2x + 4x^2 + 10x^3 + 23x^5 + 4x^7 + 5x^8
print B: 3x^3 + 5x^5 + 7x^9 + 8x^10   
print A again: 2x + 4x^2 + 13x^3 + 28x^5 + 4x^7 + 5x^8 + 7x^9 + 8x^10
*/