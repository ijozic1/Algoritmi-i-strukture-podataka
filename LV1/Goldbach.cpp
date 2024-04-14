#include <iostream>
#include <cmath>
#include <stdexcept>
#include <ctime>

bool DaLiJeProst(int &p){
    if(p%2==0) return false;
    for(int i=3; i<=std::sqrt(p); i+=2)
        if(p%i==0) return false;
    return true;
}

void Goldbach(int n, int &p, int &q){
    if(n%2!=0 || n<=2) throw std::logic_error("Neispravno n");
    if(p+q==n) return;
    q=n-p;
    if(DaLiJeProst(q)) return;
    while(q>2){
        p++; q--;
        if(DaLiJeProst(p) && DaLiJeProst(q)) return; //da li bi bilo razlike da je ovdje break??
    }
}

int main (){
    std::cout<<"Unesite broj n: ";
    int n=0, p=2, q=2;
    std::cin>>n;
    clock_t vrijeme1 = clock();
    try{
        Goldbach(n,p,q);
        std::cout<<n<<"="<<p<<"+"<<q<<std::endl                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          ;
    }
    catch(std::logic_error &izuzetak){ 
        std::cout<<izuzetak.what()<<std::endl;
    }

    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000); //bilo 1000 pa je mili
    std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " mikro s." << std::endl;

    return 0;
}