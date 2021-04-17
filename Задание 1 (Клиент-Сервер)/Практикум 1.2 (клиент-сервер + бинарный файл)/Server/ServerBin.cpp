#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Student {
	char fam[20];
	int vals[4];
};

int main() {
	setlocale(LC_ALL, "rus");
	cout << "\t Сервер\n";
	for (int i = 0; i < 30; i++) cout << "-";
	cout << endl;
	ifstream fin;
	int r = 0;
	while (true) {		
		fin.open("..\\client.bin", ios::binary);
		if (fin.is_open()) {
			fin.seekg(0, fin.end);
			int sz = fin.tellg();
			fin.close();
			if (sz > r) {
				fin.open("..\\client.bin", ios::binary);
				if (fin.is_open()) {
					fin.seekg(r);
					r = sz;
					Student y;
					fin.read((char*) &y, sizeof(y));
					fin.close();
					cout << y.fam << ": ";
					int mnv = 5;
					for (int i = 0; i < 4; i++)
						mnv = min(mnv, y.vals[i]);

					if (mnv <= 2) 
						cout << "Имеет задолженность!" << endl;
					if (mnv == 3) 
						cout << "Нет задолженностей, нет стипендии" << endl;
					if (mnv == 4)
						cout << "Нет задолженностей, есть стипендия: 1500 рублей" << endl;
					if (mnv >= 5)
						cout << "Нет задолженностей, есть стипендия: 2200 рублей" << endl;
				}
			}
		}
	}
	return 0;
}