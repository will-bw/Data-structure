#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<vector>
#include<algorithm>
#include<bitset>
#define MAX_LEN 521
using namespace std;
struct huf_node//树节点
{
    char ch='\0';//字符
    int pow=0;//权值
    huf_node* father=NULL;//父节点
    huf_node* l_child=NULL;//左孩子
    huf_node* r_child=NULL;//右孩子
};
class str_stack//简单字符栈
{
public:
    str_stack(){top=-1;}
    void stack_clear(){top=-1;}
    bool is_empty(){return top==-1;}
    string str_copy();//赋值
    char pop(){top--;return str[top+1];}
    void push(char ch){top++;str[top]=ch;}
private:
    char str[MAX_LEN];
    int top;
};
class char_data//节点数组(采用队)
{
public:
    char_data(){front=0;tail=-1;}
    void node_push(huf_node nod);//新增节点（入队）
    void node_delete();//从开始删除一个节点（出队）
    void node_clear();
    bool is_empty(){return tail==-1;}//判断是否为空
    int length(){return tail==-1?0:tail-front+1;}
    friend class huf_tree;
private:
    huf_node nodes[MAX_LEN];
    int front;
    int tail;
};
class huf_tree
{
public:
    huf_tree(){}
    void Huf_clear();//清空
    void text_load();//加载文本
    string str_01_load();//加载编码
    void char_data_make();//字符处理
    void huf_make();//构造哈夫曼树
    void get_key(str_stack& s,huf_node* first);//得到字符对应的01串
    void Code(string file,string dest);//对文本进行编码
    void decode(string file,string dest);//解码
private:
    char_data data;//字符组
    huf_node *head;//头结点
    map<char,string> key;//字符和对应的的编码
    map<char,int> m;//字符和出现的次数
    string file_for_coding_name;
    string dest_file;
};
// void MYswap(huf_node& a,huf_node& b)
// {
//     huf_node *c=new huf_node;
//     *c=a;
//     a=b;
//     b=*c;
//     delete c;
// }
bool cmp(huf_node a,huf_node b)
{
    if(a.pow==b.pow)
        return a.ch<b.ch;
    return a.pow<b.pow;
}
string str_stack::str_copy()
{
    string s;
    for(int i=0;i<=top;i++)
    {
        s+=str[i];
    }
    return s;
}
// void char_data::node_sort(int l,int r)//快排
// {
//     int x=nodes[l].pow;
//     int i=l,j=r;
//     if(i>=j) return;
//     while(i<j)
//     {
//         while(nodes[i].pow<x) i++;
//         while(nodes[j].pow>x) j--;
//         if(i<j) MYswap(nodes[i],nodes[j]);
//         node_sort(l,j);
//         node_sort(j+1,r);
//     }
// }
void char_data::node_push(huf_node nod)
{
    tail++;
    nodes[tail]=nod;
    sort(&nodes[front],&nodes[tail]+1,cmp);
}
void char_data::node_delete()
{
    front++;
}
void char_data::node_clear()
{
    front=0;
    tail=-1;
}

