#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <netdb.h>
#include <algorithm>
#include <csignal>
#include "server.h"
#define MAXCONN 10
using namespace std;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

/*const unsigned port = 5100;*/
const unsigned MAXBUFLEN = 512;
server *server::_server = NULL;

//int server::online_users = 0;
template<typename Out>
void server :: split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}
void server::sigint_handler(int sig_num)
{
    signal(SIGINT, sigint_handler);
    //printf("caught");
    _server->exit_from_server();
    for(int i=0; i<_server->online_users.size();i++){
        close(_server->online_users[i].sock_fd);
        close(_server->server_sockfd);
    }
    exit(0);


}
void server::add_logged_in_user(logged_in_user usr) {
    _server->online_users.push_back(usr);
}
vector<logged_in_user> server::get_online_users() {
    return _server->online_users;
}
string server::get_username_from_sockfd(int sock_fd) {
    for(int i=0; i<_server->online_users.size(); i++) {
        if(_server->online_users[i].sock_fd == sock_fd) return _server->online_users[i].username;
    }

}
int server::get_sockfd_from_username(string username) {
    for(int i=0; i<_server->online_users.size(); i++) {
        if(_server->online_users[i].username == username) return _server->online_users[i].sock_fd;
    }
}
std::vector<std::string> server :: split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
bool server :: is_logged_in(string username) {
    int flag = 0;
    for(int i=0; i< _server->online_users.size(); i++) {
        if(online_users[i].username == username) flag=1;
    }
    if(flag) return true;
    else return false;

}
int server ::begin(std::string user_info_file_path, std::string config_file_path) {
    this->user_info_file_path = user_info_file_path;
    this->configuration_file_path = config_file_path;
    _server = this;
    signal(SIGINT, sigint_handler);
    //vector<config> configuration;
    //vector<user_info> users;
    //std::string fname = "configurations";
    //std::string user_file_name = "user_credentials";
    this->current_configurations = get_configurations(this->configuration_file_path.c_str());
    this->current_users = get_users(this->user_info_file_path);
    //cout << this->current_users.size() << endl;
    //cout << this->current_configurations.size() << endl;
    //cout << users.size() << endl;
    //cout << configuration.size() << endl;
    return EXIT_SUCCESS;
}

