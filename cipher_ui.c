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
    fgets(buffer,CHAR_LIMIT,stdin);
    scanf("%d", &shift);
    ioctl(driver_location, _IO(major,2));
    write(driver_location, &shift, sizeof(int));
    ioctl(driver_location, _IO(major,0), buffer);
    write(driver_location, buffer, sizeof(char)*(strlen(buffer)+1));
    buffer[0]='a';
    read(driver_location, buffer, sizeof(char)*(strlen(buffer)+1));
    printf("%s",buffer);
    return 0;
}
