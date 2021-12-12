#include <iostream>
using namespace std;
int q[1000];

void heap_make(int *q, int n, bool dir,bool & is_heap)
{
    for (int i = (n - 2) >> 1; i >= 0; i--)
    {
        for (int j = i, k = 2 * j + 1; k <= n - 1; k = 2 * k + 1)
        {
            if (dir)//为真转换大顶堆
            {
                if (k < n - 1)
                {
                    k = q[k] > q[k + 1] ? k : k + 1;
                }
                if (q[j] < q[k])
                {
                    int s = q[j];
                    q[j] = q[k];
                    q[k] = s;
                    is_heap=0;
                    j=k;
                }
            }
            else{
                if (k < n - 1)
                {
                    k = q[k] < q[k + 1] ? k : k + 1;
                }
                if (q[j] > q[k])
                {
                    int s = q[j];
                    q[j] = q[k];
                    q[k] = s;
                    is_heap=0;
                    j=k;
                }
            }
        }
    }
}
int main()
{
    int n;
    cin >> n;
    bool is_heap=1;
    for (int i = 0; i < n; i++)
    {
        cin >> q[i];
    }
    heap_make(q,n,1,is_heap);//转换大顶堆
    if(is_heap)
    {
        cout<<"是大顶堆，转换结果:"<<endl;
    }
    else {
        cout<<"不是大顶堆，转换结果:"<<endl;
    }
    for (int i = 0; i < n; i++)
    {
        cout << q[i] << " ";
    }
    cout<<endl;
    is_heap=1;
    heap_make(q,n,0,is_heap);//转换小顶堆
    if(is_heap)
    {
        cout<<"是小顶堆，转换结果:"<<endl;
    }
    else {
        cout<<"不是小顶堆，转换结果:"<<endl;
    }
    for (int i = 0; i < n; i++)
    {
        cout << q[i] << " ";
    }
    return 0;
}