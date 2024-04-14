#include <iostream>
#include <vector>

using namespace std;
template <typename TipEl> class Lista {
  public:
    Lista() {} 
    virtual ~Lista() {}
    virtual int
    brojElemenata() const = 0;
    virtual TipEl trenutni() const = 0; 
    virtual TipEl &
    trenutni() = 0; 
    virtual bool prethodni() = 0; 
    virtual bool
    sljedeci() = 0; 
    virtual void pocetak() = 0; 
    virtual void kraj() = 0;    
    virtual void obrisi() = 0;
    virtual void dodajIspred(const TipEl &element) = 0;
    virtual void dodajIza(const TipEl &element) = 0;
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
};

template <typename Tip> class Stek {
    JednostrukaLista<Tip> lista;

    bool jePrazan() const {
        return (this->lista.brojElemenata()==0); 
    }

  public:
    Stek() {}
    ~Stek() {}

    Stek(const Stek &s) {  
        this->lista=s.lista;
    }

    Stek &operator=(const Stek &s) {
        if(this!=&s){
            this->lista=s.lista;
        }
        return *this;
    }

    void brisi() {
        while (!jePrazan()) {
            skini();
        }
    }

    void stavi(const Tip &el) {
        this->lista.dodajIspred(el);
        this->lista.pocetak();
    }

    Tip skini() {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        this->lista.pocetak();
        Tip temp=this->lista.trenutni();
        this->lista.obrisi();
        return temp;
    }

    Tip& vrh() {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        this->lista.pocetak();
        return this->lista.trenutni();
    }

    Tip vrh() const {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        this->lista.pocetak();
        return this->lista.trenutni();
    }

    int brojElemenata() const { 
        return this->lista.brojElemenata();
    }
};

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

int binarnaPretraga(const std::vector<int>& vektor, int trazeni) {
    int lijevo = 0, desno = vektor.size() - 1;

    while (lijevo <= desno) {
        int srednji = lijevo + (desno - lijevo) / 2;

        if (vektor[srednji] == trazeni) 
            return srednji;
        else if (vektor[srednji] < trazeni)
            lijevo = srednji + 1;
        else 
            desno = srednji - 1;
    }
    //nije pronađen
    return -1;
}

void pretraga(Stek<std::vector<int> >& s, int trazeni){
    if(s.brojElemenata()==0){
        std::cout<<"Nema elementa";
        return;
    }
    std::vector<int> pomocni(s.vrh());

    if(!pomocni.empty() && pomocni[0]<=trazeni){
        int indeks=binarnaPretraga(pomocni, trazeni);

        if(indeks==-1) std::cout<<"Nema elementa";
        else std::cout<<indeks<<" "<<s.brojElemenata()-1;

        return;
    }
    s.skini();
    pretraga(s, trazeni);
    s.stavi(pomocni);
}

//test funkcije
void test1(Stek<std::vector<int>> &s) {
    pretraga(s, 10);
    std::cout<<std::endl;
}

void test2(Stek<std::vector<int>> &s) {
    pretraga(s, 157);
    std::cout<<std::endl;
}

void test3(Stek<std::vector<int>> &s) {
    pretraga(s, 200);
    std::cout<<std::endl;
}

int main() {
    Stek<std::vector<int>> s;
    s.stavi({1, 2, 3, 4, 5});
    s.stavi({6, 8});
    s.stavi({9, 10, 130});
    s.stavi({157, 226, 3424});

    test1(s);
    test2(s);
    test3(s);
    return 0;
}