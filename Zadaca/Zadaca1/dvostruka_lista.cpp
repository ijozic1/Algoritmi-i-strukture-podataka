#include <iostream>
#include <stdexcept>
#include <ctime>

using std::cout;

template <typename TipEl>
class Iterator;

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
    friend class Iterator<TipEl>;
};

template <typename TipEl> class DvostrukaLista : public Lista<TipEl> {
    struct Cvor {
        TipEl element;
        Cvor *sljedeci;
        Cvor *prethodni;
        Cvor(const TipEl &el, Cvor *prethodni=nullptr, Cvor *sljedeci = nullptr) : 
            element(el), sljedeci(sljedeci), prethodni(prethodni) {}
        Cvor(Cvor *prethodni = nullptr, Cvor *sljedeci=nullptr) : 
            sljedeci(sljedeci), prethodni(prethodni) {}
    };
    Cvor *prvi, *posljednji, *trenutniEl;
    int brElemenata;

  public:
    DvostrukaLista() : prvi(nullptr), posljednji(nullptr), trenutniEl(nullptr),brElemenata(0){}
    ~DvostrukaLista() {
        while (prvi != nullptr) {
            Cvor *temp = prvi;
            prvi = prvi->sljedeci;
            delete temp;
        }
        posljednji=trenutniEl=nullptr;
    }
    DvostrukaLista(const DvostrukaLista<TipEl> &jL);
    DvostrukaLista<TipEl> &operator=(const DvostrukaLista<TipEl> &jL);
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
    friend class Iterator<TipEl>;
};

template <typename Tip>
class Iterator {
    const DvostrukaLista<Tip> *lista;
    typename DvostrukaLista<Tip>::Cvor *trenutniLista;

  public:
    Iterator(const DvostrukaLista<Tip> &l) : lista(&l), trenutniLista(l.prvi) {}

    Tip trenutni() const {
        if (trenutniLista == nullptr)
            //throw std::out_of_range("Iterator je van granica");
            return Tip();
        return trenutniLista->element;
    }

    bool prethodni() {
        if (trenutniLista == nullptr)
            return false;
        trenutniLista = trenutniLista->prethodni;
        return true;
    }

    bool sljedeci() {
        if (trenutniLista == nullptr)
            return false;
        trenutniLista = trenutniLista->sljedeci;
        return true;
    }

    void pocetak() {
        trenutniLista = lista->prvi;
    }

    void kraj() {
        trenutniLista = lista->posljednji;
    }
};

//kopirajuci konstruktor za listu
template <typename TipEl>
DvostrukaLista<TipEl>::DvostrukaLista(const DvostrukaLista<TipEl> &dL) : brElemenata(dL.brElemenata){
    prvi = new Cvor(nullptr);
    posljednji = prvi;
    trenutniEl = prvi;

    Cvor *trenutniJL = dL.prvi->sljedeci;

    while (trenutniJL != nullptr) {
        posljednji->sljedeci = new Cvor(trenutniJL->element);
        posljednji->sljedeci->prethodni = posljednji;
        posljednji = posljednji->sljedeci;
        trenutniEl = trenutniJL == dL.trenutniEl ? posljednji : trenutniEl;
        trenutniJL = trenutniJL->sljedeci;
    }
}

// Operator dodjele
template <typename TipEl>
DvostrukaLista<TipEl> &DvostrukaLista<TipEl>::operator=(const DvostrukaLista<TipEl> &dL) {
    if (this == &dL)
        return *this;

    while (prvi != nullptr) {
        Cvor *temp = prvi;
        prvi = prvi->sljedeci;
        delete temp;
    }

    brElemenata = dL.brElemenata;

    prvi = new Cvor(nullptr);
    posljednji = prvi;
    trenutniEl = prvi;

    Cvor *trenutniJL = dL.prvi->sljedeci;

    while (trenutniJL != nullptr) {
        posljednji->sljedeci = new Cvor(trenutniJL->element);
        posljednji->sljedeci->prethodni = posljednji;
        posljednji = posljednji->sljedeci;
        trenutniEl = trenutniJL == dL.trenutniEl ? posljednji : trenutniEl;
        trenutniJL = trenutniJL->sljedeci;
    }

    return *this;
}