void huf_tree::Huf_clear()
{
    data.node_clear();
    key.clear();
    m.clear();
    head=NULL;
}
void huf_tree::text_load()
{
    fstream ff(file_for_coding_name);
    char ch;
    while(!ff.eof())
    {
        ff.read(&ch,1);
        if(ff.eof()) break;
        if(m.count(ch)) m[ch]++;
        else m[ch]=1;
    }
    ff.close();
}
void huf_tree::char_data_make()
{
    huf_node temp;
    for(auto itor:m)
    {
        temp.ch=itor.first;
        temp.pow=itor.second;
        data.node_push(temp);
    }
}
void huf_tree::huf_make()
{
    huf_node tmp;
    while(data.length()>1)
    {
        sort(&data.nodes[data.front],&data.nodes[data.tail]+1,cmp);
        tmp.pow=data.nodes[data.front].pow+data.nodes[data.front+1].pow;
        tmp.l_child=&data.nodes[data.front];
        tmp.r_child=&data.nodes[data.front+1];
        data.nodes[data.front].father=&tmp;
        data.nodes[data.front+1].father=&tmp;
        data.node_push(tmp);
        data.node_delete();
        data.node_delete();
        // cout<<"ok  ";
    }
    head=&data.nodes[data.front];    
}
void huf_tree::get_key(str_stack& s,huf_node* first)
{
    if((first->l_child==NULL)&&(first->r_child==NULL))
    {
        key[first->ch]=s.str_copy();
        if(first->father)
            s.pop();
        return;
    }
    s.push('0');
    get_key(s,first->l_child);//添加一个0往左走

    s.push('1');
    get_key(s,first->r_child);//添加一个1往右走

    s.pop();//左右遍历完，父节点直接pop就行
}
void huf_tree::Code(string file,string dest)
{
    file_for_coding_name=file;
    dest_file=dest;
    text_load();//加载字符
    char_data_make();//处理字符
    huf_make();//构造哈夫曼树
    str_stack s;
    get_key(s,head);//得到字符和01串的对应
    fstream in(file_for_coding_name);
    fstream out(dest_file,ios::out|ios::trunc|ios::binary);
    char ch;
    // if(out.is_open()) cout<<"file opened";
    // while(!in.eof())
    // {
    //     in.read(&ch,1);
    //     if(in.eof()) break;
    //     out<<key[ch];
    // }
    string strs;
    while(!in.eof())//得到01串
    {
        in.read(&ch,1);
        if(in.eof()) break;
        strs+=key[ch];
    }
    int i=0,num,len=strs.length();
    int str_size;
    if(len%7==0) str_size=len/7;
    else str_size=len/7+1;
    out<<m.size()<<" "<<(len%7)<<" "<<str_size;//存入m的大小和最后的位数，和01串转换后的字符数
    for(auto itor:m)//遍历m将字符和频率一起存到编码文件中方便解码
    {
        out<<" "<<itor.first<<" "<<itor.second;
    }
    out<<" ";
    //存入编码的内容
    while(i+7<len)
    {
        num=0;
        for(int j=i;j<7+i;j++)
        {
            num=num*2+strs[j]-'0';
        }
        ch= (char)(num+32);
        out<<ch;
        i+=7;
    }
    num=0;
    for(int j=i;j<len;j++)
    {
        num=num*2+strs[j]-'0';
    }
    ch=(char)(num+32);
    out<<ch;
    // int tt=ch;
    // bitset<7> foo(tt);
    // string str=foo.to_string();
    // for(int j=0;j<7,j++;)
    // {
    //     if(str[j]!=strs[j+i])
    //     {
    //         cout<<"shit";
    //         break;
    //     }
    // }
    in.close();
    out.close();
}
string huf_tree::str_01_load()
{
    fstream in(file_for_coding_name);
    //先构造m
    int m_size,k,n;
    in>>m_size>>k>>n;
    int pow;
    char ch;
    for(int i=0;i<m_size;i++)//构造m
    {
        in.read(&ch,1);
        in.read(&ch,1);
        in>>pow;
        m[ch]=pow;
    }
    //然后读入内容
    string str;
    int asc;
    // char pre_char=ch;
    in.read(&ch,1);//把空格读掉
    if(ch!=' ')
        cout<<"shit";
    string foostring;
    for(int i=0;i<n-1;i++)//处理前n-1位
    {
        in.read(&ch,1);
        asc=ch-32;
        bitset<7> foo(asc);
        foostring=foo.to_string();
        // for(int j=0;j<7;j++)
        // {
        //     if(temp_01_string[i*7+j]!=foostring[j])
        //     {
        //         cout<<"shit";
        //         break;
        //     }
        // }
        // pre_char=ch;
        str+=foostring;
        // int j=str.length()-1;
        // if(temp_01_string[j]!=str[j])
        //     cout<<"shit";
    }
    in.read(&ch,1);//处理最后一位
    asc=ch-32;
    bitset<7> foo(asc);
    string temp=foo.to_string();
    if(k>0) temp=temp.erase(0,7-k);
    str+=temp;
    // cout<<"ok";
    // int aaa=str.length();
    // int bbb=temp_01_string.length();
    // if(str!=temp_01_string)
    //     cout<<"shit";
    // int j=0;
    // for(;j<aaa;j++)
    // {
    //     if(str[j]!=temp_01_string[j])
    //     {
    //         cout<<"shit";
    //         break;
    //     }
    // }
    in.close();
    return str;
}
void huf_tree::decode(string file,string dest)
{
    file_for_coding_name=file;
    dest_file=dest;
    Huf_clear();//先清空
    string str=str_01_load();//得到01串
    char_data_make();//构造节点数组
    huf_make();//构造哈夫曼树
    fstream out(dest_file,ios::out|ios::trunc|ios::binary);
    huf_node* first;
    // str=temp_01_string;
    for(int i=0;i<str.size();)
    {
        first=head;
        while((first->l_child)||(first->r_child))
        {
            if(str[i]=='0') first=first->l_child;
            else first=first->r_child;
            i++;
        }
        out.put(first->ch);
    }
    out.close();
}

int main()
{
    huf_tree my_tree;
    my_tree.Code("chars.txt","code_dest.txt");
    cout<<"code successfully!"<<endl;
    my_tree.decode("code_dest.txt","decode_dest.txt");
    cout<<"decode successfully";
    return 0;
}