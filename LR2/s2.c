#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>

#include "settings.h"

void perror_and_exit(char *s)
{
	perror(s);
	exit(1);
}

void convert_to2(int num, char *res);
void convert_to13(int num, char *res);

void convert_and_print(char *b)
{
    int n = atoi(b);
    char res2[RES_LEN] = { 0 };
    convert_to2(n, res2);
        
    char res13[RES_LEN] = { 0 };
    convert_to13(n, res13);
                
    printf("decimal: %d\n", n);
    printf("binary: %s\n", res2);
    printf("octal: %o\n", n);
    printf("hexadecimal: %x\n", n);
    printf("13th: %s\n\n", res13); 
}


// a - число в 10й СС
// p - основание СС [2, 30]
// s - строка для результата
void convert_to2(int num, char *res)
{
    char temp[RES_LEN] = { 0 };
    int i = 0;
    while (num >= 2)
    {
        temp[i] = (num % 2) + 48;
        num /= 2;
        i++;
    }
    temp[i] = num + 48;
    int j = 0;
    for (; j < strlen(temp); j++, i--)
    {
        res[j] = temp[i];
    }       
}

void convert_to13(int num, char *res)
{
    char temp[RES_LEN] = { 0 };
    int i = 0;
    while (num >= 13)
    {
        int rest = num % 13;
        temp[i] = rest + 48;
        
        if (rest > 9)
            temp[i] += 7;

        num /= 13;
        i++;
    }
    temp[i] = num + 48;
    if (num > 9)
        temp[i] += 7;
    
    int j = 0;
    for (; j < strlen(temp); j++, i--)
    {
        res[j] = temp[i];
    }       
}

int main(void)
{
    printf("Server started\n");

    // создаёт конечную точку соединения и возвращает файловый дескриптор.
    // AF_INET Протоколы Интернет IPv4
    // SOCK_STREAM Обеспечивает создание двусторонних, 
    // надёжных потоков байтов на основе установления соединения.
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket failed");
        return EXIT_FAILURE;
    }
    // Для  сетевого  взаимодействия 
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        // Linux kernel может быть либо big endian, либо little endian, в зависимости от архитектуры
        // преобразовать число из порядка хоста в сетевой
        .sin_port = htons(SOCK_PORT),
        // программа сервер зарегистрируется на всех адресах той  машины, на которой она выполняется
        .sin_addr.s_addr = INADDR_ANY    
    };
    // связывание сокета с некоторым адресом
    // 1 - дескриптор сокета, 2 - указатель на структуру struct sockaddr
    if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return EXIT_FAILURE;
    }

    char buf[MSG_LEN];
    while (1)
    {
        if (recvfrom(sock, buf, MSG_LEN, 0, NULL, NULL) == -1)
            perror_and_exit("recvfrom()");
        
        convert_and_print(buf);
	}

	close(sock);
	return 0;
}