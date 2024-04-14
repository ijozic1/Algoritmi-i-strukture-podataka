#include <iostream>
#include <cmath>
#include <vector>

int F(std::vector<int> &v, int a){
    std::cout<<"Pozvana rekurzija a, v.size() "<<a<<" "<<" "<<v.size()<<std::endl;
    if(a==1) {
        std::cout<<"Izlazim: a, v.size() "<<a<<" "<<" "<<v.size()<<std::endl;
        return 1;
    }
    int s=1+F(v, a/2)+F(v, a/2);
    v.push_back(s);
    return s;
}

int main(){
    std::vector<int>v;
    int a=9;
    F(v,a);
    for(int i=0; i<v.size(); i++) std::cout<<v[i]<<" ";
    return 0;
}