
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

#include <arpa/inet.h>
#include <netdb.h>

#include "settings.h"

int main(void)
{
    // создаёт конечную точку соединения и возвращает файловый дескриптор.
    // AF_INET Протоколы Интернет IPv4
    // SOCK_STREAM Обеспечивает создание двусторонних, 
    // надёжных потоков байтов на основе установления соединения. 
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        perror("socket failed");
        return EXIT_FAILURE;
    }
    // преобразование  доменного  имени  сервера  в  его сетевой адрес
    struct hostent *server = gethostbyname(HOST); 
    if (server == NULL) {
        perror("gethostbyname failed");
        return EXIT_FAILURE;
    }

    //memset((char *)&server, 0, sizeof(server)); // заполняем нулями
    // Для  сетевого  взаимодействия 
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = *((struct in_addr*) server->h_addr_list[0]),
        // преобразовать число из порядка хоста в сетевой
        .sin_port = htons(SOCK_PORT)
    };
    
    /*
    // Соединение с другим сокетом
    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("connect failed");
        return EXIT_FAILURE;
    }
    */
    printf("Input message: ");
    
    char msg[MSG_LEN];
    fgets(msg, MSG_LEN, stdin);
    if (sendto(sock, msg, MSG_LEN, 0, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        perror("socket");
        exit(1);   
    }
    close(sock);
    return 0;
}
