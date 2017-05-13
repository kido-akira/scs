#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN  1024
#define SERVER_IP_ADDR  "192.168.10.37"
#define SERVER_PORT     60606

#define ERRLOG(...)     fprintf(stderr, __VA_ARGS__)

void winSockError(const char* func, int code = 0) {
    char label[DEFAULT_BUFLEN];
    if(code == 0) code = WSAGetLastError();
    switch(code) {
    case WSASYSNOTREADY:
        sprintf(label, "%s", "WSASYSNOTREADY");
        break;
    case WSAVERNOTSUPPORTED:
        sprintf(label, "%s", "WSAVERNOTSUPPORTED");
        break;
    case WSAEINPROGRESS:
        sprintf(label, "%s", "WSAEINPROGRESS");
        break;
    case WSAEPROCLIM:
        sprintf(label, "%s", "WSAEPROCLIM");
        break;
    case WSAEFAULT:
        sprintf(label, "%s", "WSAEFAULT");
        break;
    case WSANOTINITIALISED:
        sprintf(label, "%s", "WSANOTINITIALISED");
        break;
    case WSAENETDOWN:
        sprintf(label, "%s", "WSAENETDOWN");
        break;
    case WSAEACCES:
        sprintf(label, "%s", "WSAEACCES");
        break;
    case WSAEINTR:
        sprintf(label, "%s", "WSAEINTR");
        break;
    case WSAENETRESET:
        sprintf(label, "%s", "WSAENETRESET");
        break;
    case WSAENOBUFS:
        sprintf(label, "%s", "WSAENOBUFS");
        break;
    case WSAENOTCONN:
        sprintf(label, "%s", "WSAENOTCONN");
        break;
    case WSAENOTSOCK:
        sprintf(label, "%s", "WSAENOTSOCK");
        break;
    case WSAEOPNOTSUPP:
        sprintf(label, "%s", "WSAEOPNOTSUPP");
        break;
    case WSAESHUTDOWN:
        sprintf(label, "%s", "WSAESHUTDOWN");
        break;
    case WSAEWOULDBLOCK:
        sprintf(label, "%s", "WSAEWOULDBLOCK");
        break;
    case WSAEMSGSIZE:
        sprintf(label, "%s", "WSAEMSGSIZE");
        break;
    case WSAEHOSTUNREACH:
        sprintf(label, "%s", "WSAEHOSTUNREACH");
        break;
    case WSAEINVAL:
        sprintf(label, "%s", "WSAEINVAL");
        break;
    case WSAECONNABORTED:
        sprintf(label, "%s", "WSAECONNABORTED");
        break;
    case WSAECONNRESET:
        sprintf(label, "%s", "WSAECONNRESET");
        break;
    case WSAETIMEDOUT:
        sprintf(label, "%s", "WSAETIMEDOUT");
        break;
    case WSAECONNREFUSED:
        sprintf(label, "%s", "WSAECONNREFUSED");
        break;
    default:
        sprintf(label, "%s", "UNKNOWN_ERROR");
        break;
    }
    ERRLOG("%s(%d): %s failed\n", label, code, func);
    WSACleanup();
}

bool query(const char* message) {
    char host[] = SERVER_IP_ADDR;
    int  port   = SERVER_PORT;
    int  retVal = 0;

    WSADATA wsaData;
    retVal = WSAStartup(MAKEWORD(2, 0), &wsaData); // version 2.0
    if(retVal) {
        winSockError("WSAStartup", retVal);
        return false;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET) {
        winSockError("socket");
        return false;
    }

    struct sockaddr_in server;
    memset((void*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    retVal = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if(retVal == SOCKET_ERROR) {
        winSockError("connect"); //sockes are automatically deallocated
        return false;
    }

    retVal = send(sock, message, (int)strlen(message), 0);
    if(retVal == SOCKET_ERROR) {
        winSockError("send"); //sockes are automatically deallocated
        return false;
    }

    char recvBuf[DEFAULT_BUFLEN];
    memset(recvBuf, 0, sizeof(recvBuf));
    retVal = recv(sock,recvBuf, sizeof(recvBuf), 0);
    if(retVal == SOCKET_ERROR) {
        winSockError("recv"); //sockes are automatically deallocated
        return false;
    }

    retVal = closesocket(sock);
    if(retVal == SOCKET_ERROR) {
        winSockError("closesocket");
        return false;
    }

    retVal = WSACleanup();
    if(retVal == SOCKET_ERROR) {
        winSockError("WSACleanup");
        return false;
    }

    return true;
}


int main(int argc, char* argv[]) {
    if(argc < 2) {
        ERRLOG("usage: %s message\n", argv[0]);
        return 1;
    }

    if(query(argv[1])) {
        printf("Authentication was successful.\n");
    } else {
        printf("Authentication failed.\n");
    }

    return 0;
}

