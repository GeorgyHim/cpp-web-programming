#include <iostream>
#include <sstream>
#include <string>

// ��� ���������� ������ freeaddrinfo � MinGW
#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#define BUF_SIZE 1024

using std::cerr;
using namespace std;

int main() {
	setlocale(LC_ALL, "rus");
	// ���������� ����
	cout << "\t ����-������\n";
	for (int i = 0; i < 30; i++) 
		cout << "-";
	

	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		cerr << "������ WSAStartup! \n" << WSAGetLastError();
		return -1;
	}

	// IP-����� ���������� ������ �������
	addrinfo* addr = NULL;

	// ������ ��� ������������� ��������� ������
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// �������������� ���������, �������� ����� ������
	// ��� HTTP-������ ����� ������ �� 8000-� ����� localhost
	if (getaddrinfo("127.0.0.1", "8000", &hints, &addr)) {
		cerr << "������ getaddrinfo! \n" << WSAGetLastError();
		WSACleanup(); 
		return -1;
	}

	// ������� ��������� �����
	SOCKET listener = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (listener == INVALID_SOCKET) {
		cerr << "������ socket! \n" << WSAGetLastError();
		WSACleanup(); 
		return -1;
	}

	// ����������� ��������� ����� � IP-������ �������
	if (bind(listener, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR) {
		cerr << "������ bind! \n" << WSAGetLastError();
		closesocket(listener);
		freeaddrinfo(addr);
		WSACleanup(); 
		return -1;
	}

	// �������������� ��������� �����
	if (listen(listener, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "������ bind! \n" << WSAGetLastError();
		closesocket(listener);
		freeaddrinfo(addr);
		WSACleanup(); 
		return -1;
	}

	char buf[BUF_SIZE] = {};
	SOCKET sClient;
	// ���� ������ ���������
	while (true) {
		// ��������� �������� ���������� 
		if ((sClient = accept(listener, NULL, NULL)) == INVALID_SOCKET) {
			cerr << "������ accept! \n" << WSAGetLastError();
			closesocket(listener);
			freeaddrinfo(addr);
			WSACleanup(); 
			return -1;
		}

		int len = recv(sClient, (char*) buf, BUF_SIZE, 0);

		if (len == SOCKET_ERROR) {
			cerr << "������ recv! \n" << WSAGetLastError();
            closesocket(sClient);
		} 
		if (len == 0) {
			cout << "���������� ������� ��������" << endl;
		}
		else {
			cout << "������������� ����� ������!\n";

			// ������ ����� ����� ������ � ������ �������
			buf[len] = '\0';

			// ��������� ����� � ������� HTML
			std::stringstream responseBody, response;
			// ���� ������
			responseBody << "<title>Test C++ HTTP Server</title>\n"
                << "<h1>Test page</h1>\n"
                << "<p>This is body of the test page...</p>\n"
                << "<h2>Request headers</h2>\n"
                << "<pre>" << buf << "</pre>\n"
                << "<em><small>Test C++ Http Server</small></em>\n";

			// ���� �����
			response << "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
				<< "Content-Length: " << responseBody.str().length()
				<< "\r\n\r\n" << responseBody.str();

			// ���������� ����� �������
			if (send(sClient, response.str().c_str(), 
				response.str().length(), 0) == SOCKET_ERROR) {
					cerr << "������ send! \n" << WSAGetLastError();
			} 

			// ��������� ����������
			closesocket(sClient);
		}
	}

	closesocket(listener);
	freeaddrinfo(addr);
	WSACleanup(); 
	return 0;
}