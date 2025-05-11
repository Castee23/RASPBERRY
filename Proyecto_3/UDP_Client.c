#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

#define PORT 5000
#define MSG  "Hello Server"
#define BUF  64

int main(int argc, char *argv[])
{
    if (argc < 2) { puts("Uso: IOTClient <IP-servidor>"); return 1; }
    INIT_NET();

    sock_t s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == (sock_t)-1) { perror("socket"); return 1; }

    struct sockaddr_in serv = { .sin_family = AF_INET,
                                .sin_port   = htons(PORT) };
    if (inet_pton(AF_INET, argv[1], &serv.sin_addr) != 1) {
        fputs("IP no válida\n", stderr); return 1;
    }

    sendto(s, MSG, (int)strlen(MSG), 0, (void*)&serv, sizeof(serv));
    puts("→ \"Hello Server\" enviado; esperando…");

    char buf[BUF] = {0};
    int n = recvfrom(s, buf, BUF-1, 0, NULL, NULL);
    if (n < 0) { perror("recvfrom"); return 1; }

    buf[n] = '\0';  printf("← \"%s\"\n", buf);
    CLOSESOCK(s);  STOP_NET();
    return 0;
}
