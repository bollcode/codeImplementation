#include <iostream>

#include "Shared_Ptr.h"

using namespace std;

int main(){
    int* a = new int(10);
    int* b = new int(20);
    {
        Share_Ptr<int> p1(a);
        Share_Ptr<int> p2(p1);
        Share_Ptr<int> p3;
        p3 = p2;
        cout << p1.useCount() << endl; // 3
        Share_Ptr<int> p4(b);
        p3 = p4;
        cout << p1.useCount() << endl; // 2
        cout << p4.useCount() << endl; // 2
    }
    
    // cout << p1.useCount() << endl;
    cout << *a << endl; // 出现不确定数，智能指针会自动的释放掉a指向的内存

    



}