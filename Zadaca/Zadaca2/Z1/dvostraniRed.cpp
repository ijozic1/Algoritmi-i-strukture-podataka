#include <iostream>

using std::cout;

template <typename TipEl>
class DvostraniRed {
  private:
    struct Cvor {
        TipEl vrijednost;
        Cvor* sljedeci, *prethodni;

        Cvor(const TipEl& v, Cvor* sljed, Cvor* preth) : vrijednost(v), sljedeci(sljed), prethodni(preth) {}
    };

    Cvor* prvi, *zadnji;
    int brElemenata;

    bool jePrazan() const {
        return brElemenata == 0;
    }

  public:
    DvostraniRed() : prvi(nullptr), zadnji(nullptr), brElemenata(0) {}

    //kopirajuci konstruktor
    DvostraniRed(const DvostraniRed &dr) : prvi(nullptr), zadnji(nullptr), brElemenata(0) {
        if(dr.brojElemenata()==0){
            return;
        }
        Cvor* temp = dr.prvi;
        while (temp != nullptr) {
            staviNaVrh(temp->vrijednost);
            temp = temp->sljedeci;
        }
    }

    //operator dodjele
    DvostraniRed& operator=(const DvostraniRed &dr) {
        if (this != &dr) {
            brisi();
            Cvor* temp = dr.prvi;
            while (temp != nullptr) {
                staviNaVrh(temp->vrijednost);
                temp = temp->sljedeci;
            }
        }
        return *this;
    }

    ~DvostraniRed() {
        brisi();
    }

    void brisi() {
        while (!jePrazan()) {
            skiniSaCela();
        }
    }

    void staviNaVrh(const TipEl& vrijednost) {
        Cvor* noviCvor = new Cvor(vrijednost, nullptr, zadnji);
        if (jePrazan())
            prvi = noviCvor;
        else
            zadnji->sljedeci = noviCvor;
        zadnji = noviCvor;
        brElemenata++;
    }

    TipEl skiniSaVrha() {
        if (jePrazan()) 
            throw std::logic_error("Dvostrani red je prazan.");
        Cvor* temp = zadnji;
        zadnji = zadnji->prethodni;
        TipEl vr=temp->vrijednost;
        delete temp;
        brElemenata--;
        return vr;
    }

    void staviNaCelo(const TipEl& vrijednost) {
        Cvor* noviCvor = new Cvor(vrijednost, prvi, nullptr);
        if (jePrazan())
            zadnji = noviCvor;
        else 
            prvi->prethodni = noviCvor;
        prvi = noviCvor;
        brElemenata++;
    }

    TipEl skiniSaCela() {
        if (jePrazan()) 
            throw std::logic_error("Dvostrani red je prazan.");
        Cvor* temp = prvi;
        prvi = prvi->sljedeci;
        TipEl vr=temp->vrijednost;
        delete temp;
        brElemenata--;
        return vr;
    }

    TipEl vrh() const {
        if (!jePrazan())
            return zadnji->vrijednost;
        throw std::logic_error("Dvostrani red je prazan.");
    }

    TipEl & vrh() {
        if (!jePrazan())
            return zadnji->vrijednost;
        throw std::logic_error("Dvostrani red je prazan.");
    }

    TipEl celo() const {
        if (!jePrazan())
            return prvi->vrijednost;
        throw std::logic_error("Dvostrani red je prazan.");
    }

    TipEl & celo() {
        if (!jePrazan())
            return prvi->vrijednost;
        throw std::logic_error("Dvostrani red je prazan.");
    }

    int brojElemenata() const {
        return brElemenata;
    }
};