void server::send_logout_info_to_friends(string username) {
    vector<logged_in_user> online_friends = get_logged_in_friends(get_logged_in_user_by_name(username));
    for(int i=0; i<online_friends.size();i++) {
        int temp_sockfd = get_sockfd_from_username(online_friends[i].username);
        //pthread_mutex_lock(&m);
        write(temp_sockfd,("loggedout$"+username).c_str(),MAXBUFLEN);
        //pthread_mutex_unlock(&m);

    }
    /*int temp_sockfd = get_sockfd_from_username(username);
    pthread_mutex_lock(&m);
    write(temp_sockfd,("loggedout$"+username).c_str(),MAXBUFLEN);
    pthread_mutex_unlock(&m);*/

}
user_info server::get_user_info_by_name(string username) {
    //user_info usr;
    for(int i=0; i<_server->current_users.size(); i++) {
        if(_server->current_users[i].name == username) return _server->current_users[i];
    }
}
vector<logged_in_user> server::get_logged_in_friends(logged_in_user new_user) {
    vector<logged_in_user> friends;
    user_info new_user_info = get_user_info_by_name(new_user.username);
    for(int i=0; i< new_user_info.friends.size(); i++) {
        if(is_logged_in(new_user_info.friends[i])) {
            for(int j=0; j< _server->online_users.size(); j++) {
                if(online_users[j].username == new_user_info.friends[i])
                    friends.push_back(online_users[i]);
            }

        }
    }
    return friends;

}
logged_in_user server::get_logged_in_user_by_name(string username) {
    for(int num=0; num<_server->online_users.size(); num++) {
        if(_server->online_users[num].username == username) return _server->online_users[num];
    }
}
int server::exit_from_server() {

    write_all_infos_to_file();
    return EXIT_SUCCESS;
}
void server::write_all_infos_to_file() {
    ofstream outFile;
    const char * file = _server->user_info_file_path.c_str();
    outFile.open(file,ios::binary|ios::out);
    outFile.clear();
    if (outFile.fail()) {
        cout << "Could Not Open File\n";
    } else {
        for(int j=0; j< _server->current_users.size(); j++) {
            string to_write;
            string pipe = "|";
            size_t pipe_sz = pipe.size();
            size_t size = _server->current_users[j].name.size();
            //cout << size << endl;
            size_t size_pass = _server->current_users[j].password.size();
            to_write+=_server->current_users[j].name+pipe+_server->current_users[j].password+pipe;
            //outFile.write(this->current_users[j].name.c_str(), sizeof(size));
            //outFile.write(pipe.c_str(), sizeof(pipe_sz));
            //outFile.write(this->current_users[j].password.c_str(), sizeof(size_pass));
            //outFile.write(pipe.c_str(), sizeof(pipe_sz));
            for(int k=0; k<_server->current_users[j].friends.size(); k++) {
                string comma = ";";
                //size_t comma_sz = comma.size();
                to_write+=_server->current_users[j].friends[k];
                to_write+=comma;
                //size_t frname_sz = this->current_users[j].friends[k].size();
                //outFile.write(this->current_users[j].friends[k].c_str(), sizeof(frname_sz));
                //outFile.write(comma.c_str(), sizeof(comma_sz));
                //outFile << endl;

            }

            cout << to_write << endl;
            outFile << to_write << endl;
            //outFile.write(to_write.c_str(),to_write.size());
            //outFile << endl;




            //outFile.write(this->current_users[j].name, sizeof());

        }
    }

}
void server::evict_user_from_online_users(string username) {
    _server->online_users.erase(
            std::remove_if(_server->online_users.begin(), _server->online_users.end(), [&](logged_in_user const & usr) {
                return usr.username == username;
            }),
            _server->online_users.end());
}
int server::run() {
    int client[MAXCONN];
    unsigned int port = get_configurations(_server->configuration_file_path)[0].value;
    //int server_sockfd;
    int client_sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t sock_len;
    ssize_t _buf_size;
    //char buf[MAXBUFLEN];
    fd_set allset, rset;
    int maxfd;
    int len;
    server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero((void *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_UNSPEC;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    bind(server_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    len = sizeof(serv_addr);
    getsockname(server_sockfd, (struct sockaddr *)&serv_addr, &sock_len);
    std::cout << "Fully Qualified Domain Name = " << get_fully_qualified_domain_name() << std::endl;
    std::cout << "Port = " << ntohs(serv_addr.sin_port) << std::endl;
    listen(server_sockfd,5);
    for(int i = 0; i<MAXCONN; i++) client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(server_sockfd, &allset);
    maxfd = server_sockfd;
    int i;

    while (1) {
        rset = allset;
        select(maxfd+1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(server_sockfd, &rset)) {
            /* somebody tries to connect */
            if ((client_sockfd = accept(server_sockfd, (struct sockaddr
            *)(&cli_addr), (socklen_t *)&len)) < 0) {
                if (errno == EINTR)
                    continue;
                else {
                    perror(":accept error");
                    return EXIT_SUCCESS;
                }
            }
	    //cout << _server->online_users.size();

            /*
            if (rec_sock < 0) {
                perror(": accept");
                exit(1);
            }
            */

            /* print the remote socket information */

            /*printf("remote machine = %s, port = %d.\n",
                   inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));*/
            //int i;

            for (i=0; i<MAXCONN; i++) {
                if (client[i] < 0) {
                    client[i] = client_sockfd;
                    FD_SET(client[i], &allset);
                    break;
                }
            }
            if (i == MAXCONN) {
                printf("too many connections.\n");
                close(client_sockfd);
            }
            if (client_sockfd > maxfd) maxfd = client_sockfd;
        }

        for (i=0; i<MAXCONN; i++) {
	    //cout << _server->online_users.size();
	    
            if (client[i] < 0) continue;
            if (FD_ISSET(client[i], &rset)) {
                char buf[MAXBUFLEN];
                int num;
                num = read(client[i], buf,MAXBUFLEN);
                cout << buf << endl;
                if (num == 0) {
                    /* client exits */
                    close(client[i]);
                    FD_CLR(client[i], &allset);
                    client[i] = -1;
                } else {
                    //write(client[i], "abcd", num); //server response from here
                    vector<string> commands;
                    commands = split(buf, ':');
		    memset(buf, 0, sizeof(buf));
                    if(commands[0] == "r") {
                        if(is_available(commands[1],commands[2])) {
                            string response = "registered";
                            pthread_mutex_lock(&m);
                            write(client[i],response.c_str(), response.size());
                            pthread_mutex_unlock(&m);

                        }
                        else {
                            string response = "500";
                            write(client[i],response.c_str(), response.size());
                        }
                    }
                    else if(commands[0] == "l") {
                        if(user_exists(commands[1],commands[2])) {
                            string response = "loggedin$";
                            response += commands[1]+"$";
                            printf("remote machine = %s, port = %d.\n",
                                   inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                            logged_in_user new_user;
                            new_user.username = commands[1];
                            new_user.ip_addr = inet_ntoa(cli_addr.sin_addr);

                            new_user.port = ntohs(cli_addr.sin_port);
                            new_user.sock_fd = client[i];
                            add_logged_in_user(new_user);
                            vector<logged_in_user> logged_in_friends = get_logged_in_friends(new_user);
                            //cout << logged_in_friends.size() << endl;
                            for(int j=0; j<logged_in_friends.size(); j++) {
                                string _sentinel= ":";
                                cout << logged_in_friends[j].ip_addr << endl;
                                string locs_info = "locs"+_sentinel+new_user.ip_addr+_sentinel+new_user.username+"|" ;
                                string locs_info_to_user = "locs"+_sentinel+logged_in_friends[j].ip_addr+_sentinel+logged_in_friends[j].username+"|";
                                pthread_mutex_lock(&m);
                                write(logged_in_friends[j].sock_fd,locs_info.c_str(), locs_info.size());
                                response+=locs_info_to_user;
                                //write(client[i],locs_info_to_user.c_str(),locs_info.size());
                                pthread_mutex_unlock(&m);

                            }
                            //TODO send user locations to friend list
                            //TODO send user1's friend location to user1
                            pthread_mutex_lock(&m);
                            write(client[i],response.c_str(), response.size());
                            pthread_mutex_unlock(&m);
                            //online_users++;
                        }

                        else {
                            string response = "500";
                            write(client[i],response.c_str(), MAXBUFLEN);
                        }
                    }
                    else if(commands[0] == "i") {
                        string from = get_username_from_sockfd(client[i]);
                        string to = commands[1];
                        int to_sockfd = get_sockfd_from_username(to);
                        write(to_sockfd,commands[2].c_str(),commands[2].size());
                        invitation new_invitation;
                        new_invitation.from_username = from;
                        new_invitation.to_username = to;
                        _server->invitations.push_back(new_invitation);
                    }
                    else if(commands[0] == "ia") {
                        string from = get_username_from_sockfd(client[i]);
                        string to = commands[1];
                        int to_sockfd = get_sockfd_from_username(to);
                        write(to_sockfd,commands[2].c_str(),commands[2].size());
                        for(int j=0; j< _server->invitations.size(); j++) {
                            if(_server->invitations[j].from_username == to) {
                                for(int k=0; k<_server->current_users.size(); k++) {
                                    if(_server->current_users[k].name == from) {
                                        if (!std::binary_search(_server->current_users[k].friends.begin(),
                                                                _server->current_users[k].friends.end(),to)) {
                                            _server->current_users[k].friends.push_back(to);
                                            logged_in_user frm = get_logged_in_user_by_name(_server->current_users[k].name);
                                            logged_in_user too = get_logged_in_user_by_name(to);
                                            string to_send = "locs:";
                                            to_send+=frm.ip_addr;
                                            to_send+=":";
                                            to_send+=frm.username;
                                            to_send+="|";
                                            write(too.sock_fd,to_send.c_str(),to_send.size());
                                            string to_send_another = "locs:";
                                            to_send_another+=too.ip_addr;
                                            to_send_another+=":";
                                            to_send_another+=too.username;
                                            to_send_another+="|";
                                            write(frm.sock_fd,to_send_another.c_str(),to_send_another.size());



                                        }

                                        //cout << _server->current_users[k].name << " "<<  _server->current_users[k].friends.size() <<endl;
                                        //this->current_users[k].friends.push_back(to);
                                    }
                                }
                                for(int k=0; k<_server->current_users.size(); k++) {
                                    if(_server->current_users[k].name == to) {
                                        if (!std::binary_search(_server->current_users[k].friends.begin(),
                                                                _server->current_users[k].friends.end(),from)) {
                                            _server->current_users[k].friends.push_back(from);
                                            logged_in_user temp = get_logged_in_user_by_name(_server->current_users[k].name);
                                            /*for(int cnt=0; cnt< _server->current_users[k].friends.size(); cnt++) {
                                                string to_send = "locs:";
                                                to_send+=temp.ip_addr;
                                                to_send+=":";
                                                to_send+=temp.username;
                                                to_send+="|";
                                                pthread_mutex_lock(&m);
                                                write(get_logged_in_user_by_name(_server->current_users[k].friends[cnt]).sock_fd,to_send.c_str(),to_send.size());
                                                pthread_mutex_unlock(&m);

                                            }*/

                                        }
                                        //this->current_users[k].friends.push_back(from);
                                    }
                                }

                            }

                        }

                    }
                    else if(commands[0] == "logout") {
                        string username = get_username_from_sockfd(client[i]);
                        //evict_user_from_online_users();
                        send_logout_info_to_friends(username);
                        evict_user_from_online_users(username);
                        close(client[i]);
                        FD_CLR(client[i], &allset);
                        client[i] = -1;

                    }
                    else if(commands[0] == "save") {
                        exit_from_server();
                    }

                    //cout << buf <<endl;
                }
            }
        }

    }



}
string server::get_fully_qualified_domain_name() {
    string domain_name;
    struct addrinfo hints;
    struct addrinfo *info_iterator, *info;
    int get_address_info_result;
    char host[1024];
    gethostname(host,1023);
    host[1023] = '\0';
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    get_address_info_result = getaddrinfo(host, "http", &hints, &info);
    if(get_address_info_result != 0) {
        return domain_name;
    }
    for(info_iterator = info; info_iterator != NULL; info_iterator = info_iterator->ai_next) {
        domain_name += info_iterator->ai_canonname;
    }
    freeaddrinfo(info);
    return domain_name;

}
config server :: split_string_config(string s, string delimiter){
    config conf;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        conf.name = token;
        s.erase(0, pos + delimiter.length());
    }
    conf.value = (unsigned int) stoi(s);
    return conf;
}
user_info server :: split_string_user_info(string s, string delimiter){
    user_info user;
    size_t pos = 0;
    std::string token;
    int temp =0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(temp == 0)
            user.name = token;
        if(temp == 1)
            user.password = token;

        s.erase(0, pos + delimiter.length());
        temp++;
    }
    while ((pos = s.find(";")) != std::string::npos) {
        token = s.substr(0, pos);
        //cout << "tok: "<<token << endl;
        user.friends.push_back(token);
        s.erase(0, pos + delimiter.length());
        //temp++;
    }
    //cout << "ss" << s << endl;
    //user.friends.push_back(s);
    return user;
}
vector<config> server :: get_configurations(string filename){
    vector<config> configuration;
    //config conf;
    ifstream inFile;
    const char * file = filename.c_str();
    inFile.open(file,ios::binary|ios::in);
    if (inFile.fail()) {
        cout << "Could Not Open File\n";
    } else {
        std::string line;
        while(getline(inFile, line)) {
            config conf = split_string_config(line,":");
            configuration.push_back(conf);
        }
    }
    inFile.close();
    return configuration;
}
vector<user_info> server::get_users(string filename) {
    vector<user_info> users;
    ifstream inFile;
    const char * file = filename.c_str();
    inFile.open(file,ios::binary|ios::in);
    if (inFile.fail()) {
        cout << "Could Not Open File\n";
    } else {
        std::string line;
        while(getline(inFile, line)) {
            user_info user = split_string_user_info(line,"|");
            users.push_back(user);
        }
    }
    inFile.close();
    return users;


}
int server ::is_available(string username, string password) {
    int flag = 0;
    for(int i=0; i<_server->current_users.size(); i++) {
        //int flag = 0;
        if(current_users[i].name.compare(username) == 0) {
            flag = 1;
        }
    }
    if(flag == 1) {
        return 0;
    }
    else {
        user_info usr;
        usr.name = username;
        usr.password = password;
        _server->current_users.push_back(usr);
        return 1;
    }
}
int server ::user_exists(string username, string password) {
    int flag = 0;
    for(int i=0; i< _server->current_users.size(); i++) {
        if(current_users[i].name.compare(username) == 0 &&
                current_users[i].password.compare(password) == 0) {
            flag = 1;

        }
    }
    if(flag == 1) return 1;
    else return 0;
}



