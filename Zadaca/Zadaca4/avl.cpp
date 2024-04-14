#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <cmath>

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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        int balansCvora;

        Cvor(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti())
            : kljuc(k), vrijednost(v), desno(nullptr), lijevo(nullptr),
            roditelj(nullptr), balansCvora(0) {}
    };

    int brElemenata;
    Cvor *korijen;

    int balans(Cvor *t) {
        if (t == nullptr)
            return 0;
        return t->balansCvora;
    }

    int max(int a, int b) { return (a > b) ? a : b; }

    Cvor *novi(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti()) {
        Cvor *temp = new Cvor();
        temp->kljuc = k;
        temp->vrijednost = v;
        temp->desno = temp->lijevo = temp->roditelj = nullptr;
        temp->balansCvora = 0;
        return temp;
    }

    // rotiranje udesno
    void desno_rotiranje(Cvor *t) {
        Cvor *q = nullptr;
        int o = 0;
        q = t->lijevo->desno;
        t->balansCvora = 0;
        Cvor *l = t->lijevo;
        Cvor *r = t->roditelj;
        l->roditelj = r;
        l->desno = t;

        if (!l->roditelj && !o) {
            korijen = l;
            o = 1;
        } 
        else if (r->desno == t && !o) {
            l->roditelj->desno = l;
            o = 1;
        }

        if (o == 0) l->roditelj->lijevo = l;
        l->balansCvora = 0;

        if (q) q->roditelj = t;
        t->lijevo = q;
        t->roditelj = l;
    }

    // rotiranje ulijevo
    void lijevo_rotiranje(Cvor *t) {
        Cvor *q = nullptr;
        int o = 0;
        q = t->desno->lijevo;
        t->balansCvora = 0;
        Cvor *d = t->desno;
        Cvor *r = t->roditelj;
        d->roditelj = r;
        d->lijevo = t;

        if (!d->roditelj && !o) {
            korijen = d;
            o = 1;
        } 
        else if (r->desno == t && !o) {
            d->roditelj->desno = d;
            o = 1;
        }

        if (o == 0) d->roditelj->lijevo = d;

        d->balansCvora = 0;

        if (q) q->roditelj = t;
        t->desno = q;
        t->roditelj = d;
    }

    //Balans: balans lijevog cvora - balans desnog cvora
    void azurirajBalans(Cvor *t, bool b) {
        if (t == nullptr) return;
        if (b) t->balansCvora--;
        else t->balansCvora++;

        int blns = t->balansCvora;

        if (t->roditelj) {
            if (t->roditelj->desno == t) b = true;
            else b = false;
        }

        if (blns == 2) {
            if (balans(t->lijevo) == 1) desno_rotiranje(t);
            else {
                lijevo_rotiranje(t->lijevo);
                desno_rotiranje(t);
            }

        } 
        else if (blns == -2) {
            if (balans(t->desno) == -1)
                lijevo_rotiranje(t);

            else {
                desno_rotiranje(t->desno);
                lijevo_rotiranje(t);
            }
        } 
        else {
            if (blns == 0) return;
            azurirajBalans(t->roditelj, b);
        }
    }

    Cvor *traziCvor(Cvor *p, TipKljuca k) const {
        if (p == nullptr || p->kljuc == k) return p;
        if (p->kljuc < k) return traziCvor(p->desno, k);
        else if (p->kljuc > k) return traziCvor(p->lijevo, k);
        return p;
    }

    Cvor *dodaj(Cvor *p, TipKljuca k, TipVrijednosti u = TipVrijednosti(), Cvor *rod = nullptr) {
        while (p) {
            if (p->kljuc == k)  return p;
            if (k > p->kljuc) p = p->desno;
            else p = p->lijevo;
        }

        if (!p) {
            Cvor *t = AVLStabloMapa::korijen, *q = nullptr;
            p = novi(k, u);
            while (t) {
                q = t;
                if (t->kljuc > p->kljuc) t = t->lijevo;
                else t = t->desno;
            }

            if (!q)
                AVLStabloMapa::korijen = p;
            else if (q->kljuc > p->kljuc) {
                q->lijevo = p;
                p->roditelj = q;
            } 
            else {
                q->desno = p;
                p->roditelj = q;
            }

            if (!q) return p;

            if (q->lijevo && q->desno) q->balansCvora = 0;
            else {
                bool b = true;
                if (q->lijevo)
                b = false;
                azurirajBalans(q, b);
            }
        }
        return p;
    }

    Cvor *nasljednik(Cvor *t) {
        auto temp = t;
        while (temp && temp->lijevo != nullptr) temp = temp->lijevo;
        return temp;
    }

    void konstruktor(Cvor *a, Cvor *b) {
        if (b == nullptr) return;

        konstruktor(a, b->lijevo);
        konstruktor(a, b->desno);
        dodaj(a, b->kljuc, b->vrijednost);
    }

    void obrisiC(Cvor *t, TipKljuca k) {
        Cvor *r = nullptr;
        while (t != nullptr && t->kljuc != k) {
            r = t;
            if (t->kljuc < k) t = t->desno;
            else t = t->lijevo;
        }

        if (t == nullptr) return;

        Cvor *e, *q, *s ;

        if (!t->lijevo){
            e = t->desno;
            s = r;
        }
        else if (!t->desno){
            e = t->lijevo;
            s = r;
        }
        else {
            e = t->lijevo;
            q = e->desno;
            s = t;
            while (q) {
                s = e;
                e = q;
                q = e->desno;
            }
            if (t != s) {
                s->desno = e->lijevo;
                if (e->lijevo) e->lijevo->roditelj = s;
                e->lijevo = t->lijevo;
                if (t->lijevo) t->lijevo->roditelj = e;
            }
            e->desno = t->desno;
            if (t->desno) t->desno->roditelj = e;
        }

        if (!r) AVLStabloMapa::korijen = e;
        else {
            if (r->lijevo == t) {
                r->lijevo = e;
                r->balansCvora--;
            } 
            else {
                r->desno = e;
                r->balansCvora++;
            }
        }

        if (e)  e->roditelj = r;
        t->desno = t->lijevo = t->roditelj = nullptr;
        delete t;
    }

    void obrisi_stablo(Cvor *t) {
        if (t == nullptr) return;

        obrisi_stablo(t->lijevo);
        obrisi_stablo(t->desno);
        delete t;
    }

  public:
    AVLStabloMapa<TipKljuca, TipVrijednosti>() : brElemenata(0), korijen(nullptr) {}

    ~AVLStabloMapa<TipKljuca, TipVrijednosti>() { if(korijen) obrisi_stablo(korijen); }

    AVLStabloMapa<TipKljuca, TipVrijednosti>(const AVLStabloMapa<TipKljuca, TipVrijednosti> &a) {
        brElemenata = a.brElemenata;
        if (a.brElemenata == 0) korijen = nullptr;
        else {
            korijen = novi(a.korijen->kljuc, a.korijen->vrijednost);
            konstruktor(korijen, a.korijen);
        }
    }

    AVLStabloMapa<TipKljuca, TipVrijednosti> &operator=(const AVLStabloMapa<TipKljuca, TipVrijednosti> &a) {
        if (this != &a){
            obrisi();
            brElemenata = a.brElemenata;
            if (a.brElemenata == 0) korijen = nullptr;
            else {
                if(!korijen) korijen = novi(a.korijen->kljuc,a.korijen->vrijednost);
                korijen->kljuc = a.korijen->kljuc;
                korijen->vrijednost = a.korijen->vrijednost;
                konstruktor(korijen, a.korijen);
            }
        }
        return *this;
    }

    int brojElemenata() const override{ return brElemenata; }

    void obrisi() override{
        obrisi_stablo(korijen);
        korijen = nullptr;
        brElemenata = 0;
    }

    void obrisi(const TipKljuca &kljuc) override{
        obrisiC(korijen, kljuc);
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


    TipVrijednosti operator[](const TipKljuca& k) const override {
        auto c = traziCvor(korijen, k);
        if (c != nullptr) return c->vrijednost;
        return TipVrijednosti();
    }

    void Preorder(Cvor *korijen){
        if (korijen != nullptr) {
            std::cout << korijen->kljuc<< ",";
            Preorder(korijen->lijevo);
            Preorder(korijen->desno);
        }
    }

    void Preorder(){
        if (korijen != nullptr) {
            std::cout << korijen->kljuc<< ",";
            Preorder(korijen->lijevo);
            Preorder(korijen->desno);
        }
    }
};

int main() {
    AVLStabloMapa<int, int> avl1;
    for(int i=0; i<10; i++) avl1[std::rand()%100]=i;
    avl1.Preorder();

    std::cout<<"\n\n";
    AVLStabloMapa<int, double> avl2;
    for(int i=0; i<10; i++) avl2[i]=std::sin(i*i);
    avl2.Preorder();

    std::cout<<"\n\n";
    AVLStabloMapa<int, double> avl3;
    for(int i=0; i<10; i++) avl3[i+3]=std::cos(i*i);
    avl3.Preorder();

    std::cout<<"\n\n";
    AVLStabloMapa<int, double> avl4;
    for(int i=0; i<10; i++) avl4[i-1]=i;
    avl4.Preorder();

    std::cout<<"\n\n";
    AVLStabloMapa<int, double> avl5;
    for(int i=0; i<10; i++) avl5[i*i]=i;
    avl5.Preorder();

    return 0;
}