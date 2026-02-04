#include<iostream>
using namespace std;

void checkloss(int **a)
{
    cout<<"Hello"<<endl;
 //   delete[] a;
 //   a= NULL;
}


int main()
{

    int **a = new int*[10];
    for(int i=0;i<10;i++)
    {
        a[i] = new int[5];
    }
    checkloss(a);

    for(int i=0;i<10;i++)
        delete[] a[i];


    delete[] a;
    a= NULL;


}
