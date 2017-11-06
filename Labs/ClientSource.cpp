//============================================================================
// Name        : lab6Client.cpp
// Author      : Brett Harvey
// Version     : 1.0
// Description : Lab 06 Client
//============================================================================

#include <iostream>
#include<winSock2.h>
#include<wS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define DEFAULT_PORT "42"
#define DEFAULT_BUFLEN 4096

int main(){

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	int recvbuflen = DEFAULT_BUFLEN;

	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];

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
	//struct addrinfo *result = NULL;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);  // see the sample page
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
	else{
		cout << "Connected..." << endl;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	//freeaddrinfo(result);

	cout << "I am client. To exit from this program, type 'bye'" << endl;
	do{
		cout << "Client: ";
		gets_s(sendbuf, sizeof(sendbuf));

		// Send an initial buffer
		iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), ptr->ai_flags);
		if (iResult == SOCKET_ERROR) {
			cout << "send failed: " << WSAGetLastError() << endl;
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		cout << "Bytes Sent: " << iResult << endl;

		// Receive data until the server closes the connection
		//		do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, ptr->ai_flags);
		if (iResult > 0){
			cout << "Bytes received: " << iResult << endl;
			recvbuf[iResult] = '\0';
			cout << recvbuf << endl;
		}
		else if (iResult == 0)
			cout << "Connection closed\n";
		else
			cout << "recv failed: " << WSAGetLastError() << endl;
		//		} while (iResult > 0);

	} while (_strcmpi(sendbuf, "bye") != 0);


	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	// cleanup
	closesocket(ConnectSocket);
}