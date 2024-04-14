#include <iostream>
#include <stdexcept>

using std::cout;

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

// Test 1: Dodavanje i pristup vrijednostima
void testDodavanjeIPristup() {
    NizMapa<std::string, int> mapa;
    mapa["Jedan"] = 1;
    mapa["Dva"] = 2;
    mapa["Tri"] = 3;

    if (mapa.brojElemenata() == 3 &&
        mapa["Jedan"] == 1 &&
        mapa["Dva"] == 2 &&
        mapa["Tri"] == 3) {
        std::cout << "Test 1 uspješan." << std::endl;
    } else {
        std::cout << "Test 1 nije uspješan." << std::endl;
    }
}

// Test 2: Brisanje elementa
void testBrisanje() {
    NizMapa<std::string, double> mapa;
    mapa["Prvi"] = 1.1;
    mapa["Drugi"] = 2.2;

    if (mapa.brojElemenata() == 2) {
        mapa.obrisi("Prvi");

        if (mapa.brojElemenata() == 1 &&
            mapa["Prvi"] == 0.0 &&  // Default vrijednost za double je 0.0
            mapa["Drugi"] == 2.2) {
            std::cout << "Test 2 uspješan." << std::endl;
        } else {
            std::cout << "Test 2 nije uspješan." << std::endl;
        }
    } else {
        std::cout << "Test 2 nije uspješan." << std::endl;
    }
}

// Test 3: Brisanje nepostojećeg elementa
void testBrisanjeNepostojecog() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "Prvi";
    mapa[2] = "Drugi";

    if (mapa.brojElemenata() == 2) {
        try {
            mapa.obrisi(3);
        } catch (const std::out_of_range& e) {
            std::cout << "Izuzetak uhvaćen: " << e.what() << std::endl;

            if (mapa.brojElemenata() == 2) {
                std::cout << "Test 3 uspješan." << std::endl;
                return;
            }
        }
    }
    
    std::cout << "Test 3 nije uspješan." << std::endl;
}

// Test 4: Obrisi sve elemente
void testBrisanjeSvih() {
    NizMapa<char, std::string> mapa;
    mapa['a'] = "Alpha";
    mapa['b'] = "Beta";
    mapa['c'] = "Gamma";

    if (mapa.brojElemenata() == 3) {
        mapa.obrisi();

        if (mapa.brojElemenata() == 0) {
            std::cout << "Test 4 uspješan." << std::endl;
            return;
        }
    }

    std::cout << "Test 4 nije uspješan." << std::endl;
}

// Test 5: Provjera Performansi
void testPerformanse() {
    NizMapa<std::string, int> mapa;

    mapa["Jedan"] = 1;
    mapa["Dva"] = 2;
    mapa["Tri"] = 3;

    if (mapa.brojElemenata() == 3) {
        if (mapa["Jedan"] == 1 && mapa["Dva"] == 2 && mapa["Tri"] == 3) 
            std::cout << "Test 5 uspješan." << std::endl;
        else 
            std::cout << "Test 5 nije uspješan (vrijednosti elemenata iz mape)." << std::endl;
    } 
    else {
        std::cout << "Test 5 nije uspješan (broj elemenata)." << std::endl;
    }

    mapa["Cetiri"] = 4;
    mapa["Pet"] = 5;

    if (mapa.brojElemenata() == 5) {
        if (mapa["Cetiri"] == 4 && mapa["Pet"] == 5) 
            std::cout << "Test dodavanja novih elemenata uspješan." << std::endl;
        else 
            std::cout << "Test dodavanja novih elemenata nije uspješan (vrijednosti dodanih elemenata)." << std::endl;
    } 
    else {
        std::cout << "Test dodavanja novih elemenata nije uspješan (broj elemenata nakon dodavanja novih)." << std::endl;
    }
}

int main() {
    NizMapa<std::string, float> glavni_gradovi;
    glavni_gradovi["Bosna i Hercegovina"] = 123.45;
    glavni_gradovi["Hrvatska"] = 67.89;
    glavni_gradovi["Sjedinjene Američke Države"] = 987.65;

    std::cout << "Broj elemenata:" << glavni_gradovi.brojElemenata() << std::endl;

    std::cout << "Vrijednost za Hrvatska: " << glavni_gradovi["Hrvatska"] << std::endl;

    glavni_gradovi.obrisi("Hrvatska");

    std::cout << "Broj elemenata nakon brisanja: " << glavni_gradovi.brojElemenata() << std::endl<<std::endl;

    testDodavanjeIPristup();
    testBrisanje();
    testBrisanjeNepostojecog();
    testBrisanjeSvih();
    testPerformanse();

    std::cout << "Svi testovi prolaze." << std::endl;

    return 0;
}