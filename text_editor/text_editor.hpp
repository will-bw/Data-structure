#pragma once

#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include"double_link.hpp"
using namespace std;
struct position{int line,off;};
class Editor
{
public:
    Editor(string file);//构造
    ~Editor();//析构
    void file_load();//把文件载入编辑器
    void file_update();//把更新写回文件
    bool move_cursor(position pos);//把光标移到pos处
    int get_line();//得到当前光标所在行的标号
    int get_offset();//得到当前偏移量
    bool text_insert(string s);//在当前光标后面添加串s
    bool text_insert(int line,string s);//在第line行后面插入一行串s
    bool text_insert(position pos,string s);//在pos位置后面插入串s
    bool text_delete(int num);//删除当前光标前面num个字符
    bool text_delete(position pos,int num);//删除pos位置前面num个字符
    bool text_delete_line(int num);//删除第num行
    position& text_seek(position pos,string s);//在第pos后面查找串s,返回首位置
    void change(string s,string t);//查找文本中的s并替换为t；
    void show();//打印当前所有内容
    position& get_cursor();//得到当前光标位置
    int get_link_length(){return lines_data.get_length();}//获取文件行数
private:
    string filename;
    double_link<string> lines_data;//编辑器存储文本的空间
    fstream ff;//文件
    double_link_node<string> *cursor;//光标行位置
    int offset;//光标在该行的偏移值 
};
void get_next(string s,int *next)//求kmp算法中的next数组
{
    next[0]=-1;//next[0]置为-1
    next[1]=0;//next[1]肯定为0;
    for(int i=2;i<s.size();i++)//遍历得到每个next[i]
    {
        int max_len=i-1;
        int len,j;
        for(len=max_len;len>=1;len--)//从最长开始遍历得到值
        {
            for(j=0;j<len;j++)
            {
                if(s[j]!=s[i-len+j])//如果有不匹配的值就中断
                break;
            }
            if(len==j) {next[i]=len;break;}//如果匹配完成则next[i]=len
        }
        if(len<1) next[i]=0;//如果len==1都不满足那么一定是0
    }
}
fstream& operator>>(fstream& file,double_link<string>& lines)//重载操作符>>
{
    string s;
    while(!file.eof())//将文件内容全部载入链表
    {
        getline(file,s);
        // cout<<s;
        lines.push_back(s);
        // cout<<"one line is loaded."<<endl;
    }
    // cout<<"loading ended."<<endl;
    return file;
}
fstream& operator<<(fstream& file,double_link<string>& lines)//重载操作符<<
{
    double_link_node<string> *p=lines.get_first()->next;
    while(p)
    {
        if(p==lines.get_last()) file<<p->data;
        else file<<p->data<<endl;
        p=p->next;
    }
    return file;
}
ostream& operator<<(ostream& out,position &pos)
{
    out<<"("<<pos.line<<","<<pos.off<<")";
    return out;
}
int Editor::get_line()
{
    // cout<<"get_line is working."<<endl;
    int j=0;
    double_link_node<string>* p=lines_data.get_first();
    while(p!=cursor)
    {
        p=p->next;
        j++;
    }
    return j;
}
position& Editor::get_cursor()
{
    position* po=new position;
    po->line=get_line();
    po->off=offset;
    return *po;
}
void Editor::file_load()
{
    // cout<<"file_load is working."<<endl;
    if(ff.is_open())
        ff>>lines_data;
    else
    {
        cout<<"file open error!!!"<<endl;
        exit(0);
    }
}
Editor::Editor(string file)
{
    filename=file;
    ff.open(filename);
    // if(ff.is_open()) 
    //     cout<<"successfully opened!"<<endl;
    file_load();//载入文本
    cursor=lines_data.get_last();//光标行位置初始化在最后一行
    offset=cursor->data.size();//偏移在最后一个字符
    ff.close();
}
void Editor::file_update()
{
    // cout<<"file_update is working!"<<endl;
    ff.open(filename,ios::out|ios::trunc);
    ff<<lines_data;
    ff.close();
}
Editor::~Editor()
{
    file_update();
    // cout<<"~Editor is working."<<endl;
    lines_data.node_clear();
}
bool Editor::move_cursor(position pos)
{
    if(pos.line>lines_data.get_length())
    {
        cout<<"the line overflow!!!"<<endl;
        exit(0);
    } 
    cursor=lines_data.get_first();
    for(int i=0;i<pos.line;i++)
        cursor=cursor->next;
    if(cursor->data.size()<pos.off)
    {
        cout<<"the offset overflow!!!"<<endl;
        exit(0);
    }
    offset=pos.off;
    return 1;
}
int Editor::get_offset(){return offset;}
bool Editor::text_insert(string s)
{
    cursor->data.insert(offset,s);
    offset+=s.size();
    return 1;
}
bool Editor::text_insert(int line,string s)
{
    
    if(line>lines_data.get_length())
    {
        cout<<"the line overflow!!!"<<endl;
        exit(0);
    }
    double_link_node<string>* p=lines_data.get_first();
    for(int i=0;i<line;i++)
        p=p->next;
    lines_data.lat_insert(p,s);
    cursor=p->next;
    offset=s.size();//每次操作完都把光标移到相应位置
    return 1;
}
bool Editor::text_insert(position pos,string s)
{
    move_cursor(pos);
    text_insert(s);
    offset+=s.size();
    return 1;
}
bool Editor::text_delete(int num)
{
    double_link_node<string>* p;
    while(num>offset)
    {
        cursor->data.erase(0,offset);
        num-=offset;
        p=cursor;
        cursor=cursor->prev;
        offset=cursor->data.size();
        if(p->data.size()==0) lines_data.node_delete(p);
    }
    if(num)
    {
        cursor->data.erase(offset-num,num);
        offset-=num;
        if(cursor->data.size()==0)//如果该行删为空了，则删除该节点
        {
            p=cursor;
            cursor=cursor->prev;
            offset=cursor->data.size();
            lines_data.node_delete(p);
        }
    }
    return 1;
}
bool Editor::text_delete(position pos,int num)
{
    move_cursor(pos);
    text_delete(num);
    return 1;
}
bool Editor::text_delete_line(int num)
{
    double_link_node<string>* p=lines_data.get_first();
    for(int i=0;i<num;i++)
        p=p->next;
    lines_data.node_delete(p);
    return 1;
}
position& Editor::text_seek(position pos,string s)
/*现在是2021年10月29日北京时间23:05.
保留以下注释掉的一堆cout语句不删，以此纪念我没有用dbg
在这个函数耗费整整两天的debug时光
虽然现在我还是不理解
为什么第二个while语句判断条件里的字符串长度提前用一个int变量过度就可以了
而直接用.size()就不行*/
{
    position* po=new position;
    po->line=-1;
    po->off=-1;
    move_cursor(pos);//先把光标移过去
    int *s_next=new int[s.size()];
    get_next(s,s_next);//求出s的next函数
    // cout<<"the next :"<<endl;
    // for(int i=0;i<s.size();i++)
    //     cout<<s_next[i]<<",";
        // cout<<"the cursor:\n";
        // cout<<get_cursor();
    while(cursor!=NULL)
    {
        // string temp_s=cursor->data;
        // cout<<temp_s<<endl;
        int i=offset-1,j=0;
        int len1=cursor->data.size();
        int len2=s.size();
        // cout<<len1<<","<<len2;
        // while((i<len1)&&(j<len2))
        while((i<(int)(cursor->data.size()))&&(j<(int)(s.size())))
        /*现在是2021年10月30日9:59
        破案了，size函数返回的是无符号数，j更新为-1的时候，跟无符号数一比较就出问题
        强转int就可以了*/
        {   
/*          cout<<"the start i:"<<i<<endl;
            cout<<"the start j:"<<j<<endl;
            cout<<temp_s[i]<<","<<s[j]<<endl; */

            if((j==-1)||(cursor->data[i]==s[j]))  {i++;j++;}
            else   {j=s_next[j];}
            // if(cursor->data.length()>4||s.length()>2)
            //     cout<<"shit";
        }
/*         if((i<temp_s.size())&&(j<s.size()))
        {
            cout<<"why the while broke?"<<endl;
        } */
/*         cout<<"the last i:"<<i<<endl;
        // cout<<"temp_s.size():"<<temp_s.size()<<endl;
        cout<<"the last j:"<<j<<endl;
        // cout<<"s.size():"<<s.size()<<endl; */
        if(j==s.size())
        {
            // cout<<"I look it!!!! It's in the line "<<get_line()<<" and offset"<<i-j+1<<endl;
            po->off=i-j+1;
            po->line=get_line();
            return *po;
        }
        else
        {
            // cout<<"cursor=next\n";
            cursor=cursor->next;
            offset=1;
            continue;
        }
    }
    return *po;
}
void Editor::show()
{
    // cout<<"show is working."<<endl;
    double_link_node<string> *p;
    p=lines_data.get_first()->next;
    while(p)
    {
        cout<<p->data<<endl;
        p=p->next;
    }
}
void Editor::change(string s,string t)
{
    position po;
    po.line=1;
    po.off=1;
    while(1)
    {
        po=text_seek(po,s);
        if(po.line==-1) break;
        move_cursor(po);
        cursor->data=cursor->data.replace(po.off-1,t.size(),t);
        po.off++;
        if(po.off>cursor->data.size())
        {
            po.line++;
            cursor=cursor->next;
            offset=1;
            po.off=1;
        }
    }
}