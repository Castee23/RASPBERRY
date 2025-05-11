#include <stdio.h>
#include <string.h>
#include "common.h"

#define PORT     5000
#define BUF      64
#define OK       "Hello RPI"
#define ERR      "WrongMessage"

int main(void)
{
    INIT_NET();
    sock_t s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == (sock_t)-1) { perror("socket"); return 1; }

    struct sockaddr_in addr = { .sin_family = AF_INET,
                                .sin_port   = htons(PORT),
                                .sin_addr   = { .s_addr = htonl(INADDR_ANY) } };
    if (bind(s, (void*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }

    puts("Server listo; esperando");
    char buf[BUF] = {0};
    struct sockaddr_in cli; socklen_t len = sizeof(cli);

    int n = recvfrom(s, buf, BUF-1, 0, (void*)&cli, &len);
    if (n < 0) { perror("recvfrom"); return 1; }

    buf[n] = '\0';
    printf("Datagrama %s de %s:%d\n",
           buf, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

    const char *ans = strcmp(buf,"Hello Server")==0 ? OK : ERR;
    sendto(s, ans, (int)strlen(ans), 0, (void*)&cli, len);

    CLOSESOCK(s);  STOP_NET();
    return 0;
}
