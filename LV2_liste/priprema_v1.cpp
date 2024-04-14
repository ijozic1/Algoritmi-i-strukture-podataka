#include <iostream>
#include <stdexcept>

template <typename TipEl>
class Lista{
  public:
    Lista(){} //kreira praznu listu
    virtual ~Lista(){}
    virtual int brojElemenata()const=0; //vraca broj elemenata u listi - svaki put prilikom dodavanja ili brisanja ce biti potrebno modificirati odgovarajuci atribut
    virtual TipEl trenutni() const=0; //vraca vrijednost trenutnog clana
    virtual TipEl & trenutni()=0; //vraca referencu na trenutni clan kako bi se mogao mijenjati
    virtual bool prethodni()=0; //pomijera trenutni element unazad - ako je ok vrati true, a ako je prvi vraca false
    virtual bool sljedeci()=0; //pomijera trenutni element unazad - ako je uspjesno vraca true, a ako je na posljednjem elementu vraca false
    virtual void pocetak()=0; //postavlja prvi element za trenutni
    virtual void kraj()=0; //postavlja posljednji element za trenutni
    virtual void obrisi()=0; //brise trenutni; sljedeci postaje trenutni, a ako je obrisan posljednji onda trenutni postaje onaj prije njega i treba povesti racuna da se promijeni i ono u metodi posljednji
    //ako je prazna lista trenutni, prethodni, sljedeci i pocetak, kraj, obrisi bacaju izuzetke
    virtual void dodajIspred(const TipEl & element)=0; //dodaje element ispred trenutnog
    virtual void dodajIza(const TipEl & element)=0; //dodaje element iza trenutnog
    //ako je lista prazna obje dodaju element na pocetak i taj el postaje trenutni inace nema pomijeranja
    virtual TipEl &operator[](int i)=0;
    virtual TipEl operator[](int i) const=0;
    //pristup clanu niza, ako je i negativno ili >= brojElemenata treba baciti izuzetak
};

template <typename TipEl>
class NizLista : public Lista<TipEl>{
    static const int maxVel=100;
    //moras napraviti realokaciju sa duplanjem kad se popuni u potpunosti - pokazivac na dinamicki alocirani niz
    //prebacimo sve elemente na novu poziciju, dodamo i obrisemo staru
    //pokazivac na niz pokazivaca
    //najlakse ti je implementirati posebnu metodu koja ce raditi realokaciju i povecavati kapacitet
    TipEl cppNiz[maxVel];
    int brElemenata, trenutniEl;
  public:
    NizLista(): brElemenata(0), trenutniEl(-1) {}
    int brojElemenata() const override {return brElemenata;} 
    TipEl trenutni() const override;
    TipEl & trenutni() override;
    bool prethodni() override; 
    bool sljedeci() override;
    void pocetak() override; 
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const TipEl & element) override;
    void dodajIza(const TipEl & element) override;
    TipEl &operator[](int i) override;
    TipEl operator[](int i) const override;
    //kopirajuci konstruktor, operator dodjele i destruktor dodati
};

template <typename TipEl>
class JednostrukaLista : public Lista<TipEl>{
    struct Cvor{
        TipEl element;
        Cvor* sljedeci;
        Cvor(const TipEl &el) : element(el), sljedeci(nullptr){}
    };
    Cvor *prvi, *posljednji, *trenutniEl;
    int brElemenata;
  public:
    JednostrukaLista(): prvi(nullptr), posljednji(nullptr), trenutniEl(nullptr), brElemenata(0) {}
    ~JednostrukaLista(){
        while(prvi!=nullptr){
            Cvor* obrisan=prvi;
            prvi=prvi->sljedeci;
            delete obrisan;
        }
    }
    int brojElemenata() const override {return brElemenata;} 
    TipEl trenutni() const override;
    TipEl & trenutni() override;
    bool prethodni() override; 
    bool sljedeci() override;
    void pocetak() override; 
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const TipEl & element) override;
    void dodajIza(const TipEl & element) override;
    TipEl &operator[](int i) override;
    TipEl operator[](int i) const override;
};

//NizLista
template <typename TipEl>
TipEl NizLista<TipEl>::trenutni() const{
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    return cppNiz[trenutniEl];
}

template <typename TipEl>
TipEl & NizLista<TipEl>::trenutni(){
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    return cppNiz[trenutniEl];
}

template <typename TipEl>
bool NizLista<TipEl>::prethodni(){
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    if(trenutniEl<=0) return false;
    trenutniEl--;
    return true;
}

template <typename TipEl>
bool NizLista<TipEl>::sljedeci(){
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    if(trenutniEl>=this->brojElemenata()-1 || trenutniEl>=maxVel-1) return false;
    trenutniEl++;
    return true;
}

template <typename TipEl>
void NizLista<TipEl>::pocetak(){
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    trenutniEl=0;
}

