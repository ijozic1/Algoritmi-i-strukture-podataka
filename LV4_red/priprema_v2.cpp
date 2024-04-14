#include <iostream>
#include <stdexcept>

using std::cout;

template <typename TipEl> class Lista {
  public:
    Lista() {} // kreira praznu listu
    virtual ~Lista() {}
    virtual int
    brojElemenata() const = 0; // vraca broj elemenata u listi - svaki put prilikom dodavanja ili brisanja ce biti potrebno modificirati odgovarajuci atribut
    virtual TipEl trenutni() const = 0; // vraca vrijednost trenutnog clana
    virtual TipEl &
    trenutni() = 0; // vraca referencu na trenutni clan kako bi se mogao mijenjati
    virtual bool prethodni() = 0; // pomijera trenutni element unazad - ako je ok vrati true, a ako je prvi vraca false
    virtual bool
    sljedeci() = 0; // pomijera trenutni element unazad - ako je uspjesno vraca true, a ako je na posljednjem elementu vraca false
    virtual void pocetak() = 0; // postavlja prvi element za trenutni
    virtual void kraj() = 0;    // postavlja posljednji element za trenutni
    virtual void obrisi() = 0; // brise trenutni; sljedeci postaje trenutni, a ako je obrisan posljednji onda trenutni postaje onaj prije njega
    // ako je prazna lista trenutni, prethodni, sljedeci i pocetak, kraj, obrisi bacaju izuzetke
    virtual void dodajIspred(const TipEl &element) = 0; // dodaje element ispred trenutnog
    virtual void dodajIza(const TipEl &element) = 0; // dodaje element iza trenutnog
    // ako je lista prazna obje dodaju element na pocetak i taj el postaje trenutni inace nema pomijeranja
    virtual TipEl &operator[](int i) = 0;
    virtual TipEl operator[](int i) const = 0;
};

template <typename TipEl> class JednostrukaLista : public Lista<TipEl> {
    struct Cvor {
        TipEl element;
        Cvor *sljedeci;
        Cvor(const TipEl &el, Cvor *sljedeci = nullptr) : element(el), sljedeci(sljedeci) {}
        Cvor(Cvor *sljedeci = nullptr) : sljedeci(sljedeci) {}
    };
    Cvor *prvi, *posljednji, *trenutniEl;
    int brElemenata, duzinaLijevo, duzinaDesno;

  public:
    JednostrukaLista() : prvi(new Cvor(nullptr)), posljednji(prvi), trenutniEl(prvi),
            brElemenata(0), duzinaDesno(0), duzinaLijevo(0) {}
    ~JednostrukaLista() {
        while (prvi != nullptr) {
            Cvor *temp = prvi;
            prvi = prvi->sljedeci;
            delete temp;
        }
    }
    JednostrukaLista(const JednostrukaLista<TipEl> &jL);
    JednostrukaLista<TipEl> &operator=(const JednostrukaLista<TipEl> &jL);
    int brojElemenata() const override { return brElemenata; }
    TipEl trenutni() const override;
    TipEl &trenutni() override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const TipEl &element) override;
    void dodajIza(const TipEl &element) override;
    TipEl &operator[](int i) override;
    TipEl operator[](int i) const override;
    void izbaciSvakiNTi(int n);
};

template<typename TipEl>
class Red{
    JednostrukaLista<TipEl> lista;
    bool daLiJePrazan() const {
        return (this->lista.brojElemenata()==0); 
    }
  public:
    Red(){}
    ~Red(){}
    Red(const Red &r){
        this->lista=r.lista;
    }
    Red &operator=(const Red & r){
        if(this!=&r){
            this->lista=r.lista;
        }
        return *this;
    }
    void stavi(const TipEl &el);
    TipEl skini();
    TipEl & celo();
    TipEl celo() const;
    void brisi(){
        while(!daLiJePrazan()) this->skini();
    }
    int brojElemenata(){
        return this->lista.brojElemenata();
    }
};

// JednostrukaLista
template <typename TipEl> // kopirajuci konstruktor
JednostrukaLista<TipEl>::JednostrukaLista(const JednostrukaLista<TipEl> &jL) {
    brElemenata = jL.brElemenata;
    prvi = new Cvor();
    Cvor *temp1 = jL.prvi->sljedeci;
    Cvor *temp2 = prvi;

    while (temp1 != nullptr) {
        temp2->sljedeci = new Cvor(temp1->element);
        temp1 = temp1->sljedeci;
        temp2 = temp2->sljedeci;
    }
    posljednji = temp2;
    trenutniEl = prvi;
    if (brElemenata > 0) {
        duzinaLijevo = 1;
        duzinaDesno = brElemenata - 1;
    }
}

template <typename TipEl> // operator dodjele
JednostrukaLista<TipEl> &
JednostrukaLista<TipEl>::operator=(const JednostrukaLista<TipEl> &jL) {
    while (prvi != nullptr) {
        Cvor *temp = prvi;
        prvi = prvi->sljedeci;
        delete temp;
    }
    brElemenata = jL.brElemenata;
    prvi = new Cvor();
    Cvor *temp1 = jL.prvi->sljedeci;
    Cvor *temp2 = prvi;

    while (temp1 != nullptr) {
        temp2->sljedeci = new Cvor(temp1->element);
        temp1 = temp1->sljedeci;
        temp2 = temp2->sljedeci;
    }
    posljednji = temp2;
    trenutniEl = prvi;
    if (brElemenata > 0) {
        duzinaLijevo = 1;
        duzinaDesno = brElemenata - 1;
    }
    return *this;
}

