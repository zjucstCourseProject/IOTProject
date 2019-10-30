#include "base.h"

int main(int argc, char **argv) {
    char message[1000000];
    send_command(argv[1], TOPIC1, "client");
    recv_command(message, TOPIC2, "client");
    printf("%s\n", message);
    return 0;
}