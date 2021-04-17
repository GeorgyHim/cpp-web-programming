#include <iostream>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define BUF_SIZE 2048

#define SRV_PORT 666
#define SRV_ADDR "127.0.0.1"

using namespace std;

struct Student {
	char fam[20];
	int vals[4];
};

struct Message {
	int type;
	char message[256];
	Student st;
};


int main() {
	setlocale(LC_ALL, "rus");
	// ���������� ����
	cout << "\t ������\n";
	cout << "���� �������������� � ��������: " << endl;
	cout << "1: ���-������" << endl;
	cout << "2: �������" << endl;
	cout << "������� ������: exit \n";
	for (int i = 0; i < 30; i++) 
		cout << "-";
	cout << endl;

	int type;
	cout << "�������� ��� �������������� � ��������: ";
	cin >> type;
	string str;		
	getline(cin, str); // ��������� ������� ������

	// �������������� ���������� �������
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		cout << "������ WSAStartup! \n" << WSAGetLastError();
		return -1;
	}

	// �������� ������
	SOCKET s;
	if ( (s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "������ socket! \n" << WSAGetLastError();
		WSACleanup(); 
		return -1;
	}

	// �������� ������ � ����� �������
	sockaddr_in srvAddr;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(SRV_PORT);

	HOSTENT* hst;

	// �������������� IP-������	�� ����������� � ������� ������
	if (inet_addr(SRV_ADDR) != INADDR_NONE) 
		srvAddr.sin_addr.s_addr = inet_addr(SRV_ADDR);
	else {
		// ������� �������� IP-����� �� ��������� �����
		if (hst = gethostbyname(SRV_ADDR))
			((unsigned long *)&srvAddr.sin_addr)[0] = 
			        ((unsigned long **)hst->h_addr_list)[0][0];
		else {
			cout << "�������� ������� �����! \n" << WSAGetLastError();
			closesocket(s);
			WSACleanup();
			return -1;
		}
	}

	// ������������ ����������
	if (connect(s, (sockaddr*) &srvAddr, sizeof(srvAddr))) {
		cout << "������ connect! \n" << WSAGetLastError();
		closesocket(s);
		WSACleanup();
		return -1;
	}

	int len = 0;
	char buf[BUF_SIZE + 1] = {0};
	// ���� ��������� ���������
	while (true) {
		len = recv(s, &buf[0], BUF_SIZE, 0);
		if (len == SOCKET_ERROR) {
			cout << "������ recv! \n" << WSAGetLastError();
			closesocket(s);
			WSACleanup();
			return -1;
		}

		// ����� ��������� �� �������
		buf[len] = '\0';
		cout << "(������): " << buf << endl;

		
		Message msg;
		msg.type = type;
		
		if (type == 1) {
			cout << "������� ���������: " << endl;
			// ���� ��������� ������������
			cin.getline(msg.message, 255);	
			if (string(msg.message) == "exit") {
				// �������� ���������
				send(s, (char*) &msg, sizeof(msg), 0);
				cout << "Exit... \n"; 
				break;
			}
		}
		if (type == 2) {
			cout << "������� �������: ";
			cin.getline(msg.st.fam, 20);
			if (string(msg.st.fam) == "exit") {
				// �������� ���������
				send(s, (char*) &msg, sizeof(msg), 0);
				cout << "Exit... \n"; 
				break;
			}
			cout << "������� ������ ������: ";
			for (int i = 0; i < 4; i++)
				cin >> msg.st.vals[i];
			getline(cin, str); // ������� ������
		}

		// �������� ���������
		send(s, (char*) &msg, sizeof(msg), 0);

	}

	closesocket(s);
	WSACleanup();
	return 0;
}