template <typename TipEl> TipEl JednostrukaLista<TipEl>::trenutni() const {
    if (this->brojElemenata() == 0)
        throw std::range_error("Izvan opsega");
    return (trenutniEl->sljedeci)->element;
}

template <typename TipEl> TipEl &JednostrukaLista<TipEl>::trenutni() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Izvan opsega");
    return (trenutniEl->sljedeci)->element;
}

template <typename TipEl> bool JednostrukaLista<TipEl>::prethodni() {
    if (this->brojElemenata() == 0)
        throw std::logic_error("Lista je prazna");
    if (duzinaLijevo == 1) return false;
    Cvor *prethodni = prvi;
    while (prethodni->sljedeci != trenutniEl) prethodni = prethodni->sljedeci;

    trenutniEl = prethodni;
    duzinaLijevo--;
    duzinaDesno++;
    return true;
}

template <typename TipEl> bool JednostrukaLista<TipEl>::sljedeci() {
    if (this->brojElemenata() == 0)
        throw std::logic_error("Prazna lista");
    if (duzinaDesno == 0) return false;
    trenutniEl = trenutniEl->sljedeci;
    duzinaLijevo++;
    duzinaDesno--;
    return true;
}

template <typename TipEl> void JednostrukaLista<TipEl>::pocetak() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Prazna lista");
    trenutniEl = prvi;
    duzinaLijevo = 1;
    duzinaDesno = brElemenata - 1;
}

template <typename TipEl> void JednostrukaLista<TipEl>::kraj() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Prazna lista");
    trenutniEl = prvi->sljedeci;
    while (trenutniEl->sljedeci != posljednji)
        trenutniEl = trenutniEl->sljedeci;
}

template <typename TipEl> void JednostrukaLista<TipEl>::obrisi() {
    if (brojElemenata() == 0 || trenutniEl == nullptr)
        throw std::range_error("Prazna lista");
    if (duzinaDesno == 0) {
        delete posljednji;
        trenutniEl->sljedeci = nullptr;
        posljednji = trenutniEl;
        prethodni();
        duzinaDesno--; brElemenata--;
        return;
    }

    Cvor *temp = (trenutniEl->sljedeci)->sljedeci;
    delete trenutniEl->sljedeci;
    trenutniEl->sljedeci = temp;
    duzinaLijevo--;
    brElemenata--;
}

template <typename TipEl>
void JednostrukaLista<TipEl>::dodajIspred(const TipEl &element) {
    Cvor *novi = new Cvor(element, trenutniEl->sljedeci);
    if (this->brojElemenata() == 0) {
        prvi->sljedeci = novi;
        posljednji = novi;
        brElemenata++;
        duzinaLijevo++;
        return;
    }
    trenutniEl->sljedeci = novi;
    trenutniEl = novi;
    brElemenata++;
    duzinaLijevo++;
}

template <typename TipEl>
void JednostrukaLista<TipEl>::dodajIza(const TipEl &element) {
    if (this->brojElemenata() == 0) {
        Cvor *novi = new Cvor(element, prvi->sljedeci);
        prvi->sljedeci = novi;
        brElemenata++;
        posljednji = novi;
        duzinaLijevo++;
        return;
    }
    Cvor *novi = new Cvor(element, (trenutniEl->sljedeci)->sljedeci);

    (trenutniEl->sljedeci)->sljedeci = novi;

    if (trenutniEl->sljedeci == posljednji)
        posljednji = novi;

    brElemenata++;
    duzinaDesno++;
}

template <typename TipEl> TipEl &JednostrukaLista<TipEl>::operator[](int i) {
    if (i < 0 || i >= this->brojElemenata())
        throw std::range_error("Izvan opsega");
    Cvor *trenutni = prvi->sljedeci;
    for (int j = 0; j < i; j++) trenutni = trenutni->sljedeci;
    return trenutni->element;
}

template <typename TipEl>
TipEl JednostrukaLista<TipEl>::operator[](int i) const {
    if (i < 0 || i >= this->brojElemenata())
        throw std::range_error("Izvan opsega");
    Cvor *trenutni = prvi->sljedeci;
    for (int j = 0; j < i; j++) trenutni = trenutni->sljedeci;
    return trenutni->element;
}

// red
template<typename TipEl>
void Red<TipEl>::stavi(const TipEl &el){
    this->lista.kraj();
    this->lista.dodajIspred(el);
    this->lista.pocetak();
}

template<typename TipEl>
TipEl Red<TipEl>::skini(){
    if(daLiJePrazan())
        throw std::logic_error("Red je prazan");
    this->lista.pocetak();
    TipEl temp=this->lista.trenutni();
    this->lista.obrisi();
    this->lista.pocetak();
    return temp;
}

template<typename TipEl>
TipEl & Red<TipEl>::celo(){
    if(daLiJePrazan())
        throw std::logic_error("Red je prazan");
    this->lista.pocetak();
    return this->lista.trenutni();
}

template<typename TipEl>
TipEl Red<TipEl>::celo() const{
    if(daLiJePrazan())
        throw std::logic_error("Red je prazan");
    this->lista.pocetak();
    return this->lista.trenutni();
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
    r.celo()++;
    std::cout << "Vrijednost na vrhu reda nakon promjene je: " << celo << std::endl;
}

template<typename Tip>
void testBrojElemenata(Red<Tip> &r) {
    int velicina = r.brojElemenata();
    std::cout << "Broj elemenata u redu: " << velicina << std::endl;
}

/*int main() {
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
    return 0;
}*/