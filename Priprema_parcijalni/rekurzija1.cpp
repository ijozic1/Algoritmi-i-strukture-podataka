#include <iostream>
#include <cmath>
#include <vector>

int F(std::vector<int> &v, int a, int &b){
    std::cout<<"Pozvana rekurzija a, b, v.size()"<<a<<" "<<b<<" "<<v.size()<<std::endl;
    if(a<=0 || b<=2) {
        std::cout<<"Izlazim: a, b, v.size() "<<a<<" "<<b<<" "<<v.size()<<std::endl;
        return a;
    }
    b-=1;
    a=F(v,a-1,b)+F(v,a,b);
    v.push_back(a);
    return a;
}

int main(){
    std::vector<int>v;
    int b=7;
    F(v,15,b);
    for(int i=0; i<v.size(); i++) std::cout<<v[i]<<" ";
    return 0;
}