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
		cout << "������� �������: ";
		cin.getline(&fam[0], 20);
		cout << "������� ������ ������: ";
		cin.getline(vals, 50);
		try {
			fout.open("..\\client.txt", ios::app);
		}
		catch (exception &e) {
			cout << "Error! ������ ������!" << endl;
			continue;
		}
		fout << fam << endl << vals << endl;		
		fout.close();
		cout << "��������� \n";
	}
	return 0;
}