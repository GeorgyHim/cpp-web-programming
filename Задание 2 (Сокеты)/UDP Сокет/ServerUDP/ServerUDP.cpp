#include <iostream> 
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define PORT 666 // ���� �������
#define BUF_SIZE 1024

using namespace std;

string greeting = "Hello, Student";

int main() {
	setlocale(LC_ALL, "rus");

	// ���������� ����
	cout << "\t UDP-������\n";
	for (int i = 0; i < 30; i++) 
		cout << "-";
	cout << endl;

	// �������������� ���������� 
	char buff[1024]; 
	if (WSAStartup(0x0202,(WSADATA *) &buff[0]))     {
		cout << "������ �������������! \n" << WSAGetLastError();
		return -1;
	}

	// �������� ������ UDP
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "������ �������� ������! \n" << WSAGetLastError();
		return -1;
	}

	// �������� ��������� ���������� ������	������
	sockaddr_in sAddr;

	// ��������� ��������� ������ ��� �������
	sAddr.sin_family = AF_INET;	
	sAddr.sin_addr.s_addr = INADDR_ANY;	// ~ clntSin.sin_addr.S_un.S_addr
	sAddr.sin_port = htons(PORT);


	
	// ��������� ����� � ������� � ������� �����
	if (bind(s, (sockaddr*) &sAddr, sizeof(sAddr))) {
		cout << "������ �������� ������! \n" << WSAGetLastError();
		return -1;
	}

	char buf[BUF_SIZE] = {0};
	int k = 1;
	// ���� ��������� �������
	while (true) {
		sockaddr_in from;
		int len = sizeof(from);
		int bsz = recvfrom(s, &buf[0], BUF_SIZE-1, 0, (sockaddr*) &from, &len);
		if (bsz == SOCKET_ERROR) {
			cout << "������ ������ ���������! \n" << WSAGetLastError();
			return -1;
		}

		// ���������� ��������� �������
		HOSTENT* hst;
		hst = gethostbyaddr((char*) &from.sin_addr, 4, AF_INET);
		cout << "New Datagram: \n" << ((hst) ? hst->h_name : "Unknown host") << endl 
			<< inet_ntoa(from.sin_addr) << endl << ntohs(from.sin_port) << endl;

		buf[bsz] = '\0';
		cout << "Datagram: " << buf << endl;

		string msg = "Datagramm " + to_string(k) + " is received: " + buf;
		k++;
		sendto(s, (char*) &msg[0], msg.size(), 0, (sockaddr*) &from, sizeof(from));
	}
	return 0;
}

