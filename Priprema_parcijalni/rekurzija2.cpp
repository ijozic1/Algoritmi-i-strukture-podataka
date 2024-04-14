#include <iostream>
#include <cmath>
#include <vector>

int F(std::vector<int> &v, int &a){
    std::cout<<"Pozvana rekurzija a, v.size() "<<a<<" "<<" "<<v.size()<<std::endl;
    if(a<=0) {
        std::cout<<"Izlazim: a, v.size() "<<a<<" "<<" "<<v.size()<<std::endl;
        return 0;
    }
    a-=1;
    a=F(v,a);
    v.push_back(a);
    return a-1;
}

int main(){
    std::vector<int>v;
    int a=4;
    F(v,a);
    for(int i=0; i<v.size(); i++) std::cout<<v[i]<<" ";
    return 0;
}