#include <iostream>
#include <stdexcept>
#include <ctime>

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

template <typename TipEl> class NizLista : public Lista<TipEl> {
    TipEl **pokNiz;
    int kapacitet, brElemenata, trenutniEl;
    void realociraj();
  public:
    NizLista(int kapacitetNovog = 10) : brElemenata(0), kapacitet(kapacitetNovog), trenutniEl(0) {
        pokNiz = new TipEl *[kapacitetNovog]();
    }
    ~NizLista() {
        for (int i = 0; i < brElemenata; i++) delete pokNiz[i];
        delete[] pokNiz;
    }
    NizLista(const NizLista<TipEl> &nL);
    NizLista &operator=(const NizLista<TipEl> &nL);
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

// NizLista
template <typename TipEl> void NizLista<TipEl>::realociraj() {
    int noviKapacitet = 2 * kapacitet;
    TipEl **noviNiz = new TipEl *[noviKapacitet]();
    for (int i = 0; i < brElemenata; i++) noviNiz[i] = pokNiz[i];
    delete[] pokNiz;
    pokNiz = noviNiz;
    kapacitet = noviKapacitet;
}

template <typename TipEl> // kopirajuci konstruktor
NizLista<TipEl>::NizLista(const NizLista<TipEl> &nL) {
    brElemenata = nL.brojElemenata();
    /// trenutniEl=nL.trenutniEl;
    kapacitet = nL.kapacitet;
    pokNiz = new TipEl *[kapacitet]();
    for (int i = 0; i < brElemenata; i++)
        pokNiz[i] = new TipEl(*nL.pokNiz[i]);
}

template <typename TipEl> // preklopljeni operator dodjele
NizLista<TipEl> &NizLista<TipEl>::operator=(const NizLista<TipEl> &nL) {
    if (this == &nL) return *this;
    while (this->kapacitet < nL.kapacitet) this->realociraj();
    for (int i = 0; i < brElemenata; i++) delete this->pokNiz[i];

    brElemenata = nL.brElemenata;

    for (int i = 0; i < brElemenata; i++) pokNiz[i] = new TipEl(*(nL.pokNiz[i]));
    return *this;
}

template <typename TipEl> TipEl NizLista<TipEl>::trenutni() const {
    if (this->brojElemenata() == 0)
        throw std::range_error("Nepostojeci element");
    return *(pokNiz[trenutniEl]);
}

template <typename TipEl> TipEl &NizLista<TipEl>::trenutni() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Nepostojeci element");
    return *(pokNiz[trenutniEl]);
}

template <typename TipEl> bool NizLista<TipEl>::prethodni() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Nepostojeci element");
    if (trenutniEl <= 0) return false;
    trenutniEl--;
    return true;
}

template <typename TipEl> bool NizLista<TipEl>::sljedeci() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Nepostojeci element");
    if (trenutniEl >= this->brojElemenata() - 1) return false;
    trenutniEl++;
    return true;
}

template <typename TipEl> void NizLista<TipEl>::pocetak() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Prazna lista");
    trenutniEl = 0;
}

template <typename TipEl> void NizLista<TipEl>::kraj() {
    if (this->brojElemenata() == 0)
        throw std::range_error("Nepostojeci element");
    trenutniEl = this->brojElemenata() - 1;
}

template <typename TipEl> void NizLista<TipEl>::obrisi() {
    if (brElemenata <= 0)
        throw std::logic_error("Prazna lista");
    delete pokNiz[trenutniEl];
    for (int i = trenutniEl; i < brElemenata - 1; i++) pokNiz[i] = pokNiz[i + 1];

    pokNiz[brElemenata - 1] = nullptr;
    if (trenutniEl == brElemenata - 1 && trenutniEl != 0) trenutniEl--;
    brElemenata--;
}

template <typename TipEl>
void NizLista<TipEl>::dodajIspred(const TipEl &element) {
    if (this->brojElemenata() >= kapacitet) this->realociraj();
    if (brElemenata == 0) {
        pokNiz[0] = new TipEl(element);
        brElemenata++;
        return;
    }
    for (int i = brElemenata; i > trenutniEl; i--) pokNiz[i] = pokNiz[i - 1];

    pokNiz[trenutniEl] = new TipEl(element);
    brElemenata++;
    trenutniEl++;
}

template <typename TipEl> void NizLista<TipEl>::dodajIza(const TipEl &element) {
    if (this->brojElemenata() >= kapacitet) this->realociraj();
    if (brElemenata == 0) {
        pokNiz[0] = new TipEl(element);
        brElemenata++;
        return;
    }
    for (int i = brElemenata; i > trenutniEl + 1; i--) pokNiz[i] = pokNiz[i - 1];

    pokNiz[trenutniEl + 1] = new TipEl(element);
    brElemenata++;
}

template <typename TipEl> TipEl &NizLista<TipEl>::operator[](int i) {
    if (i < 0 || i > brojElemenata() - 1)
        throw std::range_error("Nepostojeci element");
    return *(pokNiz[i]);
}

template <typename TipEl> TipEl NizLista<TipEl>::operator[](int i) const {
    if (i < 0 || i > brojElemenata() - 1)
        throw std::range_error("Nepostojeci element");
    return *(pokNiz[i]);
}

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

// test funkcije
template <typename TipEl> void TestBrojElemenata(const Lista<TipEl> &l) {
    std::cout << "Broj elemenata je: " << l.brojElemenata() << std::endl;
}

