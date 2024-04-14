//Koja je vremenska kompleksnost algoritma koji pronalazi sumu svih
//faktorijela od 1 do n (1!+2!+3!....n!)? Na koji način je moguće popraviti vremensku
//kompleksnost datog koda i kolika će vremenska kompleksnost biti nakon popravke?

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <ctime>

int main(){
    int s=0, fact=1, n=0;
    std::cout<<"Unesite n: ";
    std::cin>>n;
    clock_t vrijeme1 = clock();
    for(int i = 1; i <= n; i++){
        fact = 1;
        for (int j = 1; j <= i; j++){
            fact = fact * j;
        }
        s+=fact;
    }
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000); //bilo 1000 pa je mili
    std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " mikro s." << std::endl;
    std::cout<<"Suma je: "<<s;
    return 0;
}

//posto imamo ugnijezdenu petlju, trebalo bi da se vanjska izvrsava n puta, a unutrasnja n*(n+1)/2 puta, pa zanemarivanjem koeficijenaa dobijemo O(n^3)