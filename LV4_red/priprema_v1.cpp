#include <iostream>
#include <stdexcept>

#include "stek_jednostrukaLista.hpp"

using std::cout;

template<typename TipEl>
class Red{
    struct Cvor{
        TipEl element;
        Cvor* sljedeci;
    };
    Cvor *prvi, *posljednji;
    int brElemenata;
    bool daLiJePrazan(){
        return prvi==nullptr;
    }
  public:
    Red();
    ~Red(){
        this->brisi();
    }
    Red(const Red &r);
    Red &operator=(const Red & r);
    void stavi(const TipEl &el);
    TipEl skini();
    TipEl & celo();
    TipEl celo() const;
    void brisi(){
        while(!daLiJePrazan()) this->skini();
    }
    int brojElemenata(){
        return this->brElemenata;
    }
};

//implementacija metoda
template <typename TipEl>
Red<TipEl>::Red(){
    prvi=nullptr;
    posljednji=nullptr;
    brElemenata=0;
}

template <typename TipEl>
Red<TipEl>::Red(const Red &r){
    this->prvi=nullptr; 
    this->posljednji=nullptr; 
    this->brElemenata=0;
    Cvor *temp=r.prvi;
    while(temp!=nullptr){
        this->stavi(temp->element);
        temp=temp->sljedeci;
    }
    delete temp;
}
    
template <typename TipEl>
Red<TipEl> & Red<TipEl>::operator=(const Red & r){
    if(this==&r) return *this;
    this->brisi();
    Cvor *temp=r.prvi;
    while(temp!=nullptr){
        this->stavi(temp->element);
        temp=temp->sljedeci;
    }
    delete temp;
    return *this;
}

template<typename TipEl>
void Red<TipEl>::stavi(const TipEl &el){
    Cvor *novi=new Cvor;
    novi->element=el;
    novi->sljedeci=nullptr;
    if(daLiJePrazan()){ 
        prvi=novi;
        posljednji=novi;
    }
    else{
        posljednji->sljedeci=novi;
        posljednji=novi;
    }
    brElemenata++;
}

template<typename TipEl>
TipEl Red<TipEl>::skini(){
    if(daLiJePrazan())
        throw std::logic_error("Red je prazan1");
    Cvor *temp=prvi;
    prvi=prvi->sljedeci;
    TipEl vrijednost=temp->element;
    delete temp;
    if(prvi==nullptr) posljednji=nullptr;
    brElemenata--;
    return vrijednost;
}

template<typename TipEl>
TipEl & Red<TipEl>::celo(){
    if(daLiJePrazan())
        throw std::logic_error("Red je prazan2");
    return prvi->element;
}

template<typename TipEl>
TipEl Red<TipEl>::celo() const{
    if(daLiJePrazan())
        throw std::logic_error("Red je prazan3");
    return prvi->vrijednost;
} 

//test funckije
template<typename Tip>
void testStavi(Red<Tip> &r, const Tip &el) {
    r.stavi(el);
    std::cout << "Vrh reda: " << r.celo() << std::endl;
}

template<typename Tip>
void testSkini(Red<Tip> &r) {
    std::cout << "Vrh reda: " << r.celo() << std::endl;
    r.skini();
    std::cout << "Vrh reda nakon skidanja prvog: " << r.celo() << std::endl;
}

template<typename Tip>
void testBrisi(Red<Tip> &r) {
    std::cout << "Broj elemenata u redu prije brisanja: " << r.brojElemenata() << std::endl;
    r.brisi();
    std::cout << "Broj elemenata u redu nakon brisanja: " << r.brojElemenata() << std::endl;
}

template<typename Tip>
void testCelo(Red<Tip> &r) {
    Tip celo = r.celo();
    std::cout << "Vrijednost na vrhu reda: " << celo << std::endl;
    celo++;
    std::cout << "Vrijednost na vrhu reda nakon promjene je: " << celo << std::endl;
}

template<typename Tip>
void testBrojElemenata(Red<Tip> &r) {
    int velicina = r.brojElemenata();
    std::cout << "Broj elemenata u redu: " << velicina << std::endl;
}

/*kroz red prolazimo tako sto skidamo element sa pocetka i prebacujemo ga na kraj
- moramo pamtiti velicinu pocetnog da bismo znali koliko elemenata prebacujemo*/

template<typename Tip>
void izbaciVeceOdAritmeticke(Red <Tip> &r){
    Tip aritmeticka{};
    int velicina=r.brojElemenata();
    for(int i=0; i<velicina; i++){
        Tip temp=r.skini();
        aritmeticka+=temp;
        r.stavi(temp);
    }
    aritmeticka/=velicina;
    for(int i=0; i<velicina; i++){
        Tip temp=r.skini();
        if(temp>aritmeticka) continue;
        r.stavi(temp);
    }
}

template<typename Tip>
void spojiStekIRed(Stek<Tip> &s, Red <Tip> &r, int pocetnaDuzinaReda){
    while(s.brojElemenata()!=0) r.stavi(s.skini());
    for(int i=0; i<pocetnaDuzinaReda; i++) s.stavi(r.skini());
    //sad mi se u steku nalazi prvobitni elementi reda, 
    //a u redu su mi elementi iz steka ali u obrnutom poretku
    int brojac=0;
    while(r.brojElemenata()!=0){ 
        s.stavi(r.skini());
        brojac++;
    }
    for(int i=0; i<brojac; i++) r.stavi(s.skini());
    while(r.brojElemenata()!=0) s.stavi(r.skini());
}

template<typename Tip>
void spojiRedIListu(Red <Tip> &r, Lista<Tip> &l, int n){
    int vel=l.brojElemenata()+r.brojElemenata();
    //prvo premjesti sve u jednu kolekciju i nekako zapamti odakle dokle ti je sta
    while(vel>0){
        int brojac=n;
        //ova prebacivanja rijesi
        while(brojac>0 && r.brojElemenata()!=0){

        }
        brojac=n;
        while(brojac>0 && l.brojElemenata()!=0){
            
        }
        vel--;
    }
}

int main() {
    Red<double> red;
    testStavi(red,1.01);
    testStavi(red,2.02);
    testStavi(red,3.03);
    testBrojElemenata(red);
    testSkini(red);
    testCelo(red);
    testBrojElemenata(red);
    testBrisi(red);
    try{
        testCelo(red);
    }
    catch(std::exception &e){
        std::cout<<e.what()<<std::endl;
    }
    
    //test za izbacivanje vecih od aritmeticke sredine
    red.stavi(1.01); red.stavi(5.02); red.stavi(2.04);
    izbaciVeceOdAritmeticke(red);
    std::cout<<"Red prije funkcije koja izbacuje vece od aritmeticke sredine je:"<< 
        " 1.01, 5.02, 2.04,\n a nakon pozivanja: ";
    for(int i=0; i<red.brojElemenata(); i++){
        std::cout<<red.celo()<<"  ";
        red.stavi(red.skini());
    }
    red.brisi();

    //test za spoji red i stek u stek
    Red<int> r;
    Stek<int> s;
    r.stavi(1); r.stavi(2); r.stavi(3); r.stavi(4);
    s.stavi(5); s.stavi(6); s.stavi(7); s.stavi(8);
    spojiStekIRed(s, r, r.brojElemenata());
    std::cout<<"\n\nStek nakon modifikacije u obrnutom redosljedu glasi:\n";
    while(s.brojElemenata()!=0) std::cout<<s.skini()<<" ";
    std::cout<<std::endl;
    return 0;
}