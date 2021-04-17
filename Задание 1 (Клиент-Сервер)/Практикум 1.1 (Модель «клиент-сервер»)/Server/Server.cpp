#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");
	ifstream fin;
	ofstream fout;
	fout.open("..\\server.txt", ios::trunc);
	fout.close();
	int r = 0;
	while (true) {		
		try {
			fin.open("..\\client.txt");
		}
		catch (exception &e) {
			cout << "Error! ������ ������!" << endl;
			continue;
		}
		
		fin.seekg(0, fin.end);
		int sz = fin.tellg();
		fin.close();
		if (sz > r) {
			try {
			fin.open("..\\client.txt");
			}
			catch (exception &e) {
				cout << "Error! ������ ������!" << endl;
				continue;
			}
			fin.seekg(r);
			r = sz;
			char fam[20];
			fin.getline(&fam[0], 20);
			cout << "�������: " << fam << endl;
			int v[4];			
			int mnv = 5;
			for (int i = 0; i < 4; i++) {
				fin >> v[i];
				mnv = min(mnv, v[i]);
				cout << v[i] << ' ';
			}					
			fin.close();
			cout << endl;	

			fout.open("..\\server.txt", ios::app);
			if (fout.is_open()) {
				fout << fam << ": ";
				if (mnv <= 2) 
					fout << "����� �������������!" << endl;
				if (mnv == 3) 
						fout << "��� ��������������, ��� ���������" << endl;
				if (mnv == 4)
							fout << "��� ��������������, ���� ���������: 1500 ������" << endl;
				if (mnv == 5)
					fout << "��� ��������������, ���� ���������: 2200 ������" << endl;
				fout.close();
				cout << "���������� \n";
			}
		}
	}
	return 0;
}