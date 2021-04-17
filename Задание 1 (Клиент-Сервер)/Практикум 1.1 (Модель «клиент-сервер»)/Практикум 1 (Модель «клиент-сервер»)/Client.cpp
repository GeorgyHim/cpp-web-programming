#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");
	ofstream fout;
	fout.open("..\\client.txt", ios::trunc);
	fout.close();
	while (true) {
		char fam[20];
		char *vals = new char[7];
		cout << "Введите фамилию: ";
		cin.getline(&fam[0], 20);
		cout << "Введите четыре оценки: ";
		cin.getline(vals, 50);
		try {
			fout.open("..\\client.txt", ios::app);
		}
		catch (exception &e) {
			cout << "Error! Ошибка чтения!" << endl;
			continue;
		}
		fout << fam << endl << vals << endl;		
		fout.close();
		cout << "Добавлено \n";
	}
	return 0;
}