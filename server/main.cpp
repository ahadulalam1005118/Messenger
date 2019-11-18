#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <zconf.h>
#include <cstring>
#include <csignal>
#include "server.h"

using namespace std;
/*void sigint_handler(int sig_num)
{
    signal(SIGINT, sigint_handler);
    //printf("caught");
    exit(0);

}*/

int main(int argc, char  **argv) {
    if(argc != 3)
    {
        std::cout << "error!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string user_info_file(argv[1]);
    std::string configuration_file(argv[2]);
    //signal(SIGINT,server->sigint_handler);
    server server;
    //signal(SIGINT,server.sigint_handler);
    server.begin(user_info_file, configuration_file);
    server.run();
    //server.exit_from_server();
    //server.run();
    //_server._exit();
    return EXIT_SUCCESS;

}

/*
void *process_connection(void *arg) {
    int sockfd;
    ssize_t n;
    char buf[MAXBUFLEN];

    sockfd = *((int *)arg);
    free(arg);

    pthread_detach(pthread_self());
    while ((n = read(sockfd, buf, MAXBUFLEN)) > 0) {
        buf[n] = '\0';
        cout << buf << endl;
        write(sockfd, buf, strlen(buf));
    }
    if (n == 0) {
        cout << "client closed" << endl;
    } else {
        cout << "something wrong" << endl;
    }
    close(sockfd);
    return(NULL);
}
*//*


int main() {
    int serv_sockfd, cli_sockfd, *sock_ptr;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t sock_len;
    pthread_t tid;

    cout << "port = " << port << endl;
    serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((void*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(serv_sockfd, 5);

    for (; ;) {
        sock_len = sizeof(cli_addr);
        cli_sockfd = accept(serv_sockfd, (struct sockaddr *)&cli_addr, &sock_len);

        cout << "remote client IP == " << inet_ntoa(cli_addr.sin_addr);
        cout << ", port == " << ntohs(cli_addr.sin_port) << endl;

        sock_ptr = (int *)malloc(sizeof(int));
        *sock_ptr = cli_sockfd;

        pthread_create(&tid, NULL, &process_connection, (void
        *)sock_ptr);
    }
}*/
