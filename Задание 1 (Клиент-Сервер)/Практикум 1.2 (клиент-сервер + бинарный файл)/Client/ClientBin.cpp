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
	cout << "\t Клиент\n";
	for (int i = 0; i < 30; i++) cout << "-";
	cout << endl;

	Student st;
	ofstream fout;
	fout.open("..\\client.bin", ios::trunc);
	fout.close();
	while (true) {
		cout << "Введите фамилию: ";
		cin.getline(st.fam, 20);
		cout << "Введите четыре оценки: ";
		for (int i = 0; i < 4; i++)
			cin >> st.vals[i];
		string s;
		getline(cin, s);
		fout.open("..\\client.bin", ios::binary|ios::app);
		if (fout.is_open()) {
			fout.write((char*)&st, sizeof(st));
			fout.close();
			cout << "Добавлено\n";
		}
	}
	return 0;
}