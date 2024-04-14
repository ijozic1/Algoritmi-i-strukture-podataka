#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>
#include <string>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
  public:
    Mapa<TipKljuca, TipVrijednosti>() {}
    virtual ~Mapa<TipKljuca, TipVrijednosti>() {}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti &operator[](TipKljuca i) = 0;
    virtual const TipVrijednosti operator[](TipKljuca i) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  protected:
    int velicina, br_ele;
    std::pair<TipKljuca, TipVrijednosti> **p;

  public:
    NizMapa<TipKljuca, TipVrijednosti>(): Mapa<TipKljuca, TipVrijednosti>(), p(new std::pair<TipKljuca, TipVrijednosti> *[100000]()),
        velicina(100000), br_ele(0) {}

    ~NizMapa<TipKljuca, TipVrijednosti>() {
        for (int i = 0; i < br_ele; i++)
        delete p[i];
        delete[] p;
    }

    NizMapa<TipKljuca, TipVrijednosti>(const NizMapa<TipKljuca, TipVrijednosti> &a) {
        br_ele = a.br_ele;
        velicina = a.velicina;
        if (a.velicina == 0) p = nullptr;

        else {
            p = new std::pair<TipKljuca, TipVrijednosti> *[velicina]();
            for (int i = 0; i < a.br_ele; i++) {
                p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
            }
        }
    }

    NizMapa<TipKljuca, TipVrijednosti> &operator=(const NizMapa<TipKljuca, TipVrijednosti> &a) {
        if (this == &a) return *this;

        obrisi();
        br_ele = a.br_ele;
        if (a.Velicina() > Velicina()) {
            delete[] p;
            p = new std::pair<TipKljuca, TipVrijednosti> *[velicina + a.Velicina()]();
            velicina += a.velicina;
        }
        for (int i = 0; i < a.br_ele; i++)
            p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);

        return *this;
    }

    void obrisi() {
        if (br_ele == 0) throw std::range_error("Nema elemenata");

        for (int i = 0; i < br_ele; i++) delete p[i];
        br_ele = 0;
    }

    void obrisi(const TipKljuca &kljuc) {
        for (int i = 0; i < br_ele; i++)
            if (p[i]->first == kljuc) {
                delete p[i];
                for (int j = i; j < br_ele - 1; j++) p[j] = p[j + 1];
                p[br_ele] = nullptr;
                br_ele--;
                return;
            }
        throw std::logic_error("Nema kljuca");
    }

    int brojElemenata() const { return br_ele; }
    int Velicina() const { return velicina; }

    TipVrijednosti &operator[](TipKljuca kljuc) {
        if (br_ele == velicina) {
            std::pair<TipKljuca, TipVrijednosti> **temp = new std::pair<TipKljuca, TipVrijednosti> *[10000 + velicina]();
            for (int i = 0; i < br_ele; i++) {
                temp[i] = new std::pair<TipKljuca, TipVrijednosti>(*p[i]);
                delete p[i];
            }
            delete[] p;
            velicina += 10000;
            p = temp;
            temp = nullptr;
        }

        for (int i = 0; i < br_ele; i++)
            if (p[i]->first == kljuc) return p[i]->second;

        p[br_ele] = new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
        br_ele++;
        return p[br_ele - 1]->second;
    }

    const TipVrijednosti operator[](TipKljuca kljuc) const {
        for (int i = 0; i < br_ele; i++)
            if (p[i]->first == kljuc) return p[i]->second;

        return TipVrijednosti();
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *l_strana, *d_strana, *roditelj;

        Cvor(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti())
            : kljuc(i), vrijednost(u), d_strana(nullptr), l_strana(nullptr),
            roditelj(nullptr) {}
    };

    Cvor *novi(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti()) {
        Cvor *temp = new Cvor();
        temp->kljuc = i;
        temp->vrijednost = u;
        temp->d_strana = temp->l_strana = temp->roditelj = nullptr;
        return temp;
    }

    Cvor *pretraga(Cvor *p, TipKljuca i) const {
        if (p == nullptr || p->kljuc == i) return p;
        if (p->kljuc < i) return pretraga(p->d_strana, i);
        if (p->kljuc > i) return pretraga(p->l_strana, i);
        return p;
    }

    Cvor *dodaj(Cvor *p, TipKljuca i, TipVrijednosti u = TipVrijednosti(), Cvor *rod = nullptr) {
        if (p == nullptr) {
            p = novi(i, u);
            p->roditelj = rod;
            return p;
        }
        if (i > p->kljuc) p->d_strana = dodaj(p->d_strana, i, u, p);
        else p->l_strana = dodaj(p->l_strana, i, u, p);
        return p;
    }

    Cvor *nasljednik(Cvor *t) {
        auto ovaj = t;
        while (ovaj && ovaj->l_strana != nullptr)
        ovaj = ovaj->l_strana;
        return ovaj;
    }

    void konstruktor(Cvor *a, Cvor *b) {
        if (b == nullptr) return;

        konstruktor(a, b->l_strana);
        konstruktor(a, b->d_strana);
        dodaj(a, b->kljuc, b->vrijednost);
    }

    Cvor *obrisiC(Cvor *t, TipKljuca i) {
        if (t == nullptr) return t;

        if (i < t->kljuc) t->l_strana = obrisiC(t->l_strana, i);
        else if (i > t->kljuc) t->d_strana = obrisiC(t->d_strana, i);
        else {
            if (t->l_strana == nullptr) {
                Cvor *temp = t->d_strana;
                delete t;
                return temp;
            } 
            if (t->d_strana == nullptr) {
                Cvor *temp = t->l_strana;
                delete t;
                return temp;
            }

            auto q = nasljednik(t->d_strana);
            t->kljuc = q->kljuc;
            t->vrijednost = q->vrijednost;
            t->d_strana = obrisiC(t->d_strana, q->kljuc);
        }
        return t;
    }

    void obrisi_stablo(Cvor *t) {
        if (t == nullptr) return;

        obrisi_stablo(t->l_strana);
        obrisi_stablo(t->d_strana);
        delete t;
    }

    int br_ele;
    Cvor *p;

  public:
    BinStabloMapa<TipKljuca, TipVrijednosti>() : br_ele(0), p(nullptr) {}

    ~BinStabloMapa<TipKljuca, TipVrijednosti>() { obrisi_stablo(p); }

    BinStabloMapa<TipKljuca, TipVrijednosti>(const BinStabloMapa<TipKljuca, TipVrijednosti> &a) {
        br_ele = a.br_ele;
        if (a.br_ele == 0) p = nullptr;
        else {
            p = novi(a.p->kljuc, a.p->vrijednost);
            konstruktor(p, a.p);
        }
    }

    BinStabloMapa<TipKljuca, TipVrijednosti> &operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &a) {
        if (this == &a) return *this;

        obrisi();
        br_ele = a.br_ele;
        if (a.br_ele == 0) p = nullptr;
        else {
            p->kljuc = a.p->kljuc;
            p->vrijednost = a.p->vrijednost;
            konstruktor(p, a.p);
        }
        return *this;
    }

    int brojElemenata() const { return br_ele; }

    void obrisi() {
        obrisi_stablo(p);
        p = novi();
        br_ele = 0;
    }

    void obrisi(const TipKljuca &kljuc) {
        if (p->kljuc == kljuc) p = obrisiC(p, kljuc);
        else obrisiC(p, kljuc);
        br_ele--;
    }

    TipVrijednosti &operator[](TipKljuca i) {
        if (p == nullptr) {
            p = novi(i);
            br_ele++;
            return p->vrijednost;
        }

        auto c = pretraga(p, i);

        if (c == nullptr) {
            dodaj(p, i);
            br_ele++;
            c = pretraga(p, i);
        }
        return c->vrijednost;
    }

    const TipVrijednosti operator[](TipKljuca i) const {
        auto c = pretraga(p, i);
        if (c != nullptr) return c->vrijednost;
        return TipVrijednosti();
    }
};

