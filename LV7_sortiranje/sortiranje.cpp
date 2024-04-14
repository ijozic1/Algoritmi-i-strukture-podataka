#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <exception>

using namespace std;

void ucitaj(string imeDat, int*& niz, int& vel) {
    ifstream datoteka(imeDat);
    if (!datoteka) {
        datoteka.close();
        throw std::logic_error("Neuspjesno otvaranje datoteke\n");
    }
    int broj;
    vel = 0;

    while (datoteka >> broj) vel++;

    datoteka.clear();
    datoteka.seekg(0, ios::beg);

    niz = new int[vel];
    for (int i = 0; i < vel; i++)
        datoteka >> niz[i];

    datoteka.close();
}

template<typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for (int i = vel-1; i >0; i--) {
        for (int j = 1; j <=i; j++) {
            if (niz[j-1] > niz[j]) {
                swap(niz[j-1], niz[j]);
            }
        }
    }
}

template<typename Tip>
void selection_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        Tip min=niz[i];
        int min_index = i;
        for (int j = i + 1; j < vel; j++) {
            if (niz[j] < min) {
                min_index = j;
                min=niz[j];
            }
        }
        swap(niz[i], niz[min_index]);
    }
}

template<typename Tip>
void insertionSort(Tip*niz, int vel){
    for(int i=1; i<vel; i++){
        Tip temp=niz[i];
        int j=i-1;
        while(j>=0 && niz[j]>temp){
            niz[j+1]=niz[j];
            j--;
        }
        niz[j+1]=temp;
    }
}

template<typename Tip>
void shellSort(Tip* niz, int vel, int *H, int velH){
    for(int i=0; i<velH; i++){
        int h=H[i];
        for(int j=h; j<vel; j++){
            Tip temp=niz[j];
            int k=j-h;
            while(k>=0 && niz[k]>temp){
                niz[k+h]=niz[k];
                k=k-h;
            }
            niz[k+h]=temp;
        }
    }
}

template<typename Tip>
void shell_Sort(Tip* niz, int vel, int h){
    int H[100], velH=0;
    while(h>0){ 
        H[velH++]=h;
        h/=2;
    }
    shellSort(niz,vel,H,velH);
}

template<typename Tip>
void countingSort(Tip *niz, int vel,Tip *nizB, int k){
    int *nizC=new int[k+1];
    for(int i=0; i<=k; i++) nizC[i]=0;
    for(int j=0; j<vel; j++) nizC[niz[j]]=nizC[niz[j]]+1;
    for(int i=1; i<=k; i++) nizC[i]=nizC[i]+nizC[i-1];
    for(int j=vel-1; j>=0; j--){
        nizC[niz[j]]=nizC[niz[j]]-1;
        nizB[nizC[niz[j]]]=niz[j];
    }
    delete[] nizC;
}

template<typename Tip>
void Zamijeni(Tip* niz, int i, int j) {
    Tip temp=niz[i];
    niz[i]=niz[j];
    niz[j]=temp;
}

template<typename Tip>
int Particija(Tip* niz, int prvi, int zadnji) {
    Tip pivot=niz[prvi];
    int p=prvi+1;
    while(p<=zadnji){
        if(niz[p]<=pivot) p++;
        else break;
    }
    for(int i=p+1; i<=zadnji; i++){
        if(niz[i]<pivot){
            Zamijeni(niz,p, i);
            p++;
        }
    }
    Zamijeni(niz, prvi, p-1);
    return p-1;
}

template<typename Tip>
void quickSort(Tip* niz, int prvi, int zadnji) {
    if (prvi < zadnji) {
        int pivot=Particija(niz, prvi, zadnji);
        quickSort(niz, prvi, pivot - 1);
        quickSort(niz, pivot + 1, zadnji);
    }
}

template<typename Tip>
void quick_sort(Tip* niz, int vel) {
    quickSort(niz, 0, vel - 1);
}

