#include <iostream>
#include <stdexcept>
#include <ctime>

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

    std::cout << "NizMapa:\n";
    std::cout << "Vrijeme dodavanja: " << vrijemeDodavanja << " sekundi\n";
    std::cout << "Vrijeme pristupa: " << vrijemePristupa << " sekundi\n\n";

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

    std::cout << "BinStabloMapa:\n";
    std::cout << "Vrijeme dodavanja: " << vrijemeDodavanja << " sekundi\n";
    std::cout << "Vrijeme pristupa: " << vrijemePristupa << " sekundi\n";

    return 0;
}