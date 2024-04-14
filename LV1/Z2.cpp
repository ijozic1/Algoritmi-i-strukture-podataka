// Koja je vremenska kompleksnost sljedećeg algoritma?

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <ctime>

int main(){
    int s=0, n=0;
    std::cout<<"Unesite n: ";
    std::cin>>n;
    clock_t vrijeme1 = clock();
    for( int i = n; i > 0; i-=2) {
        for( int j = 0; j < 2*n; j++ ) {
            for( int k = 0;k < n; k++){
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

//vanjska petlja ide n/2 puta, prva sa j se izvrsava 2n puta i posljednja sa k se izvrsava n puta => n/2*2*n*n=O(n^3)