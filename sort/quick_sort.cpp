// 快排

#include<iostream>
using namespace std;
int q[10000];
int count = 0;
void quick_sort(int q[],int l,int r)
{
    int x=q[l];
    int i=l,j=r;
    if(i>=j) return;
    while(i<j)
    {
        while(q[i]<x) i++;
        while(q[j]>x) j--;
        if(i<j) swap(q[i],q[j]);
    }
    cout<<"第"<<(++count)<<"趟: "<<endl;
    for(int i=0;i<9;i++)
    {
        cout<<q[i]<<" ";
    }
    cout<<endl;
    quick_sort(q,l,j);quick_sort(q,j+1,r);
}
int main()
{
    int n;scanf("%d",&n);
    for (int i=0;i<n;i++)
    {
       scanf("%d",&q[i]); 
    }
     quick_sort(q,0,n-1);
    for(int i=0;i<n;i++)
    {
        printf("%d ",q[i]);
    }
}