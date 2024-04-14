//Koja je vremenska kompleksnost sljedećeg algoritma?

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <ctime>

int main(){
    int s=0, n=0;
    std::cout<<"Unesite n: ";
    std::cin>>n;
    clock_t vrijeme1 = clock();
    for( int i = n; i > 1; i /= 2 ) {
        for( int j = 1; j < n; j *= 2 ) {
            for( int k = 0; k < n; k += 2 ) {
                s+=k;
            }
        }
    }
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000); //bilo 1000 pa je mili
    std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " mikro s." << std::endl;
    std::cout<<"Suma je: "<<s;
    return 0;
}