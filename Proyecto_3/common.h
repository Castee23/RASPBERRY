/* common-udp.h  – cabeceras y utilidades cruzadas Win / POSIX */
#ifdef _WIN32
  #define _WIN32_WINNT 0x0601
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib,"Ws2_32.lib")
  typedef SOCKET sock_t;
  #define CLOSESOCK(s)  closesocket(s)
  #define INIT_NET()    do { WSADATA w; WSAStartup(MAKEWORD(2,2), &w); } while(0)
  #define STOP_NET()    WSACleanup()
#else
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <unistd.h>
  typedef int sock_t;
  #define CLOSESOCK(s)  close(s)
  #define INIT_NET()    ((void)0)
  #define STOP_NET()    ((void)0)
#endif
