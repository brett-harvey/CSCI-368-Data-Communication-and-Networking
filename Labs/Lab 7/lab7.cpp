#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include<fstream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 70000
#define DEFAULT_PORT "80"
#define GET_STRING "GET http://cse.stfx.ca/~pdsec13/pics/scalable_computing.jpg /HTTP/1.1\r\nHost: cse.stfx.ca\r\nConnection: close\r\n\r\n" //The link that is present in this line is interchangeable, however the one seen here is for the php
#define SERVER_ADDR "cse.stfx.ca"

int main()
{
	cout << "HELLO I AM A CLIENT" << endl;
	WORD wVersion = MAKEWORD(2, 2); // Used to request version 2.2 of Windows sockets
	WSADATA wsaData;
	int iResult;

	string response;

	int resp_leng;
	char buffer[DEFAULT_BUFLEN];

	// Initialize Winsock
	iResult = WSAStartup(wVersion, &wsaData);   // Data loaded by WSAStartup
	if (iResult != 0)
	{
		cout << "WSAStartup failed: " << iResult << endl;   // Error check if WSAStartup successful
		return 1;
	}

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints)); // make sure the struct is empty; same as memset()
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_protocol = IPPROTO_TCP; // Set IP Protocol = TCP




	// Resolve the server address and port
	iResult = getaddrinfo(SERVER_ADDR, DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		cout << "getaddrinfo failed: " << iResult << endl;
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		cout << "Client: Error at socket connect(): " << WSAGetLastError() << endl;
		cout << "Client: Unable to connect to server!" << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//connect
	iResult = send(ConnectSocket, GET_STRING, strlen(GET_STRING), 0);


	if (iResult == SOCKET_ERROR)
	{
		cout << "send failed: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	ofstream picOutput;
	picOutput.open("Lab 7.jpg", std::ios::binary);
	ofstream outputFile("data.txt", ios::out);
	boolean flag = false;
	char * pntr;
	int p;

	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0){
			cout << "Bytes received: " << iResult << endl;
			recvbuf[iResult] = '\0';
			// Since iResult is the length of the recvbuf
			// This places a NULL character at the end to stop unecessary output
			pntr = strstr(recvbuf, "jpeg");
			if (pntr != NULL || flag == true) {
				if (flag != true){
					pntr = strstr(recvbuf, "\r\n\r\n");
					p = pntr - recvbuf;
				}
				char *dpntr = strstr(pntr, "\n");
				picOutput.write(dpntr + 3, iResult - p);
				flag = true;
				cout << strlen(dpntr);
			}
			else {

				outputFile.write(recvbuf, iResult);
				cout << recvbuf << endl;

			}
		}
		else if (iResult == 0)
			cout << "Connection closed\n";
		else
			cout << "recv failed: " << WSAGetLastError() << endl;
	} while (iResult > 0);
	outputFile.close();

	freeaddrinfo(result);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
