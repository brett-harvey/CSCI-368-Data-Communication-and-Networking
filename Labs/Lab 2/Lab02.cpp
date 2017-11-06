/*
 * Author: Brett Harvey
 * Date: September 25th, 2017
 * Description: Creating a winsock application 
 */

// Library: to Import -> libws2_32.a

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

char HostInfo() {

  char host_name[128];

  // To set static hostnames:
  // char host_name[] = "alonso.stfx.ca"
  // char host_name[] = "moodle.stfx.ca"

  gethostname(host_name, sizeof(host_name));
  cout << host_name << endl;

   cout << host_name << endl;
   struct hostent *remoteHost;
struct in_addr addr;
DWORD dwError;
char **pAlias;
int i;
remoteHost = gethostbyname(host_name);
if (remoteHost == NULL) {
   dwError = WSAGetLastError();
   if (dwError != 0) {
      if (dwError == WSAHOST_NOT_FOUND) {
         cout << "Host not found\n";
         return 1;
       } else if (dwError == WSANO_DATA) {
         cout << "No data record found\n";
       } else {
         cout << "Function failed with error: " << dwError << endl;
         return 1;
       }
   }
} else {
   cout << "Function returned:\n";
   cout << "Official name: " << remoteHost->h_name << endl;
   i=0;
   for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
      cout << "Alternate name #" << ++i << ": " << *pAlias << endl;
   }
   cout << "\tAddress type: ";
   switch (remoteHost->h_addrtype) {
      case AF_INET:
         cout << "AF_INET\n";
         break;
      case AF_NETBIOS:
         cout << "AF_NETBIOS\n";
         break;
      default:
         cout << remoteHost->h_addrtype << endl;
         break;
   }
   cout << "\tAddress length: " << remoteHost->h_length << endl;
      i = 0;
   if (remoteHost->h_addrtype == AF_INET)
   {
       while (remoteHost->h_addr_list[i] != 0) {
           addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
           cout << "\tIP Address #" << i << ": " << inet_ntoa(addr) << endl;
       }
   } else if (remoteHost->h_addrtype == AF_NETBIOS) {
         cout << "NETBIOS address was returned\n";
   }
}
}
int main() {
   WORD wVersion = MAKEWORD(2, 2); // Used to request version 2.2 of Windows sockets
   WSADATA wsaData;
   int iResult;
   // Initialize Winsock
   iResult = WSAStartup(wVersion, &wsaData);   // Data loaded by WSAStartup
   if (iResult != 0) {
      cout << "WSAStartup failed: " << iResult << endl;   // Error check if WSAStartup successful
      return 1;
   }
   HostInfo();
   WSACleanup();
   return 0;
}