template <typename TipEl>
TipEl DvostrukaLista<TipEl>::trenutni() const {
    if (brElemenata == 0)
        throw std::out_of_range("Lista je prazna");

    return trenutniEl->element;
}

template <typename TipEl>
TipEl &DvostrukaLista<TipEl>::trenutni() {
    if (brElemenata == 0)
        throw std::out_of_range("Lista je prazna");

    return trenutniEl->element;
}

template <typename TipEl>
bool DvostrukaLista<TipEl>::prethodni() {
    if (brElemenata == 0 || trenutniEl == prvi)
        return false;

    trenutniEl = trenutniEl->prethodni;
    return true;
}

template <typename TipEl>
bool DvostrukaLista<TipEl>::sljedeci() {
    if (brElemenata == 0 || trenutniEl->sljedeci == nullptr)
        return false;

    trenutniEl = trenutniEl->sljedeci;
    return true;
}

template <typename TipEl>
void DvostrukaLista<TipEl>::pocetak() {
    if (brElemenata == 0)
        return;

    trenutniEl = prvi;
}

template <typename TipEl>
void DvostrukaLista<TipEl>::kraj() {
    if (brElemenata == 0)
        return;

    trenutniEl = posljednji;
}

template <typename TipEl>
void DvostrukaLista<TipEl>::obrisi() {
    if (brElemenata == 0)
        throw std::logic_error("Lista je prazna");

    Cvor *temp = trenutniEl;

    if(trenutniEl==prvi){
        prvi=prvi->sljedeci;
        trenutniEl=prvi;
        //prvi->prethodni=nullptr; - ovo bilo odkomentirano ali krahiralo
        temp->sljedeci=nullptr;
        brElemenata--;
        delete temp;
        return;
    }
    if(trenutniEl==posljednji){
        posljednji=posljednji->prethodni;
        trenutniEl=posljednji;
        posljednji->sljedeci=nullptr;
        temp->prethodni=nullptr;
        brElemenata--;
        delete temp;
        return;
    }
    trenutniEl->prethodni->sljedeci=trenutniEl->sljedeci;
    trenutniEl->sljedeci->prethodni=trenutniEl->prethodni;
    trenutniEl=trenutniEl->sljedeci;
    brElemenata--;
    delete temp;
}

template <typename TipEl>
void DvostrukaLista<TipEl>::dodajIspred(const TipEl &element) {
    Cvor *novi = new Cvor(element);
    if(brElemenata==0){
        prvi=novi;
        trenutniEl=prvi;
        posljednji=prvi;
        brElemenata++;
        return;
    }
    if(trenutniEl==prvi){
        prvi->prethodni=novi;
        novi->sljedeci=prvi;
        prvi=novi;
        brElemenata++;
        return;
    }

    novi->prethodni=trenutniEl->prethodni;
    novi->sljedeci=trenutniEl;
    trenutniEl->prethodni->sljedeci=novi;
    trenutniEl->prethodni=novi;
    brElemenata++;
}

template <typename TipEl>
void DvostrukaLista<TipEl>::dodajIza(const TipEl &element) {
    Cvor *novi = new Cvor(element);;
    if(brElemenata==0){
        prvi=novi;
        trenutniEl=prvi;
        posljednji=prvi;
        brElemenata++;
        return;
    }
    if(trenutniEl==posljednji){
        posljednji->sljedeci=novi;
        novi->prethodni=posljednji;
        posljednji=novi;
        brElemenata++;
        return;
    }
    
    novi->prethodni=trenutniEl;
    novi->sljedeci=trenutniEl->sljedeci;
    trenutniEl->sljedeci->prethodni=novi;
    trenutniEl->sljedeci=novi;

    brElemenata++;
}

template <typename TipEl>
TipEl &DvostrukaLista<TipEl>::operator[](int i) {
    if (i < 0 || i >= brElemenata)
        throw std::out_of_range("Indeks van granica");
    Cvor*temp=trenutniEl;
    // Ako je indeks blizu pocetka
    if (i <= brElemenata / 2) {
        //trenutniEl = prvi;
        temp=prvi;
        for (int j = 0; j < i; j++)
            //trenutniEl = trenutniEl->sljedeci;
            temp=temp->sljedeci;
    } 
    else {
        // blize kraju
        //trenutniEl = posljednji;
        temp=posljednji;
        for (int j = brElemenata - 1; j > i; j--)
            //trenutniEl = trenutniEl->prethodni;
            temp=temp->prethodni;
    }
    return temp->element;//trenutniEl->element;
}