unsigned int heshiranje(std::string ulaz, unsigned int max){
    unsigned int suma = 5381;
    for (int i(0); i < ulaz.length(); i++) suma = suma * 33 + ulaz[i];
    return suma % max;
}

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
  protected:
    int br_ele;
    std::vector<std::list<std::pair<TipKljuca, TipVrijednosti> *>> p;
    unsigned int (*funkcija)(TipKljuca, unsigned int);

  public:
    HashMapaLan<TipKljuca, TipVrijednosti>() : Mapa<TipKljuca, TipVrijednosti>(), br_ele(0), funkcija(nullptr) {
        p.resize(10000);
    }

    //~HashMapaLan<TipKljuca, TipVrijednosti>() { obrisi(); }
    ~HashMapaLan<TipKljuca, TipVrijednosti>() {
        if (funkcija) {
            obrisi();
        } 
        else {
            for (int i = 0; i < p.size(); i++) {
                auto it = p[i].begin();
                while (it != p[i].end()) {
                    delete *it;
                    it = p[i].erase(it);
                }
            }
        }
    }

    HashMapaLan<TipKljuca, TipVrijednosti>(const HashMapaLan<TipKljuca, TipVrijednosti> &a) {
        br_ele = a.br_ele;
        funkcija = a.funkcija;
        for (int i = 0; i < a.p.size(); i++) {
            p.push_back(std::list<std::pair<TipKljuca, TipVrijednosti> *>());
            auto it = a.p[i].begin();
            while (it != a.p[i].end()) {
                p[i].push_back(new std::pair<TipKljuca, TipVrijednosti>((*it)->first, (*it)->second));
                it++;
            }
        }
    }

    HashMapaLan<TipKljuca, TipVrijednosti> &operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &a) {
        if (this == &a) return *this;

        obrisi();

        while (p.size() < a.p.size())
            p.push_back(std::list<std::pair<TipKljuca, TipVrijednosti> *>());

        br_ele = a.br_ele;
        funkcija = a.funkcija;

        for (int i = 0; i < a.p.size(); i++) {
            auto it = a.p[i].begin();
            while (it != a.p[i].end()) {
                p[i].push_back(new std::pair<TipKljuca, TipVrijednosti>((*it)->first, (*it)->second));
                it++;
            }
        }
        return *this;
    }

    void obrisi() {
        if (!funkcija){ 
            throw std::domain_error("Hash funkcija nije definisana");
            //krahira se na ovom throw-u
        }
        if (br_ele == 0) throw std::range_error("Nema elemenata");

        for (int i = 0; i < p.size(); i++) {
            auto it = p[i].begin();
            while (it != p[i].end()) {
                delete *it;
                it++;
            }
            p[i].resize(0);
        }
        /*for (int i = 0; i < p.size(); i++) {
            auto it = p[i].begin();
            while (it != p[i].end()) {
                delete *it;
                it = p[i].erase(it);
            }
        }*/
        br_ele = 0;
    }

    void obrisi(const TipKljuca &kljuc) {
        if (!funkcija) throw std::domain_error("Hash funkcija ne postoji");

        int in = funkcija(kljuc, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator it = p[in].begin();

        while (it != p[in].end()) {
            if ((*it)->first == kljuc) {
                delete *it;
                p[in].erase(it);
                br_ele--;
                return;
            }
            it++;
        }
        throw std::logic_error("Nema kljuca");
    }

    int brojElemenata() const { return br_ele; }

    int TraziHash(const TipKljuca &k) const {
        int in = funkcija(k, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti> *> l = p[in];
        typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator it = l.begin();
        int i = 0;

        while (it != l.end()) {
            if ((*it)->first == k) return i;
            i++;
            it++;
        }
        return -1;
    }

    TipVrijednosti &operator[](TipKljuca kljuc) {
        if (!funkcija){ 
            //definisiHashFunkciju(heshiranje(kljuc, p.size()));
            throw std::domain_error("Hash funkcija?");
        }
        //ovdje baci exception it AT5

        int in = funkcija(kljuc, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator it = p[in].begin();

        if (p[in].empty()) {
            p[in].push_back(new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()));
            br_ele++;
            it = p[in].begin();
            return (*it)->second;
        }

        if ((*it)->first == kljuc) return (*it)->second;
        else {
            int q = TraziHash(kljuc);
            if (q == -1) {
                br_ele++;
                p[in].push_back(new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()));
                p[in].sort([](const std::pair<TipKljuca, TipVrijednosti> *a,
                            const std::pair<TipKljuca, TipVrijednosti> *b) -> bool {return a->first < b->first;});
                q = TraziHash(kljuc);
                it = p[in].begin();
            }
            while (q > 0) {
                it++;
                q--;
            }
        }
        return (*it)->second;
    }

    const TipVrijednosti operator[](TipKljuca kljuc) const {
        if (!funkcija) throw std::domain_error("Hash funkcija nedefinirana");

        int in = funkcija(kljuc, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti> *> l = p[in];
        typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator it = l.begin();

        if (!p[in].empty() && (*it)->first == kljuc) return (*it)->second;
        if (p[in].empty()) return TipVrijednosti();
        else {
            in = TraziHash(kljuc);
            if (in == -1) return TipVrijednosti();
            else
                while (in > 0) {
                    it++;
                    in--;
                }
        }
        return (*it)->second;
    }

    void definisiHashFunkciju(unsigned int (*func)(TipKljuca, unsigned int)) {
        funkcija = func;
        return;
    }
};

template <typename TipOznake> 
class GranaIterator;

template <typename TipOznake> 
class Grana;

template <typename TipOznake> 
class Cvor;

template <typename TipOznake> 
class ListaGraf;

template <typename TipOznake> 
class UsmjereniGraf {
  public:
    UsmjereniGraf() {}
    UsmjereniGraf(int a) {}
    virtual ~UsmjereniGraf() {}
    
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int a)  = 0;
    virtual void dodajGranu(int p, int d, float t)  = 0;
    virtual void obrisiGranu(int p, int d)  = 0;
    virtual bool  postojiGrana(int p, int d)  = 0;
    virtual void postaviTezinuGrane(int p, int d, float t) = 0;
    virtual TipOznake dajOznakuGrane(int p, int d)  = 0;
    virtual TipOznake dajOznakuCvora(int b)  = 0;
    virtual float dajTezinuGrane(int p, int d) = 0;
    virtual void postaviOznakuCvora(int b, TipOznake a)  = 0;
    virtual void postaviOznakuGrane(int p, int d, TipOznake a)  = 0;
    Grana<TipOznake> dajGranu(int p, int d) {return Grana<TipOznake>(this,p,d);} 
    Cvor<TipOznake> dajCvor(int b) { return Cvor<TipOznake>(this,b); }

    GranaIterator<TipOznake> dajGranePocetak() {
        GranaIterator<TipOznake> it(this,0,-1);
        return ++it;
    }

    GranaIterator<TipOznake> dajGraneKraj() {
        GranaIterator<TipOznake> it(this,-1,-1);
        return it;
    }
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int p, int d) = 0;
};

