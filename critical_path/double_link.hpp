#pragma once

using namespace std;
template <class T>
struct double_link_node //节点
{
    double_link_node<T> *prev;
    double_link_node<T> *next;
    T data;
    double_link_node(){prev=NULL;next=NULL;}
};
template <class T>
class double_link //双向链表
{
public:
    double_link();//构造函数
    bool pre_insert(double_link_node<T> *p, T member); //在当前节点前面插入
    bool lat_insert(double_link_node<T> *p, T member); //在当前节点后面插入
    bool push_back(T member);//在最后插入
    bool node_delete(double_link_node<T> *p);//删除该前节点
    bool node_clear();//清空链表
    double_link_node<T> * get_first(){return first;}
    double_link_node<T> * get_last(){return last;}
    bool is_empty(){return length==0;}
    int get_length(){return length;}
private:
    double_link_node<T> *first;
    double_link_node<T> *last;
    int length;
};
template <class T>
double_link<T>::double_link()
{
    first = new double_link_node<T>;
    last = first;
    length = 0;
}
template <class T>
bool double_link<T>::lat_insert(double_link_node<T> *p, T member)
{
    // cout<<"lat_insert is working."<<endl;
    double_link_node<T> *dnode = new double_link_node<T>;
    // cout<<"dnode created."<<endl;
    dnode->data = member;
    dnode->next = p->next;
    p->next = dnode;
    dnode->prev = p;
    if(dnode->next) dnode->next->prev = dnode;
    length++;
    // cout<<"insert succeed."<<endl;
    return 1;
}
template <class T>
bool double_link<T>::pre_insert(double_link_node<T> *p, T member)
{
    if (p == first)
    {
        cout << "insert error!!!" << endl;
        return 0;
    }
    double_link_node<T> *dnode = new double_link_node<T>;
    dnode->data = member;
    dnode->prev = p->prev;
    p->prev = dnode;
    dnode->next = p;
    dnode->prev->next = dnode;
    length++;
    return 1;
}
template<class T>
bool double_link<T>::push_back(T member)
{
    // cout<<"push_back is working"<<endl;
    // if(lat_insert(last,member)) cout<<"lat_insert success."<<endl;
    lat_insert(last,member);
    last=last->next;
    return 1;
}
template<class T>
bool double_link<T>::node_delete(double_link_node<T> *p)
{
    if(p==first)
    {
        if(length==0)
        {
            delete p;
            return 0;
        }
        else {first=first->next;delete p;return 1;}
    }
    if(p==last) last=last->prev;
    p->prev->next=p->next;
    p->next->prev=p->prev;
    delete p;
    length--;
    return 1;
}
template<class T>
bool double_link<T>::node_clear()
{
    while(!is_empty())
        node_delete(last);
    return 1;
}