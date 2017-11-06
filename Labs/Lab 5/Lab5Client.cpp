//============================================================================
// Name        : Lab5LClient.cpp
// Author      : Brett Harvey
// Version     :
// Copyright   : Your copyright notice
// Description : Lab 03 in C++, Ansi-style
//============================================================================

#include "stdafx.h";
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> 
#define DEFAULT_BUFLEN 2048 
#define DEFAULT_PORT "42"
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main() {
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	int recvbuflen = DEFAULT_BUFLEN;

	char sendbuf[DEFAULT_BUFLEN]; 
	//char *sendbuf = "GET / HTTP / 1.0\n\n";

	char recvbuf[DEFAULT_BUFLEN];

	string myInput;

	int iResult;

	WORD wVersion = MAKEWORD(2, 2); // Used to request version 2.2 of Windows sockets
	WSADATA wsaData;

	// Initialize Winsock
	iResult = WSAStartup(wVersion, &wsaData);   // Data loaded by WSAStartup
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl;   // Error check if WSAStartup successful
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints)); // make sure the struct is empty; same as memset()
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_protocol = IPPROTO_TCP; // Set IP Protocol = TCP

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
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

	if (ConnectSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "Client: Error at socket connect(): " << WSAGetLastError() << endl;
		cout << "Client: Unable to connect to server!" << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message
	cout << "Connected to Server!" << endl;
	do {
		cout << "Please enter a value: ";
		myInput = gets_s(sendbuf, sizeof(sendbuf));
		cout << sendbuf << endl; 

		// Send an initial buffer#define DEFAULT_PORT "80"
		iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), ptr->ai_flags);
		if (iResult == SOCKET_ERROR) {
			cout << "send failed: " << WSAGetLastError() << endl;
			closesocket(ConnectSocket);
			WSACleanup();
			//return 1;
		}

		// cout << "Bytes S    #define DEFAULT_BUFLEN 2048";

		// Receive data until the server closes the connection
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, ptr->ai_flags);
		if (iResult > 0){
			cout << "Bytes received: " << iResult << endl;
			recvbuf[iResult] = '\0';
			cout << "From server: " << recvbuf << endl;
		}
		else if (iResult == 0)
			cout << "Connection closed\n";
		else
			cout << "recv failed: " << WSAGetLastError() << endl;
	} while (myInput != "bye");

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	system("Pause");
	// cleanup
	closesocket(ConnectSocket);
	return 0;
}
