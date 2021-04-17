#include <iostream>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define LISTENPORT 666
#define BUF_SIZE 2048

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

int nClients = 0;

void printUsers() {
	if (nClients > 0) 
		cout << nClients << " users online" << endl;
	else
		cout << "No users online" << endl;	
	cout << endl;
}

// �������� ������� ������, ������������� �������������� �������������
DWORD WINAPI connectToClient(LPVOID clientSocket) {
	SOCKET cSocket = ((SOCKET*) clientSocket)[0];
	Message msg;
	char greeting[] = "������ ����! ������ ����� � ������ \r\n";
	int len = 0;

	// ���������� ��������� �������
	send(cSocket, greeting, sizeof(greeting), 0);

	while ( (len = recv(cSocket, (char*) &msg, sizeof(msg), 0)) != SOCKET_ERROR) {
		if (msg.type == 1) {
			cout << "(������������): " << msg.message << endl << endl;
			string ans = "�������� ���������: ";
			ans += msg.message;
			send(cSocket, ans.c_str(), ans.length() , 0);
		}
		if (msg.type == 2) {
			Student y = msg.st;

			if (string(y.fam) == "exit")
				break;
			
			cout << "�������� ������ �� ������������ �������� " << y.fam << endl;
			int mnv = 5;
			for (int i = 0; i < 4; i++)
				mnv = min(mnv, y.vals[i]);

			string ans;
			if (mnv <= 2) 
				ans = "����� �������������!";
			if (mnv == 3) 
				ans = "��� ��������������, ��� ���������";
			if (mnv == 4)
				ans = "��� ��������������, ���� ���������: 1500 ������";
			if (mnv >= 5)
				ans = "��� ��������������, ���� ���������: 2200 ������";
			send(cSocket, ans.c_str(), ans.length() , 0);
		}
	}

	// ������ �����
	nClients--;
	cout << "������������ �����" << endl;
	printUsers();
	closesocket(cSocket);
	return 0;
}

int main() {
	setlocale(LC_ALL, "rus");
	// ���������� ����
	cout << "\t ������\n";
	for (int i = 0; i < 30; i++) 
		cout << "-";
	cout << endl;

	// �������������� ���������� �������
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		cout << "������ WSAStartup! \n" << WSAGetLastError();
		return -1;
	}

	// �������� ������
	SOCKET sListener;
	if ( (sListener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "������ socket! \n" << WSAGetLastError();
		WSACleanup(); 
		return -1;
	}

	// ������� ��������� ����� �������
	sockaddr_in sLocal;
	sLocal.sin_family = AF_INET;
	sLocal.sin_port = htons(LISTENPORT);
	sLocal.sin_addr.s_addr = 0;

	// ��������� ����� � ������� �������
	if ( bind(sListener, (sockaddr *) &sLocal, sizeof(sLocal)) ) {
		cout << "������ bind! \n" << WSAGetLastError();
		closesocket(sListener);
		WSACleanup(); 
		return -1;
	}

	// �������������� ��������� �����
	if (listen(sListener, 0x100)) {
		cout << "������ listen! \n" << WSAGetLastError();
		closesocket(sListener);
		WSACleanup(); 
		return -1;
	}

	cout << "Waiting for connection..." << endl;

	// ������� ����� � ����� �������
	SOCKET sClient;		// ����� �������
	sockaddr_in adrClient;	// ����� ������� (���������� ������������� ��� ������)
	char buf[BUF_SIZE + 1] = {0};
	int len = sizeof(adrClient);

	// ���� ������ ��������
	while (true) {
		// ��������� �������� ����������
		if ( (sClient = accept(sListener, (sockaddr*) &adrClient, &len)) == INVALID_SOCKET) {
			cout << "������ accept! \n" << WSAGetLastError();
			closesocket(sListener);
			WSACleanup(); 
			return -1;
		}

		// ����������� ����� �������� ������
		nClients++;
		cout << "����� �����������! \n";

		// �������� �������� � �������������� �������
		HOSTENT* hst = gethostbyaddr((char*) &adrClient.sin_addr.s_addr, 4, AF_INET);		
		
		// ������� �������� � �������������� ������� � ����� ����� ��������
		if (hst)	
			cout << hst->h_name << ' ';
		cout << inet_ntoa(adrClient.sin_addr) << endl;
		printUsers();

		//������� ����� ����� ��� ������������ �������
		DWORD threadId;
		CreateThread(NULL, NULL, connectToClient, &sClient, NULL, &threadId);
	}

	closesocket(sListener);
	WSACleanup(); 
	return 0;
}