#include <iostream>
#include <vector>

using namespace std;

template <typename Tip>
class Stek {
private:
    std::vector<Tip> stek;

    bool jePrazan() const {
        return stek.empty();
    }

public:
    Stek(){};

    // Kopirajući konstruktor
    Stek(const Stek& s) {
        stek = s.stek;
    }

    // Operator dodjele
    Stek& operator=(const Stek& s) {
        if (this != &s) {
            stek = s.stek;
        }
        return *this;
    }

    void stavi(const Tip &el) {
        stek.push_back(el);
    }

    Tip skini() {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        Tip temp = stek.back();
        stek.pop_back();
        return temp;
    }

    Tip& vrh() {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        return stek.back();
    }

    Tip vrh() const {
        if (jePrazan()) {
            throw std::logic_error("Stek je prazan.");
        }
        return stek.back();
    }

    int brojElemenata() const {
        return stek.size();
    }
};

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

/*int main() {
    Stek<std::vector<int>> s;
    s.stavi({1, 2, 3, 4, 5});
    s.stavi({6, 8});
    s.stavi({9, 10, 130});
    s.stavi({157, 226, 3424});

    test1(s);
    test2(s);
    test3(s);
    return 0;
}*/