// Funkcija za testiranje
void testirajMetode() {
    DvostraniRed<int> red;
    //dodavanje elemenata na vrh
    red.staviNaVrh(1);
    if (red.vrh() == 1 && red.celo() == 1 && red.brojElemenata() == 1)
        std::cout << "Test 1: ok" << std::endl;
    else
        std::cout << "Test 1: Neuspjesan" << std::endl;

    red.staviNaVrh(2);
    if (red.vrh() == 2 && red.celo() == 1 && red.brojElemenata() == 2)
        std::cout << "Test 2: ok" << std::endl;
    else 
        std::cout << "Test 2: Neuspjesan" << std::endl;

    // dodavanja elemenata na celo
    red.staviNaCelo(0);
    if (red.vrh() == 2 && red.celo() == 0 && red.brojElemenata() == 3)
        std::cout << "Test 3: ok" << std::endl;
    else
        std::cout << "Test 3: Neuspjesan" << std::endl;

    // skidanja sa vrha i cela
    red.skiniSaVrha();
    if (red.vrh() == 1 && red.celo() == 0 && red.brojElemenata() == 2)
        std::cout << "Test 4: ok" << std::endl;
    else 
        std::cout << "Test 4: Neuspjesan" << std::endl;

    red.skiniSaCela();
    if (red.vrh() == 1 && red.celo() == 1 && red.brojElemenata() == 1)
        std::cout << "Test 5: ok" << std::endl;
    else
        std::cout << "Test 5: Neuspjesan" << std::endl;

    // brisanje
    red.brisi();
    if (red.brojElemenata() == 0)
        std::cout << "Test 6: ok" << std::endl;
    else 
        std::cout << "Test 6: Neuspjesan" << std::endl;

    // Dodavanje i skidanje u praznom redu
    red.staviNaVrh(42);
    if (red.vrh() == 42 && red.celo() == 42 && red.brojElemenata() == 1)
        std::cout << "Test 7: ok" << std::endl;
    else
        std::cout << "Test 7: Neuspjesan" << std::endl;

    red.skiniSaVrha();
    if (red.brojElemenata() == 0)
        std::cout << "Test 8: ok" << std::endl;
    else
        std::cout << "Test 8: Neuspjesan" << std::endl;

    // Testiranje kopirajuceg konstruktora
    DvostraniRed<int> kopijaReda(red);
    kopijaReda.staviNaVrh(5);
    kopijaReda.staviNaCelo(3);
    kopijaReda.staviNaVrh(7);

    if (kopijaReda.vrh() == 7 && kopijaReda.celo() == 3 && kopijaReda.brojElemenata() == 3)
        std::cout << "Test 9: ok" << std::endl;
    else
        std::cout << "Test 9: Neuspjesan" << std::endl;

    // Testiranje operatora dodjele
    DvostraniRed<int> red2;
    red2 = kopijaReda;

    if (red2.vrh() == 7 && red2.celo() == 3 && red2.brojElemenata() == 3)
        std::cout << "Test 10: ok" << std::endl;
    else
        std::cout << "Test 10: Neuspjesan" << std::endl;

    //izuzetak za prazan red
    DvostraniRed<int> prazanRed;
    bool izuzetakBacen = false;
    try {
        prazanRed.vrh();
    } 
    catch (const std::logic_error &e) {
        izuzetakBacen = true;
    }
    if (izuzetakBacen)
        std::cout << "Test 11: ok" << std::endl;
    else 
        std::cout << "Test 11: Neuspjesan" << std::endl;

    izuzetakBacen = false;
    try {
        prazanRed.celo();
    } 
    catch (const std::logic_error &e) {
        izuzetakBacen = true;
    }
    if (izuzetakBacen)
        std::cout << "Test 12: ok" << std::endl;
    else
        std::cout << "Test 12: Neuspjesan" << std::endl;

    //brisanje praznog reda
    prazanRed.brisi();
    if (prazanRed.brojElemenata() == 0)
        std::cout << "Test 13: ok" << std::endl;
    else
        std::cout << "Test 13: Neuspjesan" << std::endl;

    std::cout << "Svi testovi su zavrseni." << std::endl;
}

int main() {
    testirajMetode();
                    
    return 0;
}