template <typename TipOznake> 
class Cvor  {
    UsmjereniGraf<TipOznake> *u;
    TipOznake oznaka;
    int broj;
    std::vector<Grana<TipOznake>> grane;
    std::vector<Cvor<TipOznake>> susjedi;

  public:
    Cvor(UsmjereniGraf<TipOznake> *a, int b) : u(a), broj(b) {}

    int dajRedniBroj() const { return broj; }

    void postaviOznaku(TipOznake a) { u->postaviOznakuCvora(broj, a); }

    TipOznake dajOznaku() const { return u->dajOznakuCvora(broj); }

    void dodajVezu(Grana<TipOznake> g, Cvor<TipOznake> s) {
        grane.push_back(g);
        susjedi.push_back(s);
    }

    void obrisiVezu(Grana<TipOznake> g, Cvor<TipOznake> s) {
        for (int i = 0; i < susjedi.size(); i++)
            if (s == susjedi[i]) {
                for (int j = i; j < susjedi.size() - 1; j++) {
                    susjedi[j] = susjedi[j + 1];
                    grane[j] = grane[j + 1];
                }
                break;
            }
        susjedi.pop_back();
        grane.pop_back();
    }

    std::vector<Grana<TipOznake>> dajGrane() const { return grane; }
    std::vector<Cvor<TipOznake>> dajSusjede() const { return susjedi; }
};

