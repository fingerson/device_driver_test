#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/ioctl.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#define CHAR_LIMIT 1000000

void driver_call(int driver_location, int major, int shift, char* buffer,int mode)
{
    ioctl(driver_location, _IO(major,2));
    write(driver_location, &shift, sizeof(int));
    ioctl(driver_location, _IO(major,mode), buffer);
    write(driver_location, buffer, sizeof(char)*(strlen(buffer)+1));
    buffer[0]='a';
    read(driver_location, buffer, sizeof(char)*(strlen(buffer)+1));
}

int main(int argc, char* argv[])
{
    if (argc<3) return -1;
    printf("%s\n",argv[1]);
    int driver_location = open(argv[1],O_RDWR);
    int major = atoi(argv[2]);
    if(driver_location == -1)
    {
        printf("Erro ao abrir o arquivo\n");
        return -1;
    }
    int shift;
    char buffer[CHAR_LIMIT+1];
    buffer[CHAR_LIMIT] = '\0';

    printf("Bem vindo ao programa criptografico supersecreto mais poderoso do seculo 1 A.C.\n");
    while(1)
    {
        int modo;
        printf("O que deseja fazer?\n1) Criptografar uma mensagem.\n2) Descriptografar uma mensagem.\n3) Sair.\n");
        scanf("%d", &modo);
        char tmpchar = getchar();
        while (tmpchar != '\n' && tmpchar != EOF) tmpchar = getchar();
        if (modo < 1 || modo > 2) break;
        printf("Digite a mensagem:\n");
        fgets(buffer,CHAR_LIMIT,stdin);
        printf("Digite a chave:\n");
        scanf("%d", &shift);
        driver_call(driver_location,major,shift,buffer, modo-1);
        printf("Sua mensagem: %s\n\n",buffer);
    }
    printf("Obrigado por utilizar o programa criptografico supersecreto mais poderoso do seculo 1 A.C.\n");
    return 0;
}
