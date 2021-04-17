#include <iostream> 
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define SRV_HOST "localhost"
#define SRV_PORT 1234
#define CLNT_PORT 1235
#define BUF_SIZE 64

using namespace std;

struct Student {
	char fam[20];
	int vals[4];
};


int main() {
	setlocale(LC_ALL, "rus");
	// ���������� ����
	cout << "\t ������\n";
	for (int i = 0; i < 30; i++) 
		cout << "-";
	cout << endl;
	
	// �������������� ���������� 
	char buff[1024]; 
	if (WSAStartup(0x0202,(WSADATA *) &buff[0]))     {
		cout << "������ �������������! \n" << WSAGetLastError();
		return -1;
	}

	// �������� ������ TCP
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "������ �������� ������! \n" << WSAGetLastError();
		return -1;
	}

	// �������� ��������� ���������� ������	������� ������� � �������
	sockaddr_in clntSin, srvSin;

	// ��������� ��������� ������ ��� �������
	clntSin.sin_family = AF_INET;	
	clntSin.sin_addr.s_addr = 0;	// ~ clntSin.sin_addr.S_un.S_addr
	clntSin.sin_port = htons(CLNT_PORT);

	// ��������� ����� � ������� � ������� �����
	if (bind(s, (sockaddr*) &clntSin, sizeof(clntSin))) {
		cout << "������ �������� ������! \n" << WSAGetLastError();
		return -1;
	}

	// ��������� ��������� ������ ��� �������
	hostent* hp;
	srvSin.sin_family = AF_INET;
	srvSin.sin_port = htons(SRV_PORT);
	hp = gethostbyname(SRV_HOST);
	((unsigned long *)&srvSin.sin_addr)[0]=
        ((unsigned long **)hp->h_addr_list)[0][0];

	// ������������� ���������� � ��������
	connect(s, (sockaddr*) &srvSin, sizeof(srvSin));

	// ���� ������� � ��������
	int len = 0;
	char buf[BUF_SIZE] = {0};
	do {
		len = recv(s, (char*) &buf, BUF_SIZE, 0);
		if (len == SOCKET_ERROR) {
			cout << "������ ������ ���������! \n" << WSAGetLastError();
			return -1;
		}

		buf[len] = 0; // ������������� ������ �����
		cout << '"' << buf  << '"' << endl;

		Student st;
		cout << "������� �������: ";
		cin.getline(st.fam, 20);
		cout << "������� ������ ������: ";
		for (int i = 0; i < 4; i++)
			cin >> st.vals[i];
		string ss;
		getline(cin, ss); // ������� ������

		// �������������� ��������� � ������ ������
		char* msg = (char*) &st;
		send(s, (char*) &msg[0], sizeof(Student), 0);

	} while (true);

	cout << "Conversation is over" << endl;
	closesocket(s);
	return 0;
}