#include <iostream>
#include <stdexcept>
#include <ctime>
#include <vector>

//using std::cout;
using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    virtual ~Mapa() {}
    virtual TipVrijednosti& operator[](const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti operator[](const TipKljuca& kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  private:
    struct Pair {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Pair(const TipKljuca& k, const TipVrijednosti& v) : kljuc(k), vrijednost(v) {}
        Pair(const Pair& drugi) : kljuc(drugi.kljuc), vrijednost(drugi.vrijednost) {} //kopirajuci
    };

    Pair** nizParova;
    int kapacitet;
    int brElemenata;

    void povecajKapacitet() {
        kapacitet *= 2;
        Pair** noviNiz = new Pair*[kapacitet];
        for (int i = 0; i < kapacitet; i++)
            noviNiz[i] = nullptr;

        for (int i = 0; i < brElemenata; i++)
            noviNiz[i] = nizParova[i];

        delete[] nizParova;
        nizParova = noviNiz;
    }

  public:
    NizMapa() : kapacitet(10), brElemenata(0) {
        nizParova = new Pair*[kapacitet];
        for (int i = 0; i < kapacitet; i++)
            nizParova[i] = nullptr;
    }

    ~NizMapa() override {
        obrisi();
        delete[] nizParova;
    }

    TipVrijednosti& operator[](const TipKljuca& kljuc) override {
        for (int i = 0; i < brElemenata; i++) {
            if (nizParova[i] != nullptr && nizParova[i]->kljuc == kljuc)
                return nizParova[i]->vrijednost;
        }
        if (brElemenata == kapacitet) {
            povecajKapacitet();
        }
        nizParova[brElemenata] = new Pair(kljuc, TipVrijednosti());
        brElemenata++;
        return nizParova[brElemenata - 1]->vrijednost;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const override {
        for (int i = 0; i < brElemenata; i++) {
            if (nizParova[i]->kljuc == kljuc)
                return nizParova[i]->vrijednost;
        }
        //throw std::out_of_range("Kljuc nije pronadjen u mapi");
        TipVrijednosti vr=TipVrijednosti();
        return vr;
    }

    int brojElemenata() const override {
        return brElemenata;
    }

    void obrisi() override {
        for (int i = 0; i < brElemenata; i++) {
            delete nizParova[i];
            nizParova[i] = nullptr;
        }
        brElemenata = 0;
    }

    void obrisi(const TipKljuca& kljuc) override {
        int indeksBrisanja = -1;
        for (int i = 0; i < brElemenata; i++) {
            if (nizParova[i]->kljuc == kljuc) {
                indeksBrisanja = i;
                break;
            }
        }
        if (indeksBrisanja == -1)
            throw std::out_of_range("Kljuc nije pronadjen u mapi");

        delete nizParova[indeksBrisanja];
        for (int i = indeksBrisanja; i < brElemenata - 1; i++)
            nizParova[i] = nizParova[i + 1];

        nizParova[brElemenata - 1] = nullptr;
        brElemenata--;
    }

    // Kopirajući konstruktor
    NizMapa(const NizMapa& druga) {
        kapacitet = druga.kapacitet;
        brElemenata = druga.brElemenata;

        nizParova = new Pair*[kapacitet];
        for (int i = 0; i < kapacitet; ++i) {
            if (i < brElemenata) {
                nizParova[i] = new Pair(*druga.nizParova[i]);
            } 
            else {
                nizParova[i] = nullptr;
            }
        }
    }

    // Operator dodjele
    NizMapa& operator=(const NizMapa& druga) {
        if (this != &druga) {
            obrisi();
            delete[] nizParova;
            kapacitet = druga.kapacitet;
            brElemenata = druga.brElemenata;

            nizParova = new Pair*[kapacitet];
            for (int i = 0; i < kapacitet; ++i) {
                if (i < brElemenata) {
                    nizParova[i] = new Pair(*druga.nizParova[i]);
                } 
                else {
                    nizParova[i] = nullptr;
                }
            }
        }
        return *this;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;

        Cvor(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti())
            : kljuc(k), vrijednost(v), desno(nullptr), lijevo(nullptr),
            roditelj(nullptr) {}
    }; 

    Cvor *korijen;
    int brElemenata;     

    Cvor *novi(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti()) {
        Cvor *temp = new Cvor();
        temp->kljuc = k;
        temp->vrijednost = v;
        temp->desno = temp->lijevo = temp->roditelj = nullptr;
        return temp;
    }

    Cvor *traziCvor(Cvor *p, TipKljuca k) const {
        if (p == nullptr || p->kljuc == k)
            return p;
        if (p->kljuc < k)
            return traziCvor(p->desno, k);
        else if (p->kljuc > k)
            return traziCvor(p->lijevo, k);
        return p;
    }

    Cvor *dodaj(Cvor *p, TipKljuca k, TipVrijednosti v = TipVrijednosti(), Cvor *rod = nullptr) {
        if (p == nullptr) {
            p = novi(k, v);
            p->roditelj = rod;
            return p;
        }
        if (k > p->kljuc)
            p->desno = dodaj(p->desno, k, v, p);
        else
            p->lijevo = dodaj(p->lijevo, k, v, p);
        return p;
    }

    Cvor *nasljednik(Cvor *t) {
        auto temp = t;
        while (temp && temp->lijevo != nullptr)
            temp = temp->lijevo;
        return temp;
    }

    void konstruktor(Cvor *a, Cvor *b) {
        if (b == nullptr) return;

        konstruktor(a, b->lijevo);
        konstruktor(a, b->desno);
        dodaj(a, b->kljuc, b->vrijednost);
    }

    Cvor *obrisiC(Cvor *t, TipKljuca k) {
        if (t == nullptr) return t;

        if (k < t->kljuc)
            t->lijevo = obrisiC(t->lijevo, k);
        else if (k > t->kljuc)
            t->desno = obrisiC(t->desno, k);
        else {
            if (t->lijevo == nullptr) {
                Cvor *temp = t->desno;
                delete t;
                return temp;
            } 
            else if (t->desno == nullptr) {
                Cvor *temp = t->lijevo;
                delete t;
                return temp;
            }

            auto q = nasljednik(t->desno);
            t->kljuc = q->kljuc;
            t->vrijednost=q->vrijednost;
            t->desno = obrisiC(t->desno, q->kljuc);
        }
        return t;
    }

    void obrisi_stablo(Cvor *t) {
        if (t == nullptr) return;

        obrisi_stablo(t->lijevo);
        obrisi_stablo(t->desno);
        delete t;
    }

  public:
    BinStabloMapa<TipKljuca, TipVrijednosti>() : brElemenata(0), korijen(nullptr) {}
    ~BinStabloMapa<TipKljuca, TipVrijednosti>() { obrisi_stablo(korijen); }

    BinStabloMapa<TipKljuca, TipVrijednosti>(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo) {
        brElemenata = stablo.brElemenata;
        if (stablo.brElemenata == 0) korijen = nullptr;

        else {
            korijen = novi(stablo.korijen->kljuc, stablo.korijen->vrijednost);
            konstruktor(korijen, stablo.korijen);
        }
    }

    BinStabloMapa<TipKljuca, TipVrijednosti> &operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo) {
            if (this !=&stablo){
                obrisi();
                brElemenata = stablo.brElemenata;
                if (stablo.brElemenata== 0) korijen = nullptr;
                else {
                    korijen->kljuc=stablo.korijen->kljuc; korijen->vrijednost=stablo.korijen->vrijednost;
                    konstruktor(korijen, stablo.korijen);
                }
            }        
            return *this;
    }

    int brojElemenata() const override{ return brElemenata; }

    void obrisi() override{
        obrisi_stablo(korijen);
        korijen = novi();
        brElemenata = 0;
    }

    void obrisi(const TipKljuca &kljuc) override{
        if (korijen->kljuc == kljuc) korijen = obrisiC(korijen, kljuc);
        else obrisiC(korijen, kljuc);

        brElemenata--;
    }

    TipVrijednosti &operator[](const TipKljuca &k) override{
        if (korijen == nullptr) {
            korijen = novi(k);
            brElemenata++;
            return korijen->vrijednost;
        }
        auto c = traziCvor(korijen, k);
        if (c == nullptr) {
            dodaj(korijen, k);
            brElemenata++;
            c = traziCvor(korijen, k);
        }
        return c->vrijednost;
    }

    TipVrijednosti operator[](const TipKljuca &k) const override{
        auto c = traziCvor(korijen, k);
        if (c != nullptr)
        return c->vrijednost;
        return TipVrijednosti();
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
 protected:
    int velicina, br_ele;
    std::pair<TipKljuca, TipVrijednosti> **p;
    bool *prazno;
    unsigned int (*funkcija)(TipKljuca, unsigned int);

 public:
    HashMapa<TipKljuca, TipVrijednosti>(): Mapa<TipKljuca, TipVrijednosti>(), p(new std::pair<TipKljuca, TipVrijednosti> *[100000]()),
            velicina(100000), br_ele(0), prazno(new bool[100000]), funkcija(nullptr) {
        for (int i = 0; i < 100000; i++) prazno[i] = false;
    }

    ~HashMapa<TipKljuca, TipVrijednosti>() {
        if (p) {
            for (int i = 0; i < velicina; i++) delete p[i];
            delete[] p;
        }

        if (prazno)
        delete[] prazno;
    }

    HashMapa<TipKljuca, TipVrijednosti>(const HashMapa<TipKljuca, TipVrijednosti> &a) {
        br_ele = a.br_ele;
        velicina = a.velicina;
        funkcija = a.funkcija;
        prazno = new bool[velicina];

        if (a.velicina == 0) {
            p = nullptr;
            for (int i = 0; i < velicina; i++) prazno[i] = false;
        }

        else {
            p = new std::pair<TipKljuca, TipVrijednosti> *[velicina]();

            for (int i = 0; i < a.velicina; i++) {
                if (a.p[i]) p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
                prazno[i] = a.prazno[i];
            }
        }
    }

    HashMapa<TipKljuca, TipVrijednosti> &operator=(const HashMapa<TipKljuca, TipVrijednosti> &a) {
        if (this == &a) return *this;
        if (a.Velicina() > Velicina()) prosiri(a.velicina);

        for (int i = 0; i < Velicina(); i++) prazno[i] = false;

        br_ele = a.br_ele;
        funkcija = a.funkcija;

        for (int i = 0; i < a.velicina; i++) {
            if (a.p[i]) {
                delete p[i];
                p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
            }
            prazno[i] = a.prazno[i];
        }
        return *this;
    }

    void prosiri(int a = 10000) {
        std::pair<TipKljuca, TipVrijednosti> **temp = new std::pair<TipKljuca, TipVrijednosti> *[a + velicina]();
        bool *q = new bool[a + velicina]();

        for (int i = 0; i < velicina; i++) {
            if (p[i]) temp[i] = new std::pair<TipKljuca, TipVrijednosti>(*p[i]);
            delete p[i];
            q[i] = prazno[i];
        }
        for (int i = 0; i < a; i++) q[velicina + i] = false;

        delete[] p;
        delete[] prazno;
        velicina += a;
        p = temp;
        prazno = q;
    }

    void obrisi() {
        if (br_ele == 0) throw std::range_error("Nema elemenata");
        for (int i = 0; i < velicina; i++) prazno[i] = false;
        br_ele = 0;
    }

    void obrisi(const TipKljuca &kljuc) {
        for (int i = 0; i < velicina; i++)
            if (p[i] && p[i]->first == kljuc) {
                prazno[i] = false;
                br_ele--;
                return;
            }
        throw std::logic_error("Nema kljuca");
    }

    int brojElemenata() const { return br_ele; }
    int Velicina() const { return velicina; }

    /* int h (const int& k ,int i) const {
        return (k % velicina + i) % velicina;
    }*/

    bool UmetniHash(const TipKljuca &k) {
        int j, i = 1;
        do {
            do {
                j = funkcija(k, i);
                if (prazno[j] == false) {
                    delete p[j];
                    p[j] = new std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
                    prazno[j] = true;

                    return true;
                } 
                else i++;
            }while (i < velicina);
            prosiri();
        } while (velicina < 100000000);

        return false;
    }

    int TraziHash(const TipKljuca &k) const {
        int indeks, i(1);
        do {
            indeks = funkcija(k, i);
            if (prazno[indeks] && p[indeks]->first == k) return indeks;
            else i++;
        } while ((prazno[indeks] == true && i != velicina) || prazno[funkcija(k, i)] == true);
        return -1;
    }

    TipVrijednosti &operator[](const TipKljuca& kljuc) {
        if (!funkcija) throw std::bad_exception();
        if (br_ele == velicina) prosiri();

        int in = funkcija(kljuc, velicina);
        if (!prazno[in]) {
            delete p[in];
            p[in] = new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
            br_ele++;
            prazno[in] = true;
            return p[in]->second;
        }
        if (prazno[in] && p[in]->first == kljuc) return p[in]->second;
        else {
            in = TraziHash(kljuc);
            if (in == -1) {
                UmetniHash(kljuc);
                br_ele++;
                in = TraziHash(kljuc);
                prazno[in] = true;
            }
        }

        return p[in]->second;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const {
        if (!funkcija) throw std::bad_exception();

        int in = funkcija(kljuc, velicina);
        if (prazno[in] && p[in]->first == kljuc) return p[in]->second;
        else {
            in = TraziHash(kljuc);
            if (in == -1) return TipVrijednosti();
        }
        return p[in]->second;
    }

    void definisiHashFunkciju(unsigned int (*func)(TipKljuca, unsigned int)) {
        funkcija = func;
        return;
    }
};

/*unsigned int djbhash(std::string ulaz, unsigned int max) {
    unsigned int suma = 5381;
    // 5381 je pocetna vrijednost koja poboljsava distribuciju
    for (int i(0); i < ulaz.length(); i++)
        suma = suma * 33 + ulaz[i];
    return suma % max;
}*/

unsigned int mojahash(int ulaz, unsigned int max) {
    return ulaz % max;
}

bool UmetniHash(std::vector<int> &a, const int k) {
    int i = 1;
    int j = mojahash(k, a.size());

    if (a[j] == 0 || a[j] == -1){
        a[j]=k;
        int l = 0;
        for(int i = 0; i<a.size();i++)
            if(a[i] != 0 && a[i] != -1) l++;
        if(l==a.size()) a.resize(a.size()+3,0);
        return true;
    }

    j = mojahash(k, i);
    do {
        if (a[j] == 0 || a[j] == -1) {
            a[j] = k;
            int l = 0;
            for(int i = 0; i<a.size();i++)
                if(a[i] != 0 && a[i] != -1)l++;
            if(l==a.size())
            a.resize(a.size()+3,0);
            return true;
        } 
        else j = j + 1;
        if(j == a.size()) j = 0;
    } while (j < a.size());
    return false;
}

int main() {
    const int brojElemenata = 10000;  // Broj elemenata za testiranje
    srand(time(nullptr));

    // Testiranje NizMapa
    NizMapa<int, double> nizMapa;

    clock_t pocetak = clock();

    for (int i = 0; i < brojElemenata; i++)
        nizMapa[i];

    clock_t kraj = clock();
    double vrijemeDodavanja = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

    pocetak = clock();

    for (int i = 0; i < brojElemenata; ++i)
        int vrijednost = nizMapa[i];

    kraj = clock();
    double vrijemePristupa = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

    pocetak = clock();
    //nizMapa.obrisi();
    for (int i = 0; i < brojElemenata/2; ++i)
        nizMapa.obrisi(i);
    kraj = clock();
    double vrijemeBrisanja = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

    std::cout << "NizMapa:\n";
    std::cout << "Vrijeme dodavanja: " << vrijemeDodavanja << " sekundi\n";
    std::cout << "Vrijeme pristupa: " << vrijemePristupa << " sekundi\n";
    std::cout << "Vrijeme brisanja: " << vrijemeBrisanja << " sekundi\n\n";

    // Testiranje BinStabloMapa
    BinStabloMapa<int, double> binStabloMapa;

    pocetak = clock();
    for (int i = 0; i < brojElemenata; ++i)
        binStabloMapa[i];

    kraj = clock();
    vrijemeDodavanja = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

    pocetak = clock();

    for (int i = 0; i < brojElemenata; ++i)
        int vrijednost = binStabloMapa[i];

    kraj = clock();
    vrijemePristupa = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

    pocetak = clock();
    //binStabloMapa.obrisi();
    for (int i = 0; i < brojElemenata/2; ++i)
        binStabloMapa.obrisi(i);
    kraj = clock();
    vrijemeBrisanja = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

    std::cout << "BinStabloMapa:\n";
    std::cout << "Vrijeme dodavanja: " << vrijemeDodavanja << " sekundi\n";
    std::cout << "Vrijeme pristupa: " << vrijemePristupa << " sekundi\n";
    std::cout << "Vrijeme brisanja: " << vrijemeBrisanja << " sekundi\n\n";

    try{
        //Testiranje HashMapa
        HashMapa<int, double> hashMapa;
        hashMapa.definisiHashFunkciju(mojahash);

        pocetak = clock();
        for (int i = 0; i < brojElemenata; ++i)
            hashMapa[i];

        kraj = clock();
        vrijemeDodavanja = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

        pocetak = clock();

        for (int i = 0; i < brojElemenata; ++i)
            int vrijednost = hashMapa[i];

        kraj = clock();
        vrijemePristupa = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

        pocetak = clock();
        //hashMapa.obrisi();
        for (int i = 0; i < brojElemenata/2; ++i)
            hashMapa.obrisi(i);
        kraj = clock();
        vrijemeBrisanja = static_cast<double>(kraj - pocetak) / CLOCKS_PER_SEC;

        std::cout << "\nHashMapa:\n";
        std::cout << "Vrijeme dodavanja: " << vrijemeDodavanja << " sekundi\n";
        std::cout << "Vrijeme pristupa: " << vrijemePristupa << " sekundi\n";
        std::cout << "Vrijeme brisanja: " << vrijemeBrisanja << " sekundi\n\n";
    }
    catch(std::exception &e){
        std::cout<<"\nHashMapa: "<<e.what()<<std::endl;
    }

    //nakon testiranja nad istom velicinom ulaznih podataka vidimo da je hashMapa ubjedljivo najbrza 
    //po vremenu pristupa i dodavanja, dok je binStabloMapa najsporija
    //brisanje se najbrze izvodi u stablu dok je najsporije u nizMapi
    return 0;
}