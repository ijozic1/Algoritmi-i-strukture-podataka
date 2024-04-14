#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

void Goldbach(int n, int& p, int& q) {
	if (n%2==1) throw "Broj nije paran";
	if (n<=2) throw "Broj <= 2";
	if (n==4) {
		p=2; q=2; return;
	}
	
	p=q=0;
	vector<int> prosti;
	
	for (int i(2); i<n; i++) {
		bool found=true;
		for (int j(2); j<i; j++)
			if (i%j==0) found=false;
		if (found) prosti.push_back(i);
	}
	
	for (int i(0); i<prosti.size(); i++) {
		for (int j(0); j<prosti.size(); j++) {
			if (prosti[i]+prosti[j] == n) {
				p=prosti[i];
				q=prosti[j];
			}
		}
	}
	
	if (p==0) throw "Hipoteza ne vazi";
}

void Goldbach2(int n, int& p, int& q) {
	if (n%2==1) throw "Broj nije paran";
	if (n<=2) throw "Broj <= 2";
	if (n==4) {
		p=2; q=2; return;
	}
	
	for (int i(2); i<n/2; i++) {
		bool found=true;
		for (int j(2); j<i/2; j++)
			if (i%j==0) { found=false; break; }
		if (!found) continue;
		for (int j(2); j<(n-i)/2; j++)
			if ((n-i)%j==0) { found=false; break; }
		if (!found) continue;
		p=i;
		q=(n-i);
		return;
	}
	throw "Hipoteza ne vazi";
}

int main() {
	int n,p,q;
	cout<<"Unesite n: ";
	cin>>n;
	try {
		clock_t vrijeme1 = clock();
		
		Goldbach(n,p,q);
		
		clock_t vrijeme2 = clock();
		
		int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
		cout<<"Vrijeme izvrsenja: "<<ukvrijeme<<" ms."<<endl;
		
		cout<<"p="<<p<<" q="<<q<<endl;
		
	} catch (const char e[]) {
		cout<<"Greska: "<<e<<endl;
	}
	return 0;
}