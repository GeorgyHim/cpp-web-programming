#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <Windows.h>
#include <string>

using namespace std;

struct Student {
	char fam[20];
	int vals[4];
};

void processFile(ifstream& fin, int& r, int sz, string path) {
	fin.open(path, ios::binary);
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

int isPressed(int Key) {
	if (GetAsyncKeyState(Key) & 0x8000)
		return Key;
	else
		return 0;
}

int main() {
	setlocale(LC_ALL, "rus");

	//Очистка папки src
	WIN32_FIND_DATAA wfd;
	HANDLE hf = FindFirstFileA("..\\src\\*", &wfd);
	if (hf != INVALID_HANDLE_VALUE) {
		do {
			if (wfd.cFileName[0] != '.') {
				string path = wfd.cFileName;
				path = "..\\src\\" + path;
				DeleteFileA(path.c_str());
			}
		} while (FindNextFileA(hf,&wfd)!=0);
		FindClose(hf);
	}

	// Оформление окна
	cout << "\t Сервер\n";
	cout << "Команды:\n";
	cout << "A - Добавить клиента\n";
	cout << "E - Завершить программу\n";
	for (int i = 0; i < 30; i++) cout << "-";
	cout << endl;
	
	map <string, int> r; // размеры файлов клиентов
	vector <PROCESS_INFORMATION> pi; // Вектор дескрипторов дочерних процессов
	while (true) {
		// Начинаем проверку всех файлов на обновление
		Student st;
		hf = FindFirstFileA("..\\src\\*", &wfd);
		if (hf != INVALID_HANDLE_VALUE) {
			do {
				if (wfd.cFileName[0] != '.') {
					string path = wfd.cFileName;					
					path = "..\\src\\" + path;
					ifstream fin;
					fin.open(path, ios::binary);
					if (fin.is_open()) {
						fin.seekg(0, fin.end);
						int sz = fin.tellg();
						fin.close();
						if (sz > r[path])
							processFile(fin, r[path], sz, path);
					}
				}				
			} while (FindNextFileA(hf,&wfd)!=0);
			FindClose(hf);
		}

		if (!_kbhit())
			continue;
		char c;
		c = _getch();
		if (c == 'A' || c == 'E') {
			cout << c << endl;
		}

		if (c == 'A') {
			STARTUPINFO si = {sizeof(si)};				
			PROCESS_INFORMATION _pi;
			pi.push_back(_pi);
			CreateProcess(L"..\\debug\\Client.exe", NULL, NULL, NULL, FALSE, 
				CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi.back());
			continue;
		}	
		if (c == 'E') {
			for (int i = 0; i < pi.size(); i++) {
				TerminateProcess(pi[i].hProcess, 0);
				CloseHandle(pi[i].hProcess);
				CloseHandle(pi[i].hThread);
			}
			break;
		}
	}
	return 0;
}