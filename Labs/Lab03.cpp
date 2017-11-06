//============================================================================
// Name        : cpp.cpp
// Author      : Brett Harvey
// Version     :
// Copyright   : Your copyright notice
// Description : Lab 03 in C++, Ansi-style
//============================================================================

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#define DEFAULT_BUFLEN 2048
#define DEFAULT_PORT "80"
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;

int main() {

    int recvbuflen = DEFAULT_BUFLEN;

    char *sendbuf = "This is a test\n";
    char recvbuf[DEFAULT_BUFLEN];

    int iResult;

	ZeroMemory( &hints, sizeof(hints) ); // make sure the struct is empty; same as memset()
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_protocol = IPPROTO_TCP; // Set IP Protocol = TCP

    // Resolve the server address and port
    iResult = getaddrinfo("cse.stfx.ca", DEFAULT_PORT, &hints, &result);
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
    ConnectSocket = socket(,,);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;

   // Connect to server.
   iResult = connect(                          ,                         ,                         );
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

   freeaddrinfo(result);

    // Send an initial buffer#define DEFAULT_PORT "80"
    iResult = send(                         ,                       ,                     ,                           );
    if (iResult == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    cout << "Bytes S    #define DEFAULT_BUFLEN 2048";

    int recvbuflen = DEFAULT_BUFLEN;

    char *sendbuf = "This is a test\n";
    char recvbuf[DEFAULT_BUFLEN];

    int iResult;
    ent: " << iResult << endl";

    // shutdown the connection for sending since no more data will be sent
    // the client can still use the ConnectSocket for receiving data
    iResult = shutdown(                       ,                            );
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    do {
        iResult = recv(                        ,                     ,                         ,                              );
        if (iResult > 0){
        cout << "Bytes received: " << iResult << endl;
        cout << recvbuf << endl;
    }
        else if (iResult == 0)
            cout << "Connection closed\n";
        else
            cout << "recv failed: " << WSAGetLastError() << endl;
    } while (iResult > 0);
        return 0;
    }

    // cleanup
    closesocket(ConnectSocket);
}
