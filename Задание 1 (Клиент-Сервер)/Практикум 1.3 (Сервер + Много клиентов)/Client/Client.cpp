#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Student {
	char fam[20];
	int vals[4];
};

int main() {
	setlocale(LC_ALL, "rus");

	// Оформление окна
	cout << "\t Клиент\n";
	for (int i = 0; i < 30; i++) cout << "-";
	cout << endl;

	string name, forendl;
	cout << "\t Введите ник:\n";
	getline(cin, name);

	Student st;
	ofstream fout;
	string path = "..\\src\\" + name + ".bin";
	fout.open(path, ios::trunc);
	fout.close();
	cout << "Аккаунт создан\n";
	while (true) {
		cout << "Введите фамилию: ";
		cin.getline(st.fam, 20);
		cout << "Введите четыре оценки: ";
		for (int i = 0; i < 4; i++)
			cin >> st.vals[i];
		getline(cin, forendl);
		fout.open(path, ios::binary|ios::app);
		if (fout.is_open()) {
			fout.write((char*)&st, sizeof(st));
			fout.close();
			cout << "Добавлено\n";
		}
	}

	return 0;
}