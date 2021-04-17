#include <iostream>
#include <string>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define MP make_pair
#define LISTEN_PORT 123
#define CAPACITY 100	// ����������� �������
#define SERVERNAME "__SERVER__"

using namespace std;

vector < pair <SOCKET, string> > connections; // ������ � ���� ��������
int nClients = 0;

enum Type {Login, Greeting, CommonMessage, PrivateMessage, UserCome, UserGone};

struct Message {
	Type type;
	char from[20];
	char to[20];
	char message[2048];
};

DWORD WINAPI connectToClient(LPVOID clSock) {
	SOCKET sClient = ((SOCKET*)clSock)[0];

	// ���������� ������� �������������
	Message login;
	login.type = Login;
	strcpy(login.from, SERVERNAME);
	strcpy(login.message,"������� �������: \0");
	send(sClient, (char*) &login, sizeof(login), 0);

	Message getNickname;
	int len = 0;
	if ( (len = recv(sClient, (char*) &getNickname, sizeof(getNickname), 0)) == SOCKET_ERROR) {
		cout << "������ �� ��������! \n";
		nClients--;
		closesocket(sClient);
		return -1;
	}
	
	string nickname = getNickname.from;
	// ����������� ����� �������� ������
	nClients++;	
	connections.push_back(MP(sClient, nickname));
	cout << "����� �����������! \n";
	cout << nClients << " �������������" << endl;
	int idx = nClients - 1;

	// ������� � �������� ����������� �������
	Message greeting;
	greeting.type = Greeting;	
	strcpy(greeting.from, SERVERNAME);
	string hello = "����� ���������� � ���, ";
	hello += nickname;
	strcpy(greeting.message, hello.c_str());
	send(sClient, (char*) &greeting, sizeof(greeting), 0);	
		

	// ������� � �������� ���� �������� ��������� � �������� �������� �������
	Message userCome;
	userCome.type = UserCome;
	strcpy(userCome.from, nickname.c_str());
	string uCmMsg = nickname;
	uCmMsg += " ����� � ���";
	strcpy(userCome.message, uCmMsg.c_str());
	for (int i = 0; i < connections.size(); i++) {
		if (i == idx) 
			continue;
		send(connections[i].first, (char*) &userCome, sizeof(userCome), 0);
	}

	// ���� ��������� ���������
	Message msg;
	while (true) {
		// �������� ��������� ��������� �� �������
		len = recv(sClient, (char*) &msg, sizeof(msg), 0);
		if (len == SOCKET_ERROR || (string)msg.message == "exit") {
			break;
			/*
			cout << "������ recv! \n" << WSAGetLastError();
			closesocket(sClient);
			WSACleanup();
			return -1;
			*/
		}


		// ���� ��� ������� ���������, �� �������� ��� ���� ��������
		if (msg.type == CommonMessage) {
			for (int i = 0; i < connections.size(); i++) {
				if (i == idx)
					continue;
				send(connections[i].first, (char*) &msg, sizeof(msg), 0);
			}
		}

		// ���� ��� ��������� ���������, �� �������� ��� ���������� �������
		if (msg.type == PrivateMessage) {
			string to = msg.to;
			for (int i = 0; i < connections.size(); i++) 
				if (connections[i].second == to)
					send(connections[i].first, (char*) &msg, sizeof(msg), 0);
		}
	}

	// ������ �����
	nClients--;
	// ������� � �������� ���� �������� ��������� � �������� �������� �������
	Message userGone;
	userGone.type = UserGone;
	strcpy(userGone.from, nickname.c_str());
	string uGnMsg = nickname;
	uGnMsg += " ������� ���";

	cout << uGnMsg << endl;
	cout << nClients << " �������������" << endl;

	strcpy(userGone.message, uGnMsg.c_str());
	for (int i = 0; i < connections.size(); i++) {
		if (i == idx) 
			continue;
		send(connections[i].first, (char*) &userGone, sizeof(userGone), 0);
	}
	closesocket(sClient);
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
	
	// �������� ��������������� ������
	SOCKET sListener;
	if ( (sListener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "������ socket! \n" << WSAGetLastError();
		WSACleanup(); 
		return -1;
	}

	// ������� ��������� ����� �������
	sockaddr_in srvAdr;
	srvAdr.sin_family = AF_INET;
	srvAdr.sin_port = htons(LISTEN_PORT);
	srvAdr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// ��������� ����� � ������� �������
	if (bind(sListener, (sockaddr*) &srvAdr, sizeof(srvAdr))) {
		cout << "������ bind! \n" << WSAGetLastError();
		closesocket(sListener);
		WSACleanup(); 
		return -1;
	}

	// �������������� ��������� �����
	if (listen(sListener, CAPACITY)) {
		cout << "������ listen! \n" << WSAGetLastError();
		closesocket(sListener);
		WSACleanup(); 
		return -1;
	}

	cout << "������ ����� � ������" << endl;

	// ������� ����� � ����� �������
	SOCKET sNewClient;		// ����� �������
	sockaddr_in adrClient;	// ����� ������� (���������� ������������� ��� ������)
	int len = sizeof(adrClient);

	//while (true) {
	while (true) {
		// ��������� �������� ����������
		if ( (sNewClient = accept(sListener, (sockaddr*) &adrClient, &len)) == INVALID_SOCKET) {
			cout << "������ accept! \n" << WSAGetLastError();
			break;
		}

		//������� ����� ����� ��� ������������ �������
		DWORD threadId;
		int number = connections.size();
		CreateThread(NULL, NULL, connectToClient, &sNewClient, NULL, &threadId);
	}

	closesocket(sListener);
	WSACleanup(); 
	return 0;
}