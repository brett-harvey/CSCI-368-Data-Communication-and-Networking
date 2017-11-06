//============================================================================
// Name        : Lab06Sever.cpp
// Author      : Brett Harvey
// Version     : 1.0
// Description : Lab 06 Server
//============================================================================

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <windows.h>
#include  <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <process.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "42"
#define DEFAULT_BUFLEN 512

void do_chat(void * client_socket) {

	//SOCKET *client_s = (SOCKET)client_socket;
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	cout << GetCurrentThreadId() << ": The server will repeat any input from the client until 'bye' is entered" << endl;
	do{

		iResult = recv((SOCKET)client_socket, recvbuf, recvbuflen, NULL);

		if (iResult > 0) {
			recvbuf[iResult] = '\0';
			cout << "Bytes received: " << iResult << endl;
			cout << "Message received: " << recvbuf << endl;

			// Send message back to the sender

			iResult = send((SOCKET)client_socket, recvbuf, strlen(recvbuf), 0);
			if (iResult == SOCKET_ERROR) {
				cout << "Response failed with error: " << WSAGetLastError() << endl;
				closesocket((SOCKET)client_socket);
				WSACleanup();
				break;
			}
			//cout << "I'm doing well, thank you very much for asking." << endl;
		}
		else if (iResult == 0)
			cout << "Connection closed: " << GetCurrentThreadId() << endl;
		else  {
			cout << "recv failed with error: " << WSAGetLastError() << endl;
			closesocket((SOCKET)client_socket);
			WSACleanup();
			break;
		}

	} while (_strcmpi(recvbuf, "bye") != 0);
	// shutdown the connection for sending since no more data will be sent
	//closesocket(ListenSocket);
	iResult = shutdown((SOCKET)client_socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket((SOCKET)client_socket);
		WSACleanup();
	}

	// cleanup
	closesocket((SOCKET)client_socket);

	_endthread();
}

int main(){
	WORD wVersion = MAKEWORD(2, 2); // Used to request version 2.2 of Windows sockets
	WSADATA wsaData;
	int iResult;
	DWORD thread;
	//SOCKET sock;
	// Initialize Winsock
	iResult = WSAStartup(wVersion, &wsaData);   // Data loaded by WSAStartup
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl;   // Error check if WSAStartup successful
		return 1;
	}

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "getaddrinfo failed: " << iResult << endl;
		WSACleanup();
		return 1;
	}

	//SOCKET client = INVALID_SOCKET;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	// Create a socket to listen for a connection

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Set up the TCP listening socket

	iResult = bind(ListenSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "Error at bind(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Listen for the client

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else{
		cout << "Listening..." << endl;
	}

	// Accept a client socket
	

	//closesocket(ListenSocket);
	
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	//cout << "The server while repeat any input from the client until 'bye' is entered" << endl;

	do {
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		else{
			cout << "Waiting..." << endl;
		}

		if (_beginthread(do_chat, DEFAULT_BUFLEN, (void *)ClientSocket) < 0) {

			cout << "ERROR: Creating thread.\n";
			return 1;
		}

	} while (TRUE);

	// shutdown the connection for sending since no more data will be sent
	closesocket(ListenSocket);
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
}
