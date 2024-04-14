#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int nadjiNajveciEl(std::vector<int> &a) {
    int maxElement = a[0], vel=a.size();
    for (int i = 1; i < vel; i++)
        if (a[i] > maxElement) maxElement = a[i];
    return maxElement;
}

//counting sort na osnovu zadane cifre
void countingSort(vector<int> &a, int cifra) {
    int vel = a.size();
    std::vector<int> b(vel), count(10, 0);

    //pojavljivanje cifara
    for (int i = 0; i < vel; i++) count[(a[i] / cifra) % 10]++;

    //suma
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    // izlazni vektor
    for (int i = vel - 1; i >= 0; i--) {
        b[count[(a[i] / cifra) % 10] - 1] = a[i];
        count[(a[i] / cifra) % 10]--;
    }

    std::swap(a,b);
}

//radix sort
void radixSort(std::vector<int> &a) {
    int max= nadjiNajveciEl(a);

    //counting sort za svaku cifru
    for (int cifra = 1; max / cifra > 0; cifra *= 10)
        countingSort(a, cifra);
}

// Pomocna funkcija za razmjenu elemenata na pozicijama i i j u vektoru
void swap(std::vector<int> &a, int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

//gomila
int roditelj(int i){
    return (i-1)/2;
}

int lijevoDijete(int i){
    return 2*i+1;
}

int desnoDijete(int i){
    return 2*i+2;
}

void popraviPremaDole(std::vector<int> &a, int i, int velicina) {
    while(!(i>=velicina/2 && i<velicina)){
        int veci = lijevoDijete(i), des = desnoDijete(i);

        if((des < velicina) && (a[des]>a[veci])) veci = des;

        if(a[i]>a[veci]) return;

        swap(a,i,veci);
        i = veci;
    }
}

void popraviPremaGore(std::vector <int> &a , int i){
    while((i!=0) && (a[i]>a[roditelj(i)])){
        swap(a,i,roditelj(i));
        i = roditelj(i);
    }

}

// Funkcija koja stvara maksimalnu gomilu od vektora
void stvoriGomilu(std::vector<int> &a) {
    int velicina = a.size();

    for (int i = std::floor((velicina/2.)); i >= 0; i--)
        popraviPremaDole(a, i, velicina);
}

//umetanje elementa u gomilu
void umetniUGomilu(std::vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    velicina++;
    popraviPremaGore(a,velicina-1);
}

// Funkcija koja izbacuje element iz korijena gomile
int izbaciPrvi(std::vector<int> &a, int &velicina) {
    if (a.size()==0)
        throw std::logic_error("Gomila je prazna.");

    velicina--;
    swap(a,0,velicina); 

    if(velicina!=0)popraviPremaDole(a, 0, velicina);
    return a[velicina];
}

// gomila sort
void gomilaSort(std::vector<int> &a) {
    int velicina = a.size();

    stvoriGomilu(a);

    // Redom izbacuj najveći element iz gomile i smještaj ga na kraj niza
    for (int i = velicina - 1; i > 0; i--) {
        swap(a, 0, i);
        popraviPremaDole(a, 0, i);
    }
}

// Ispis vektora
void ispisiVektor(const std::vector<int> &a) {
    for (int i = 0; i < a.size(); i++)
        std::cout << a[i] << " ";
    std::cout << std::endl;
}

//provjerava da li je vektor sortiran
bool je_sortiran(std::vector<int>&a) {
    int vel=a.size();
    for (int i = 0; i < vel - 1; i++)
        if (a[i] > a[i + 1]) return false;
    return true;
}

int main() {
    // Primjer upotrebe radix sort
    std::vector<int> v1;
    for (int i = 0; i < 20; i++)
        v1.push_back(rand() % 1000);
    
    std::cout << "Originalni vektor: ";
    ispisiVektor(v1);

    radixSort(v1);

    if (je_sortiran(v1))
        std::cout << "Vektor je uspjesno sortiran.\n" << std::endl;
    else
        std::cout << "Vektor nije ispravno sortiran.\n" << std::endl;
    

    std::cout << "Sortirani vektor (radix): ";
    ispisiVektor(v1);
    std::cout<<std::endl;

    v1.clear();

    //gomila
    for (int i = 0; i < 10; i++)
        v1.push_back(rand() % 1000);
    
    std::cout << "Originalni vektor: ";
    ispisiVektor(v1);

    stvoriGomilu(v1);

    std::cout << "Gomila: ";
    ispisiVektor(v1);

    // Dodaj element u gomilu
    int umetnuti = 8;
    int velicina = v1.size();
    umetniUGomilu(v1, umetnuti, velicina);

    std::cout << "Gomila nakon umetanja: ";
    ispisiVektor(v1);

    try{
        // Izbaci najveći element iz gomile
        int izbaceni = izbaciPrvi(v1, velicina);
        std::cout << "Izbaceni element: " << izbaceni << endl;
        std::cout << "Gomila nakon izbacivanja: ";
        ispisiVektor(v1);
    }
    catch(std::logic_error &e){
        std::cout<<e.what()<<std::endl;
    }

    // Gomila sort
    std::vector<int> v2;
    for (int i = 0; i < 20; i++)
        v2.push_back(rand() % 1000);
    
    cout << "Vektor prije sortiranja: ";
    ispisiVektor(v2);

    gomilaSort(v2);

    if (je_sortiran(v2))
        std::cout << "Vektor je uspjesno sortiran.\n" << std::endl;
    else
        std::cout << "Vektor nije ispravno sortiran.\n" << std::endl;

    std::cout << "Sortirani vektor (gomila): ";
    ispisiVektor(v2);

    return 0;
}