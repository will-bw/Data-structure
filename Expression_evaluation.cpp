#include<iostream>
#include<string>
#define N 100
using namespace std;
template <class T>
class M_stack
{
public:
    M_stack(int x){MAX=x;p=new T[MAX];top=-1;}
    ~M_stack(){delete p;}
    int push(T a);
    int pop(T& a);
    bool is_empty(){ return top==-1;}
    T get_top();
private:
    int MAX;//最大容量
    int top;//栈顶元素
    T* p; 
};
template <class T>
T M_stack<T>::get_top()
{
    // if(top==-1) return NULL;
    // return p[top];
    if(top==-1)
    {
        cout<<"error top!!!"<<endl;
        return 0;
    }
    return p[top];
}
template <class T>
int M_stack<T>::push(T a)
{
    if(top>=MAX-1) return -1;
    p[++top]=a;
    cout<<a<<"    "<<"push in"<<endl;
    return 0;
}
template <class T>
int M_stack<T>::pop(T& a)
{
    if(top==-1) return -1;
    cout<<get_top()<<"    "<<"pop out"<<endl;
    a=p[top--];
    return 0;
}
bool is_digit(char ch)
{
    return (ch>='0'&&ch<='9');
}
int value(char c)//优先级计算
{
    if(c=='(') return 0;
    else if(c=='+'||c=='-') return 1;
    else return 2;
}
int calculation(int a,int b,char c)//计算操作
{
    if(c=='+') return a+b;
    else if(c=='-') return a-b;
    else if(c=='*') return a*b;
    else return a/b;
}
int Result(string s)//求最终结果
{
    int a,b;
    bool status_flag=1;
    //状态标志位,是否为左括号或者是第一个字符
    int pos_flag=1;//正负标志位
    char ch;
    M_stack<int> number(N);//数字栈
    M_stack<char> character(N);//字符栈
    for(int i=0;i<s.size();i++)
    {
        if(is_digit(s[i])) //数字直接入数字栈
        {
            a=0;
            while(is_digit(s[i]))
                a=a*10+s[i++]-'0';
            number.push(a*pos_flag);
            pos_flag=1;//数字入栈后立马把正负标志位拨正；
            status_flag=0;//数字，状态标志位置为0
        }
        if(s[i]=='(') {character.push(s[i]);status_flag=1;}
        //左括号直接入字符栈,状态标志置为1
        if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/')//运算符把优先级高于他的都弹出来操作完再入栈
        {
                if(s[i]=='-'&&status_flag)//如果出现减号且状态标志位为1，那么这个减号是负号,不入字符栈，状态标志位不变
                {
                    pos_flag=-1;//正负标志位置为-1;
                    continue;
                }
                //while(value(s[i])<=value(character.get_top())&&!character.is_empty())
                //特别注意在这里，是否为空的判断要放在前面不然会发生空的还去取top的情况
                while(!character.is_empty()&&(value(s[i])<=value(character.get_top())))
                {
                    number.pop(a);
                    number.pop(b);
                    character.pop(ch);
                    number.push(calculation(b,a,ch));
                }
                character.push(s[i]);
                status_flag=0;//字符，状态标志位置为0
        }
        if(s[i]==')')//后括号就把两个括号之间的操作符都弹出来计算完
        {
            while(character.get_top()!='(')
            {
                number.pop(a);
                    number.pop(b);
                    character.pop(ch);
                    number.push(calculation(b,a,ch));
            }
            character.pop(ch);//计算完把左括号弹掉                
            status_flag=0;//后括号，状态标志位置为0
        }
    }
    //最后把栈中剩下的数据做最后处理
    while(!character.is_empty())
    {
         number.pop(a);
        number.pop(b);
        character.pop(ch);
        number.push(calculation(b,a,ch));
    }
    return number.get_top();
}
int main()
{
    string sourse;
    getline(cin,sourse);//读入一行
    cout<<sourse<<" = "<<Result(sourse);
    return 0;
}
/*测试案例
12+(21+69)*2-(65-25)/10
结果188
-12  +(-21+69)* 2 - (65-25 )/10
结果80
*/
