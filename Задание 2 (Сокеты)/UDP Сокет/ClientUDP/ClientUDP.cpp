#include <iostream> 
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define PORT 666 // ���� �������
#define SRV_ADDR "127.0.0.1"
#define BUF_SIZE 1024

using namespace std;

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

	
	HOSTENT* hst;
	sockaddr_in srvAddr;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(PORT);

	// ����������� IP-������ �������
	if (inet_addr(SRV_ADDR)) {
		srvAddr.sin_addr.s_addr=inet_addr(SRV_ADDR);
	}
	else {
		if (hst = gethostbyname(SRV_ADDR)) {
			srvAddr.sin_addr.s_addr = ((unsigned long **) hst->h_addr_list)[0][0];
		}
		else   {   
			cout << "Unknown Host: "<< WSAGetLastError()<< "\n";
            closesocket(s);
            WSACleanup();
            return -1;        
		}
	}

	// ����� ����������� � ��������
	string msg;
	while (true) {
		cout << "������� ���������: " << endl;
		getline(cin, msg);
		if (msg == "exit") {
			break;
		}

		// �������� ���������
		sendto(s, (char*) &msg[0], msg.size(), 0, (sockaddr*) &srvAddr, sizeof(srvAddr));

		// ����� ������
		sockaddr_in ansAddr;
		char buf[BUF_SIZE] = {0};
		int len = sizeof(ansAddr);
		int bsz = recvfrom(s, buf, sizeof(buf), 0, (sockaddr*) &ansAddr, &len);
		if (bsz == SOCKET_ERROR) {
			cout << "������ ������ ���������! \n" <<WSAGetLastError()<< "\n";
			closesocket(s);
			WSACleanup();  
			return -1;  
		}
		buf[bsz] = '\0';
		cout << "Answer: " << buf << endl;
	}
	closesocket(s);
	WSACleanup();  
	return 0;
}