template <typename TipEl>
void NizLista<TipEl>::kraj(){
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    trenutniEl=this->brojElemenata()-1;
}

template <typename TipEl>
void NizLista<TipEl>::obrisi(){
    if(this->brojElemenata()==0 || trenutniEl<0 || trenutniEl>=maxVel) 
        throw std::range_error("Nepostojeci element");
    int brEl=this->brojElemenata();
    for(int i=trenutniEl; i<brEl-1; i++) cppNiz[i]=cppNiz[i+1];
    this->brElemenata--;
    if(trenutniEl==brojElemenata()) trenutniEl--;
    else if(brojElemenata()==0) trenutniEl=-1;
}

template <typename TipEl>
void NizLista<TipEl>::dodajIspred(const TipEl &element){
    if(this->brojElemenata()>=maxVel-1) throw std::logic_error("Lista popunjena");
    if(this->brojElemenata()==0){
        cppNiz[0]=element;
        brElemenata++; trenutniEl=0;
    }
    else{
        int brEl=this->brojElemenata();
        for(int i=brEl; i>trenutniEl; i--) cppNiz[i]=cppNiz[i-1];
        cppNiz[trenutniEl+1]=element;
        brElemenata++; 
    }
    //treutni treba pomjeriti da se ne pomijera element na koji pokazuje
}

template <typename TipEl>
void NizLista<TipEl>::dodajIza(const TipEl &element){
    if(this->brojElemenata()>=maxVel-1) throw std::logic_error("Lista popunjena");
    if(this->brojElemenata()==0){
        cppNiz[0]=element;
        brElemenata++; trenutniEl=0;
    }
    else{
        int brEl=this->brojElemenata();
        for(int i=brEl; i>trenutniEl+1; i--) cppNiz[i]=cppNiz[i-1];
        cppNiz[trenutniEl+1]=element;
        brElemenata++; 
    }
}

template <typename TipEl>
TipEl & NizLista<TipEl>::operator[](int i){
    if(i<0 || i>brojElemenata()-1)
        throw std::range_error("Nepostojeci element");
    return cppNiz[i];
}

template <typename TipEl>
TipEl NizLista<TipEl>::operator[](int i) const{
    if(i<0 || i>brojElemenata()-1)
        throw std::range_error("Nepostojeci element");
    return cppNiz[i];
}

//JednostrukaLista
template <typename TipEl>
TipEl JednostrukaLista<TipEl>::trenutni() const{
    if(this->brojElemenata()==0 || trenutniEl==nullptr)
        throw std::range_error("Izvan opsega");
    return trenutniEl->element;
}

template<typename TipEl>
TipEl & JednostrukaLista<TipEl>::trenutni(){
    if(this->brojElemenata()==0 || trenutniEl==nullptr)
        throw std::range_error("Izvan opsega");
    return trenutniEl->element;
}

template<typename TipEl>
bool JednostrukaLista<TipEl>::prethodni(){
    if(this->brojElemenata()<=1 || trenutniEl==prvi) return false;
    Cvor *prethodni=prvi;
    while(prethodni->sljedeci!=trenutniEl) prethodni=prethodni->sljedeci;
    trenutniEl=prethodni;
    return true;
}

template<typename TipEl>
bool JednostrukaLista<TipEl>::sljedeci(){
    if(this->brojElemenata()<=1 || trenutniEl==nullptr) return false;
    if(trenutniEl==posljednji) return false;
    trenutniEl=trenutniEl->sljedeci;
    return true;
}
    
template<typename TipEl>
void JednostrukaLista<TipEl>::pocetak(){
     if(this->brojElemenata()==0 || trenutniEl==nullptr)
        throw std::range_error("Prazna lista");
    trenutniEl=prvi;
}

template<typename TipEl>
void JednostrukaLista<TipEl>:: kraj(){
     if(this->brojElemenata()==0 || trenutniEl==nullptr)
        throw std::range_error("Prazna lista");
    trenutniEl=posljednji;
}

template<typename TipEl>
void JednostrukaLista<TipEl>::obrisi(){
    if(brojElemenata()==0 || trenutniEl==nullptr)
        throw std::range_error("Prazna lista");
    if(trenutniEl==prvi){
        Cvor *obrisan=prvi;
        prvi=prvi->sljedeci;
        delete obrisan;
    }
    else {
        Cvor *prethodni=prvi;
        while(prethodni->sljedeci!=trenutniEl) prethodni=prethodni->sljedeci;
        prethodni->sljedeci=trenutniEl->sljedeci;
        delete trenutniEl;
    }
    if(trenutniEl==posljednji) posljednji=nullptr;
    brElemenata--;
    if(this->brojElemenata()==0) trenutniEl=nullptr;
}

