#include <exception>
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
};

template <typename Tip> class Stek {
    JednostrukaLista<Tip> lista;
    //komentarima je oznacena implementacija steka preko dinamicki alociranog niza pokazivaca
    /*Tip **stek;
    int vrh_steka;
    int kapacitet;*/

    /*void kopiraj(const Stek &s) {
        kapacitet = s.kapacitet;
        stek = new Tip *[kapacitet];
        for (int i = 0; i <= s.vrh_steka; i++)
            stek[i] = new Tip(*s.stek[i]);
        vrh_steka = s.vrh_steka;
    }*/

    /*void povecajKapacitet() {
        kapacitet *= 2;
        Tip **novi_stek = new Tip *[kapacitet]();
        for (int i = 0; i <= vrh_steka; i++)
            novi_stek[i] = stek[i];
        delete[] stek;
        stek = novi_stek;
    }*/

    bool jePrazan() const {
        return (this->lista.brojElemenata()==0); 
        //return vrh_steka == -1; 
    }

    /*bool jePun() const { 
        return vrh_steka == kapacitet - 1; 
    }*/

  public:
    Stek() {
        /*vrh_steka = -1;
        kapacitet = 10;
        stek = new Tip *[kapacitet];*/
    }

    ~Stek() {
        /*for (int i = 0; i <= vrh_steka; i++)
            delete stek[i];
        delete[] stek;*/
    }

    Stek(const Stek &s) { 
        //kopiraj(s); 
        this->lista=s.lista;
    }

    Stek &operator=(const Stek &s) {
        /*if (this != &s) {
            for (int i = 0; i <= vrh_steka; i++)
                delete stek[i];
            delete[] stek;
            kopiraj(s);
        }
        return *this;*/
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
        /*if (jePun()) {
            povecajKapacitet();
        }
        vrh_steka++;
        stek[vrh_steka] = new Tip(el);*/
        this->lista.dodajIspred(el);
        this->lista.pocetak();
    }

    Tip skini() {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        /*Tip temp = *stek[vrh_steka];
        delete stek[vrh_steka];
        vrh_steka--;
        return temp;*/
        this->lista.pocetak();
        Tip temp=this->lista.trenutni();
        this->lista.obrisi();
        return temp;
    }

    Tip& vrh() {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        //return *stek[vrh_steka];
        this->lista.pocetak();
        return this->lista.trenutni();
    }

    Tip vrh() const {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        //return *stek[vrh_steka];
        this->lista.pocetak();
        return this->lista.trenutni();
    }

    int brojElemenata() const { 
        /*return vrh_steka + 1;*/
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

//test funkcije
template<typename Tip>
int testBrojElemenata(const Stek <Tip> &s){
    return s.brojElemenata();
}

template<typename Tip>
void testBrisi(Stek <Tip> &s){
    std::cout<<"Stek ima "<<testBrojElemenata(s)<<" i poziva se funkcija za oslobadjanje steka\n";
    try{
        s.brisi();
    }catch(std::exception &e){
        std::cout<<"Doslo je do greske: "<<e.what()<<std::endl;
        throw e;
    }
    std::cout<<"Svi elementi su obrisani. Stek sada ima "<<testBrojElemenata(s)<<" elemenata\n";
}

template<typename Tip>
void testStavi(Stek <Tip> &s, const Tip &el){
    std::cout<<"Stek ima "<<testBrojElemenata(s)<<" i poziva se funkcija za dodavanje elemenata\n";
    try{
        s.stavi(el);
        std::cout<<"Dodan je element "<<el<<std::endl;
    }
    catch(std::exception &e){
        std::cout<<"Doslo je do greske: "<<e.what()<<std::endl;
        throw e;
    }
    std::cout<<"Stek sada ima "<<testBrojElemenata(s)<<" elemenata\n";
}

template<typename Tip>
void testSkini(Stek <Tip> &s){
    std::cout<<"Stek ima "<<testBrojElemenata(s)<<" i poziva se funkcija za skidanje vrha.\n";
    Tip el;
    try{
        el=s.skini();
        std::cout<<"Element na vrhu je "<<el<<"\n";
    }catch(std::exception &e){
        std::cout<<"Doslo je do greske: "<<e.what()<<std::endl;
        throw e;
    }
    std::cout<<"Vrh je skinut. Stek sada ima "<<testBrojElemenata(s)<<" elemenata\n";
}

template<typename Tip>
void testVrh(Stek <Tip> &s){
    std::cout<<"Poziva se funkcija za informaciju o vrhu.\n";
    try{
        const Tip &el=s.vrh();
        std::cout<<"Element na vrhu je "<<el<<"\n";
        s.vrh()+=2;
        std::cout<<"Element na vrhu nakon promjene je "<<el<<"\n";
    }catch(std::exception &e){
        std::cout<<"Doslo je do greske: "<<e.what()<<std::endl;
        throw e;
    }
}



/*
najjbitnije za rad sa stekom je nauciti da kada premjestimo elemente iz steka u
neku drugu strukturu dobivamo obrnut stek npr. bilo 1 2 3 4 i vrh u 4 u drugoj
strukturi cemo imati 4 3 2 1
*/

/*
paziti na rad sa stekom i for petljom ako je uslov i<brElemenata
- svaki put kad pozovemo skini, promijeniti ce se broj elemenata i onda imamo
problem
- mozemo ili spremiti brEl prije nego pocnemo vrtiti petlju ili koristiti while
vrh!=nullptr
*/


bool provjeri_zagrade(std::string tekst) {
    Stek<char> otvoreneZagrade;
    for (int i = 0; i < tekst.length(); i++) {
        char znak = tekst[i];
        if (znak == '(' || znak == '[' || znak == '{')
            otvoreneZagrade.stavi(znak);
        else if (znak == ')' || znak == ']' || znak == '}') {
            if (otvoreneZagrade.brojElemenata() == 0)
                return false;
            char vrh=otvoreneZagrade.vrh();
            if((vrh=='(' && znak!=')') || (vrh=='[' && znak!=']') || (vrh=='{' && znak!='}'))  
                return false;
            otvoreneZagrade.skini();
        }
    }
    return true;
}

template<typename Tip>
int presjek(Stek <Tip> s1, Stek <Tip> s2){
    int presjek=0;
    while(s1.brojElemenata()!=0 && s2.brojElemenata()!=0){
        if(s1.vrh()==s2.vrh()){
            s1.skini(); s2.skini(); presjek++;
        }
        else if(s1.vrh()<s2.vrh()) s1.skini();
        else if(s2.vrh()<s1.vrh()) s2.skini();
    }
    return presjek;
}

bool palindrom(Lista <char> & rijec){ 
    Stek<char> pomocni;
    int velListe=rijec.brojElemenata();
    rijec.pocetak();
    for(int i=1; i<velListe/2; i++){
        pomocni.stavi(rijec.trenutni());
        rijec.sljedeci();
    }
    if(velListe%2!=0) rijec.sljedeci();
    while(pomocni.brojElemenata()!=0){
        if(pomocni.vrh()==rijec.trenutni()){
            pomocni.skini();
            rijec.sljedeci();
        }
        else return false;
    }
    return true;
}

int spoji(Stek<int> s1, Stek<int> s2, Stek<int> &s3){
    int vel=s1.brojElemenata()+s2.brojElemenata();
    while(s2.brojElemenata()!=0){
        s3.stavi(s2.skini());
    }
    for(int i=0; i<vel; i++){
        if(s1.brojElemenata()==0 && s3.brojElemenata()!=0) s2.stavi(s3.skini());
        else if(s1.brojElemenata()!=0 && s3.brojElemenata()==0) s2.stavi(s1.skini());
        else if(s1.vrh()>s3.vrh()){ 
            s2.stavi(s1.skini());
        }
        else if(s3.vrh()>s1.vrh()){
            s2.stavi(s3.skini());
        }
        else{
            s2.stavi(s3.skini());
            s1.skini();
        }
    } 
    s3.brisi();
    while(s2.brojElemenata()!=0){
        s3.stavi(s2.skini());
    }
    return s3.brojElemenata();
}

int main() {
    Stek<double> stek;

    if (testBrojElemenata(stek)== 0) {
        std::cout << "Test za brojElemenata: Prolazi" << std::endl;
    } 
    else {
        std::cout << "Test za brojElemenata: Ne prolazi" << std::endl;
    }

    testStavi(stek, 1.22);
    testStavi(stek, 2.24);
    testStavi(stek,3.17);


    if (testBrojElemenata(stek) == 3) {
        std::cout << "Test za dodavanje elemenata na stek: Prolazi" << std::endl;
    } 
    else {
        std::cout << "Test za dodavanje elemenata na stek: Ne prolazi" << std::endl;
    }

    testSkini(stek);
    testVrh(stek);
    
    testBrisi(stek);
    if (testBrojElemenata(stek) == 0) {
        std::cout << "Test za brisanje svih elemenata iz steka: Prolazi"<< std::endl;
    } else {
        std::cout << "Test za brisanje svih elemenata iz steka: Ne prolazi"<< std::endl;
    }

    bool ok_zagrade = provjeri_zagrade("[()]{}{[()()]()}");
    std::cout << "\n\n" << ok_zagrade << std::endl;
    ok_zagrade = provjeri_zagrade("[(])");
    std::cout << "\n\n" << ok_zagrade << std::endl;

    Stek<char> s1;
    s1.stavi('F'); s1.stavi('D'); s1.stavi('C'); s1.stavi('A');
    Stek<char> s2;
    s2.stavi('G'); s2.stavi('F'); s2.stavi('E'); s2.stavi('D'); s2.stavi('C'); s2.stavi('B');
    std::cout<<"\nU presjeku s1 i s2 ima "<<presjek(s1,s2)<<" elemenata"<<std::endl;
    
    JednostrukaLista <char> rijec;
    rijec.dodajIspred('R'); rijec.sljedeci();
    rijec.dodajIspred('A'); rijec.sljedeci();
    rijec.dodajIspred('T'); rijec.sljedeci();
    //rijec.dodajIspred('T'); rijec.sljedeci(); - provjeravala radi li sa parnim brojem elemenata
    rijec.dodajIspred('A'); rijec.sljedeci();
    rijec.dodajIspred('R');
    if(palindrom(rijec)) std::cout<<"\nRijec je palindrom"<<std::endl<<std::endl;
    else std::cout<<"\nRijac nije palindrom\n"<<std::endl<<std::endl;

    Stek <int> s3;
    s3.stavi(1); s3.stavi(3); s3.stavi(5); s3.stavi(8);
    Stek <int> s4;
    s4.stavi(9); s4.stavi(6); s4.stavi(4); s4.stavi(2);
    Stek<int> s5;
    std::cout<<"s3 ima "<<s3.brojElemenata()<<", s4 ima "<<s4.brojElemenata()<<
        ", a s5 nakon spajanja ima "<<spoji(s3,s4,s5)<<" elemenata."<<std::endl;
    std::cout<<"Stek 5 u obrnutom poretku je: ";
    while(s5.brojElemenata()!=0){
        std::cout<<s5.vrh()<<" ";
        s5.skini();
    }
    return 0;
}