template <typename TipOznake> 
class Grana  {
    UsmjereniGraf<TipOznake> *u;
    TipOznake oznaka;
    float tezina;
    int p, d;

  public:
    Grana(UsmjereniGraf<TipOznake> *ug, int polazni, int dolazni) : u(ug), p(polazni), d(dolazni){ }
    float dajTezinu() const { return u->dajTezinuGrane(p,d); }
    void postaviTezinu(float a) {u->postaviTezinuGrane(p, d, a);}
    void postaviOznaku(TipOznake a) {u->postaviOznakuGrane(p, d, a); }
    TipOznake dajOznaku() const { return u->dajOznakuGrane(p,d); }
    Cvor<TipOznake> dajPolazniCvor() const { return u->dajCvor(p); };
    Cvor<TipOznake> dajDolazniCvor() const { return u->dajCvor(d); };
};

template <typename TipOznake> 
class GranaIterator {
    int p, d;
    UsmjereniGraf<TipOznake> *u;

  public:
    GranaIterator( UsmjereniGraf<TipOznake> *us, int a, int b):p(a),d(b),u(us){};

    Grana<TipOznake> operator*(){return u->dajGranu(p,d);}

    bool operator==(const GranaIterator &it)const{
        return it.u == u && it.p == p && it.d == d;
    }

    bool operator!=(const GranaIterator &it)const{
        return it.u != u || it.p != p || it.d != d;
    }

    GranaIterator &operator++(){
        GranaIterator<TipOznake> sljedeci = u->dajSljedecuGranu(p,d);
        p  = sljedeci.p;
        d = sljedeci.d;
        return *this;
    }

