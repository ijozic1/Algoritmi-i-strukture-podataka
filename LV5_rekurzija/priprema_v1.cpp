#include <iostream>
#include <stdexcept>

using std::cout;

int fib2_0(int n, int a=0, int b=1){
    if(n==0) return a;
    if (n==1) return b; 
    return fib2_0(n-1,b,a+b);
}

int nzd(int x, int y){
    if(x==y) return x;
    if(x==1 || y==1) return 1;
    if(y==0) return x;
    return nzd(y, x % y);
}

int main(){
    for(int i=0; i<10; i++)
        std::cout<<i<<". Fib. broj je: "<<fib2_0(i)<<std::endl;
    std::cout<<"\n20. Fib. broj je: "<<fib2_0(20);
    std::cout<<"\nNZD brojeva 456 i 28 je: "<<nzd(456,28);
    std::cout<<"\nNZD brojeva 101 i 12 je: "<<nzd(101,12);
    std::cout<<"\nNZD brojeva 6 i 561 je: "<< nzd(6, 561);
    return 0;
}