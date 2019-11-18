#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <cstring>
#include "client.h"

int main(int argc, char  **argv) {
    if(argc != 2)
    {
        std::cout << "error!" << std::endl;
        return EXIT_FAILURE;
    }
    //std::string user_info_file(argv[1]);
    std::string configuration_file(argv[1]);
    client client;
    client.begin(configuration_file);
    client.run();
    //server.run();
    //_server._exit();
    return EXIT_SUCCESS;

}
