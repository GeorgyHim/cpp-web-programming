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

	// ���������� ����
	cout << "\t ������\n";
	for (int i = 0; i < 30; i++) cout << "-";
	cout << endl;

	string name, forendl;
	cout << "\t ������� ���:\n";
	getline(cin, name);

	Student st;
	ofstream fout;
	string path = "..\\src\\" + name + ".bin";
	fout.open(path, ios::trunc);
	fout.close();
	cout << "������� ������\n";
	while (true) {
		cout << "������� �������: ";
		cin.getline(st.fam, 20);
		cout << "������� ������ ������: ";
		for (int i = 0; i < 4; i++)
			cin >> st.vals[i];
		getline(cin, forendl);
		fout.open(path, ios::binary|ios::app);
		if (fout.is_open()) {
			fout.write((char*)&st, sizeof(st));
			fout.close();
			cout << "���������\n";
		}
	}

	return 0;
}