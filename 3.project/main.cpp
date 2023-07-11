#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
template<class T>
void fun(T b){
    printf("T\n");
}

template<>
void fun<int>(int b){
    printf("int\n");
}




template<class T>
class A{
    public:
        A(){
            cout << "int" << endl;
            return;
        }
        T data;
};
template<>
class A<int>{
    public:
        A(){
            cout << "int" << endl;
            return;
        }
        int data;
};

int main(){
    int a;
    fun(a);
    // find();
    return 0;
}