    GranaIterator operator++(int){
        GranaIterator<TipOznake> ovaj = *this;
        ++(*this);
        return ovaj;
    }
};
    
template <typename TipOznake> 
class ListaGraf : public UsmjereniGraf<TipOznake> {
    struct grana {
        TipOznake oznaka;
        float tezina;
        bool postoji;
    };
    std::vector<std::vector<grana>> lista;
    std::vector<TipOznake> oznake;

  public:
    ListaGraf(int b) : UsmjereniGraf<TipOznake>(b) {postaviBrojCvorova(b);}
    int dajBrojCvorova() const override {return lista.size();}

    void postaviBrojCvorova(int a) override{
        grana g;
        g.postoji = false;
        for(int i = 0 ; i < lista.size() ; i++) lista[i].resize(a,g);
        std::vector<grana> ubaci(a,g);
        lista.resize(a,ubaci);
        oznake.resize(a);
    }

    bool postojiGrana(int p, int d) override{
        return lista[p][d].postoji;
    }

    void dodajGranu(int p, int d, float t) override {
        grana g; g.tezina = t; g.postoji = true; lista[p][d] = g;
    }

    void obrisiGranu(int p, int d) override {
        lista[p][d].postoji = false;
    }

    void postaviTezinuGrane(int p, int d, float t) override {
        lista[p][d].tezina = t;
    }   

    TipOznake dajOznakuCvora(int b) override {
        return oznake[b];
    }
    
    TipOznake dajOznakuGrane(int p, int d) override {
        return lista[p][d].oznaka;
    }

    float dajTezinuGrane(int p, int d) override {
        return lista[p][d].tezina;
    }

    void postaviOznakuCvora(int b, TipOznake a) override{
        oznake[b] = a;
    }

    void postaviOznakuGrane(int p, int d, TipOznake a)override {
        lista[p][d].oznaka = a; 
    }

    GranaIterator<TipOznake> dajGranePocetak() { GranaIterator<TipOznake> it(this, 0, -1); ++it; return it; } 
    GranaIterator<TipOznake> dajGraneKraj() { GranaIterator<TipOznake> it(this, -1, -1); return it; } 
    
    GranaIterator<TipOznake> dajSljedecuGranu(int p, int d)override{
        for(int i=p;i<lista.size();i++){
            for(int j=0;j<lista.size();j++){
                if(i == p && j <= d) continue;
                if(lista[i][j].postoji)
                    return GranaIterator<TipOznake>(this,i,j);
            }
        }
        return GranaIterator<TipOznake>(this,-1,-1);
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *g, std::vector<Cvor<TipOznake>> &b, Cvor<TipOznake> c) {
    bool dodati = false;
    for (int i = 0; i < b.size(); i++) {
        if (b[i].dajRedniBroj() == c.dajRedniBroj()) {
            dodati = true;
            break;
        }
    }
    if (!dodati) b.push_back(c);
    int br = g->dajBrojCvorova();
    for (int i = 0; i < br; i++)
        if (!dodati) bfs(g, b, g->dajCvor(i));
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *g, std::vector<Cvor<TipOznake>> &b, Cvor<TipOznake> c) {
    bool dodati = false;
    for (int i = 0; i < b.size(); i++) {
        if (c.dajRedniBroj() == b[i].dajRedniBroj()) {
            dodati = true;
            break;
        }
    }
    if (!dodati) b.push_back(c);
    int brojCvorova = g->dajBrojCvorova();
    for (int i = 0; i < brojCvorova; i++)
        if (g->postojiGrana(c.dajRedniBroj(), i) && !dodati) dfs(g, b, g->dajCvor(i));
}

int main() {
    try {
        UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for (GranaIterator<bool> iter = g->dajGranePocetak();
        iter != g->dajGraneKraj(); ++iter)
        std::cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
        delete g;
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    try {
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

        std::cout << "\n\nNizMapa:\n";
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
            HashMapaLan<int, double> hashMapa;
            hashMapa.definisiHashFunkciju([](int ulaz, unsigned int max) {return ulaz % max;});

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
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    try{
        std::cout<<"\n\n\nAT5 koji krahira: \n";
        HashMapaLan<std::string,std::string> m;
        // Nije definisana hash funkcija!
        std::string s1("Sarajevo"), s2("Zagreb");
        m[s1] = "BiH";
        m[s2] = "Hrvatska";
        std::cout << m.brojElemenata() << " " << m[s2];
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    return 0;
}