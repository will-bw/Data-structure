#include<iostream>
#include<string>
using namespace std;
void get_next(string s,int *next)
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
int main()
{
    string s="ABCABAA";
    int a[7];
    get_next(s,a);
    for(int i=0;i<7;i++)
        cout<<a[i]<<" ";
    return 0;
}