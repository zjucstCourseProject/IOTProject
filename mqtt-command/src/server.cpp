#include <fcntl.h>
#include <stdio.h>
#include <cstring>
#include "base.h"

int main() {
    char message[100];
    char command[100];
    char info[1000000];
    recv_command(message, TOPIC1, "server");
    sprintf(command, "%s 1> a.txt", message);
    system(command);
    usleep(100000);
    int fd = open("a.txt", O_RDONLY);
    int len = -1;
    while (len) {
        char buff[1024] = {'\0'};
        len = read(fd, buff, sizeof(buff));
        strcat(info, buff);
    }
    printf("%s\n", info);
    send_command(info, TOPIC2, "server");
    close(fd);
    system("rm a.txt");
    return 0;
}