template<typename TipEl>
void JednostrukaLista<TipEl>::dodajIspred(const TipEl & element){
    Cvor *novi=new Cvor(element);
    if(this->brojElemenata()==0){
        novi->sljedeci=prvi;
        prvi=novi;
        if(posljednji==nullptr) posljednji=prvi;
        trenutniEl=prvi;
    }
    else if(trenutniEl==prvi){
        novi->sljedeci=prvi;
        prvi=novi;
    }
    else {
        Cvor* prethodni=prvi;
        while(prethodni->sljedeci!=trenutniEl) prethodni=prethodni->sljedeci;
        novi->sljedeci=trenutniEl;
        prethodni->sljedeci=novi;
    }
    brElemenata++;
}

template<typename TipEl>
void JednostrukaLista<TipEl>::dodajIza(const TipEl & element){
    Cvor *novi=new Cvor(element);
    if(this->brojElemenata()==0){
        prvi=novi;
        posljednji=prvi;
        trenutniEl=prvi;
    }
    else if(trenutniEl==posljednji){
        trenutniEl->sljedeci=novi;
        posljednji=novi;
    }
    else{
        novi->sljedeci=trenutniEl->sljedeci;
        trenutniEl->sljedeci=novi;
    }
    brElemenata++;
}

template<typename TipEl>
TipEl & JednostrukaLista<TipEl>::operator[](int i){
    if(i<0 || i>=this->brojElemenata())
        throw std::range_error("Izvan opsega");
    Cvor* trenutni=prvi;
    for(int j=0; j<i; j++) trenutni=trenutni->sljedeci;
    return trenutni->element;
}

template<typename TipEl>
TipEl JednostrukaLista<TipEl>::operator[](int i) const{
    if(i<0 || i>=this->brojElemenata())
        throw std::range_error("Izvan opsega");
    Cvor* trenutni=prvi;
    for(int j=0; j<i; j++) trenutni=trenutni->sljedeci;
    return trenutni->element;
}

//test funkcije
template <typename TipEl>
void TestBrojElemenata(const Lista<TipEl> &l){
    std::cout<<"Broj elemenata je: "<<l.brojElemenata()<<std::endl;
}

template <typename TipEl>
void TestTrenutni(Lista <TipEl> &l){
    try{
        std::cout<<"Trenutni element: "<<l.trenutni();
        l.trenutni()++; //hoce li mi dozvoliti ako gore vratim kao pokazivac
        std::cout<<", a nakon inkrementiranja je: "<<l.trenutni()<<std::endl;
    }
    catch(std::exception &izuzetak) {
        std::cout<<izuzetak.what()<<std::endl;
    }
}

template <typename TipEl>
void TestPrethodni(Lista <TipEl> &l){
    if(l.prethodni()) std::cout<<"Pomjereno na prethodni\n";
    else std::cout<<"Nije pomjereno na prethodni\n";
}

template <typename TipEl>
void TestSljedeci(Lista <TipEl> &l){
    if(l.sljedeci()) std::cout<<"Pomjereno na sljedeci\n";
    else std::cout<<"Nije pomjereno na sljedeci\n";
}

template <typename TipEl>
void TestPocetak(Lista <TipEl> &l){
    l.pocetak();
    std::cout<<"Pomjereno na pocetak\n";
}

template <typename TipEl>
void TestKraj(Lista <TipEl> &l){
    l.kraj();
    std::cout<<"Pomjereno na kraj\n";
}

template <typename TipEl>
void TestObrisi(Lista <TipEl> &l){
    try{
        l.obrisi();
        std::cout<<"Element je obrisan\n";
    }
    catch(std::exception &izuzetak){
        std::cout<<izuzetak.what()<<std::endl;
    }
}

template <typename TipEl, typename Tip>
void TestDodajIspred(Lista <TipEl> &l, Tip element){
    l.dodajIspred(element);
    std::cout<<element<<" dodan ispred trenutnog\n";
}

template <typename TipEl, typename Tip>
void TestDodajIza(Lista <TipEl> &l, Tip element){
    l.dodajIza(element);
    std::cout<<element<<" dodan iza trenutnog\n";
}

/*int main(){
    NizLista<double> l;
    TestBrojElemenata(l);
    TestTrenutni(l);
    TestObrisi(l);
    TestDodajIspred(l,2);
    TestPocetak(l);
    TestDodajIza(l,5);
    TestKraj(l);
    TestDodajIza(l,3);
    l.sljedeci();
    l.prethodni();
    std::cout<<l[2]<<std::endl<<std::endl;

    //test za jednostruke liste
    JednostrukaLista<int> jl; 
    TestBrojElemenata(jl);
    TestTrenutni(jl);
    TestObrisi(jl);
    TestDodajIspred(jl,2);
    TestPocetak(jl);
    TestDodajIza(jl,5);
    TestKraj(jl);
    TestDodajIza(jl,3);
    l.sljedeci();
    l.prethodni();
    std::cout<<jl[2]<<std::endl<<std::endl;
    
    return 0;
}*/