#include"text_editor.hpp"
using namespace std;
int main()//进行测试
{
    Editor edit("test.txt");
    string s1=" s1 is inserted at the cursor.";
    string s2=" s2 is inserted at pos.";
    string s3=" s3 is inserted after the third row as a new line.";
    edit.show();//显示编辑器所有内容
    cout<<"the number of rows is "<<edit.get_link_length()<<endl;
    edit.text_insert(s1);//在当前光标（末尾）插入是s1
    position pos{1,23};
    edit.text_insert(pos,s2);//在pos位置插入s2
    edit.text_insert(3,s3);//在第三行后面插入s3
    cout<<endl<<"after insert:"<<endl;
    edit.show();
    position pos2{2,4};
    edit.text_delete(pos2,10);//删除从pos2开始前面5个字符
    edit.text_delete_line(4);//删除第四行
    cout<<endl<<"after delelte:"<<endl;
    edit.show();
    cout<<endl<<"the result of seek:"<<endl;
    position pos3{4,3};
    cout<<edit.text_seek(pos3,"on")<<endl;//从pos3开始查找hat出现的位置
    cout<<edit.text_seek(pos3,"good")<<endl;//如果找不到就返回位置（-1，-1）
    cout<<endl<<"after change:"<<endl;
    edit.change("h","H");//把h换为H
    edit.change("e","E");//把e换为E
    edit.show();
    return 0;
}
/*
测试结果：
my name is wangbangwen.
this is my second experiment,
and 
it's name is text editor.

To be honest, I do not know why the teacher said that this experiment was easier than the last time. 
Perhaps my misunderstanding made me take two days longer than the previous experiment. 
It is 1:45 AM on December 30, 2021. 
My roommate has fallen asleep and is snoring,
which makes me irritable because of the bug even worse .       
and I am very hungry but I can not go.  
The current situation is that I have just fixed the last bug and still do not know why. 
The bug will be fixed. I knew it early, but what I did not  know was that it would be fixed in the middle of the night.       
 All in all, I am a vegetable chicken .
the number of rows is 14

after insert:
my name is wangbangwen. s2 is inserted at pos.
this is my second experiment,
and 
 s3 is inserted after the third row as a new line.
it's name is text editor.

To be honest, I do not know why the teacher said that this experiment was easier than the last time. 
Perhaps my misunderstanding made me take two days longer than the previous experiment.
It is 1:45 AM on December 30, 2021.
My roommate has fallen asleep and is snoring,
which makes me irritable because of the bug even worse .       
and I am very hungry but I can not go.
The current situation is that I have just fixed the last bug and still do not know why.
The bug will be fixed. I knew it early, but what I did not  know was that it would be fixed in the middle of the night.       
 All in all, I am a vegetable chicken . s1 is inserted at the cursor.

after delelte:
my name is wangbangwen. s2 is inserted a
 is my second experiment,
and
it's name is text editor.

To be honest, I do not know why the teacher said that this experiment was easier than the last time.
Perhaps my misunderstanding made me take two days longer than the previous experiment.
It is 1:45 AM on December 30, 2021.
My roommate has fallen asleep and is snoring,
which makes me irritable because of the bug even worse .
and I am very hungry but I can not go.
The current situation is that I have just fixed the last bug and still do not know why.
The bug will be fixed. I knew it early, but what I did not  know was that it would be fixed in the middle of the night.
 All in all, I am a vegetable chicken . s1 is inserted at the cursor.

the result of seek:
(6,8)
(-1,-1)

after change:
my namE is wangbangwEn. s2 is insErtEd a
 is my sEcond ExpErimEnt,
and
it's namE is tExt Editor.

To bE HonEst, I do not know wHy tHE tEacHEr said tHat tHis ExpErimEnt was EasiEr tHan tHE last timE.
PErHaps my misundErstanding madE mE takE two days longEr tHan tHE prEvious ExpErimEnt.
It is 1:45 AM on DEcEmbEr 30, 2021.
My roommatE Has fallEn aslEEp and is snoring,
wHicH makEs mE irritablE bEcausE of tHE bug EvEn worsE .
and I am vEry Hungry but I can not go.
THE currEnt situation is tHat I HavE just fixEd tHE last bug and still do not know wHy.
THE bug will bE fixEd. I knEw it Early, but wHat I did not  know was tHat it would bE fixEd in tHE middlE of tHE nigHt.
 All in all, I am a vEgEtablE cHickEn . s1 is insErtEd at tHE cursor.
 */