template<typename Tip>
void merge(Tip* niz, int l, int p, int q, int u) {
    int i=0, j=q-l, k=l;
    Tip *B=new Tip[u-l+1];

    for(int m=0; m<=u-l; m++)
        B[m]=niz[l+m];
    while(i<=p-l && j<=u-l){
        if(B[i]<B[j]) niz[k]=B[i++];
        else niz[k]=B[j++];
        k++;
    }
    while(i<=p-l) niz[k++]=B[i++];
    while(j<=u-l) niz[k++]=B[j++];
    delete[] B;
}

template<typename Tip>
void mergeSort(Tip* niz, int l, int u) {
    if (l < u) {
        int p=(l+u-1)/2;
        int q=p+1;
        mergeSort(niz, l, p);
        mergeSort(niz, q, u);
        merge(niz, l, p, q,u);
    }
}

template<typename Tip>
void merge_sort(Tip* niz, int vel) {
    mergeSort(niz, 0, vel-1);
}

void generisi(string imeDat, int vel) {
    ofstream datoteka(imeDat);
    if (!datoteka) {
        datoteka.close();
        throw std::logic_error("Neuspjesno otvaranje datoteke za pisanje\n");
    }

    srand(time(0));
    for (int i = 0; i < vel; i++)
        datoteka << rand() % 100 << " ";
    
    datoteka.close();
}

template<typename Tip>
bool je_sortiran(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        if (niz[i] > niz[i + 1]) {
            return false;
        }
    }
    return true;
}

template<typename Tip>
void ispisi(string imeDat, const Tip* niz, int vel) {
    ofstream datoteka(imeDat);

    if (!datoteka) {
        datoteka.close();
        throw std::logic_error("Neuspjesno otvaranje datoteke za pisanje\n");
    }

    for (int i = 0; i < vel; i++) 
        datoteka << niz[i] << " ";

    datoteka.close();
}

int main() {
    string ulazna_datoteka = "ulaz.txt";
    string izlazna_datoteka = "izlaz.txt";
    int* niz;
    int vel=10;
    try{
        generisi(ulazna_datoteka,vel);
        ucitaj(ulazna_datoteka, niz, vel);

        int izbor;
        cout<<"Ako zelite ponovno generisato datoteku koja ce biti sortirana unesite 1: ";
        cin>>izbor;
        if(izbor==1){
            generisi(ulazna_datoteka,vel);
            ucitaj(ulazna_datoteka, niz, vel);
        }
        cout << "\nOdaberite algoritam sortiranja:" << endl;
        cout << "1. Bubble Sort\n2. Selection Sort\n3. Quick Sort\n4. Merge Sort\n5. Insertion sort\n6. Shell sort\n7. Counting sort\n" << endl;
        cin >> izbor;

        clock_t pocetak, kraj;
        pocetak = clock();
        switch (izbor) {
            case 1:
                bubble_sort(niz, vel);
                break;
            case 2:
                selection_sort(niz, vel);
                break;
            case 3:
                quick_sort(niz, vel);
                break;
            case 4:
                merge_sort(niz, vel);
                break;
            case 5:
                insertionSort(niz,vel);
                break;
            case 6:{
                cout<<"Unesite pocetni razmak: ";
                int h=0;
                cin>>h;
                shell_Sort(niz, vel,h);
                break;
            }
            case 7:{
                int max=niz[0];
                for(int i=0; i<vel; i++)
                    if(niz[i]>max) max=niz[i];
                int nizB[10];
                countingSort(niz, vel,nizB,max);
                niz=nizB;
                break;
            }
            default:
                cout << "Pogresan izbor algoritma." << endl;
                return -1;
        }
        kraj = clock();

        double vrijeme = (double)(kraj - pocetak) / CLOCKS_PER_SEC * 1000;
        cout << "Vrijeme izvrsavanja: " << vrijeme << " ms" << endl;

        if (je_sortiran(niz, vel))
            cout << "Niz je uspjesno sortiran." << endl;
        else
            cout << "Niz nije ispravno sortiran." << endl;

        ispisi("izlaz.txt", niz, vel);
    }
    catch(std::exception &e){
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}