template <typename TipEl>
TipEl DvostrukaLista<TipEl>::operator[](int i) const {
    if (i < 0 || i >= brElemenata)
        throw std::out_of_range("Indeks van granica");
    TipEl vrijednost;
    // Ako je indeks blizu pocetka
    if (i < brElemenata / 2) {
        Cvor *trenutni = prvi;
        for (int j = 0; j < i; j++) {
            trenutni = trenutni->sljedeci;
        }
        vrijednost=trenutni->element;
        //delete trenutni;
    } 
    else {
        //indeks bliži kraju
        Cvor *trenutni = posljednji;
        for (int j = brElemenata - 1; j > i; j--) {
            trenutni = trenutni->prethodni;
        }
        vrijednost= trenutni->element;
        //delete trenutni;
    }
    return vrijednost;
}

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n) {
    Iterator<Tip> it(dynamic_cast<const DvostrukaLista<Tip>&>(n));
    //Tip maksimum = it.trenutni();
    it.pocetak();
    Tip maksimum = it.trenutni();

    while (it.sljedeci()) {
        if (it.trenutni() > maksimum)
            maksimum = it.trenutni();
    }

    return maksimum;
}

//testne funkcije
void testDodajIspred(DvostrukaLista<int> &lista) {
    lista.dodajIspred(1);
    lista.dodajIspred(2);
    lista.dodajIspred(3);

    if (lista[0] == 2 && lista[1] == 3 && lista[2] == 1) {
        std::cout << "Test dodajIspred uspješan." << std::endl;
    } else {
        std::cerr << "Test dodajIspred nije uspio." << std::endl;
    }
}

void testDodajIza(DvostrukaLista<int> &lista) {
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);

    if (lista[0] == 2 && lista[1] == 3 && lista[2] == 1 && lista[3]==3 && lista[4]==2 && lista[5]==1){
        std::cout << "Test dodajIza uspješan." << std::endl;
    } else {
        std::cerr << "Test dodajIza nije uspio." << std::endl;
    }
}

void testObrisi(DvostrukaLista<int> &lista) {
    lista.dodajIspred(1);
    lista.dodajIspred(2);
    lista.dodajIspred(3);

    lista.obrisi();

    if (lista.trenutni()==3) {
        std::cout << "Test obrisi uspješan." << std::endl;
    } else {
        std::cerr << "Test obrisi nije uspio." << std::endl;
    }
}

void testPocetakKraj(DvostrukaLista<int> &lista) {
    lista.kraj();
    if (lista.trenutni() == 1) {
        std::cout << "Test pocetakKraj uspješan." << std::endl;
    } else {
        std::cerr << "Test pocetakKraj nije uspio." << std::endl;
    }

    lista.pocetak();
    if (lista.trenutni() == 2) {
        std::cout << "Test pocetakKraj uspješan." << std::endl;
    } else {
        std::cerr << "Test pocetakKraj nije uspio." << std::endl;
    }
}

void testPrethodniSljedeci(DvostrukaLista<int> &lista) {
    lista.sljedeci();
    if (lista.trenutni() == 3) {
        std::cout << "Test PrethodniSljedeci uspješan." << std::endl;
    } else {
        std::cerr << "Test PrethodniSljedeci nije uspio." << std::endl;
    }

    lista.prethodni();
    if (lista.trenutni() == 2) {
        std::cout << "Test PrethodniSljedeci uspješan." << std::endl;
    } else {
        std::cerr << "Test PrethodniSljedeci nije uspio." << std::endl;
    }
}

void testOperatorIndex(DvostrukaLista<int> &lista) {
    if (lista[0] == 2 && lista[1] == 3 && lista[2] == 1) {
        std::cout << "Test OperatorIndex uspješan." << std::endl;
    } else {
        std::cerr << "Test OperatorIndex nije uspio." << std::endl;
    }
}

int main() {
    DvostrukaLista<int> lista;

    testDodajIspred(lista);
    testDodajIza(lista);
    testObrisi(lista);
    testPocetakKraj(lista);
    testPrethodniSljedeci(lista);
    testOperatorIndex(lista);
    return 0;
}