template <typename TipEl> void TestTrenutni(Lista<TipEl> &l) {
    try {
        std::cout << "Trenutni element: " << l.trenutni();
        l.trenutni()++;
        std::cout << ", a nakon inkrementiranja je: " << l.trenutni() << std::endl;
    } 
    catch (std::exception &izuzetak) {
        std::cout << izuzetak.what() << std::endl;
    }
}

template <typename TipEl> void TestPrethodni(Lista<TipEl> &l) {
    if (l.prethodni())
        std::cout << "Pomjereno na prethodni\n";
    else
        std::cout << "Nije pomjereno na prethodni\n";
}

template <typename TipEl> void TestSljedeci(Lista<TipEl> &l) {
    if (l.sljedeci())
        std::cout << "Pomjereno na sljedeci\n";
    else
        std::cout << "Nije pomjereno na sljedeci\n";
}

template <typename TipEl> void TestPocetak(Lista<TipEl> &l) {
    l.pocetak();
    std::cout << "Pomjereno na pocetak\n";
}

template <typename TipEl> void TestKraj(Lista<TipEl> &l) {
    l.kraj();
    std::cout << "Pomjereno na kraj\n";
}

template <typename TipEl> void TestObrisi(Lista<TipEl> &l) {
    try {
        l.obrisi();
        std::cout << "Element je obrisan\n";
    } 
    catch (std::exception &izuzetak) {
        std::cout << izuzetak.what() << std::endl;
    }
}

template <typename TipEl, typename Tip>
void TestDodajIspred(Lista<TipEl> &l, Tip element) {
    l.dodajIspred(element);
    std::cout << element << " dodan ispred trenutnog\n";
}

template <typename TipEl, typename Tip>
void TestDodajIza(Lista<TipEl> &l, Tip element) {
    l.dodajIza(element);
    std::cout << element << " dodan iza trenutnog\n";
}

//vjezba
template<typename Tip>
void JednostrukaLista<Tip>::izbaciSvakiNTi(int n) {
    if (n <= 0 || this->brojElemenata() == 0 || n>=this->brojElemenata()) {
        throw std::logic_error("Neispravan n ili prazna lista.");
    }

    if (n == 1) {
        // Treba obrisati sve
        while (this->brojElemenata() > 0) {
            obrisi();
        }
        return;
    }

    int brojac = 0;

    // Pokaživači za praćenje prethodnog i trenutnog čvora.
    Cvor *prethodni = prvi;
    trenutniEl = prethodni->sljedeci;

    while (trenutniEl != nullptr) {
        brojac++;
        if (brojac == n) {
            Cvor *zaBrisanje = trenutniEl;
            trenutniEl = trenutniEl->sljedeci;
            prethodni->sljedeci = trenutniEl;
            delete zaBrisanje;
            brojac = 0;
            brElemenata--;
        } else {
            prethodni = trenutniEl;
            trenutniEl = trenutniEl->sljedeci;
        }
    }

    if (brojac != 0) {
        trenutniEl = prethodni;
        posljednji = prethodni;
    }
    duzinaLijevo = 1;
    duzinaDesno = brElemenata - 1;
}

void testIzbaciSvakiNTi() {
    JednostrukaLista<int> lista;

    for (int i = 1; i <= 10; i++) {
        lista.dodajIza(i);
    }

    std::cout << "Originalna lista: ";
    lista.pocetak();
    int vel=lista.brojElemenata();
    int brojac=0;
    while (brojac<vel) {
        std::cout << lista.trenutni() << " ";
        lista.sljedeci();
        brojac++;
    }
    std::cout << std::endl;

    int n = 3;
    lista.izbaciSvakiNTi(n);

    std::cout << "Nakon izbacivanja svakog " << n << ". elementa: ";
    lista.pocetak();
    vel=lista.brojElemenata();
    brojac=0;
    while (brojac<vel) {
        std::cout << lista.trenutni() << " ";
        lista.sljedeci();
    }
    std::cout << std::endl;
}

template<typename Tip>
Tip dajMaksimum(const NizLista<Tip>& n) {
    int vel=n.brojElemenata();
    NizLista <Tip> pomocna=n;
    pomocna.pocetak();
    Tip max=pomocna.trenutni();
    for(int i=1; i<vel; i++){
        pomocna.sljedeci();
        Tip temp=pomocna.trenutni();
        if(temp>max) max=temp;
    }
    return max;
}

template<typename Tip>
Tip dajMaksimum(const JednostrukaLista<Tip>& n) {
    int vel=n.brojElemenata();
    JednostrukaLista<Tip>pomocna=n;
    pomocna.pocetak();
    Tip max=pomocna.trenutni();
    for(int i=1; i<vel; i++){
        pomocna.sljedeci();
        Tip temp=pomocna.trenutni();
        if(temp>max) max=temp;
    }
    return max;
}


int main() {
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
    std::cout<<l[2]<<std::endl;
    clock_t vrijeme1 = clock();
		std::cout<<"Najveci element je: "<<dajMaksimum(l)<<std::endl;
	clock_t vrijeme2 = clock();
	int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000);
	std::cout<<"Vrijeme izvrsenja: "<<ukvrijeme<<" mikros."<<std::endl;
    std::cout<<std::endl<<std::endl;

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
    vrijeme1 = clock();
		std::cout<<"Najveci element je: "<<dajMaksimum(jl)<<std::endl;;
	vrijeme2 = clock();
	ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000);
	std::cout<<"Vrijeme izvrsenja: "<<ukvrijeme<<" mikros."<<std::endl;

    //test zad 3 sa LV
    try {
        testIzbaciSvakiNTi();
    } 
    catch (const std::exception &e) {
        std::cerr << "Izuzetak: " << e.what() << std::endl;
    }

    return 0;
}