//
// Created by ahad on 11/14/18.
//

#include <sstream>
#include <zconf.h>
#include <netdb.h>
#include <strings.h>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <algorithm>
#include "client.h"
#define MAXCONN 10
const unsigned MAXBUFLEN = 512;
client *client::_client = NULL;
bool client::login_status = false;
int sockfd;
int client_as_server_sockfd;
pthread_mutex_t accept_lock = PTHREAD_MUTEX_INITIALIZER;
//char buf[MAXBUFLEN];
// int sockfd;
string client::current_username = "";
vector<friends_info> client::friends_list = {};
vector<connected_friends> client::friends_sockfd = {};
void * client::process_connection_client(void *arg) {

        int cli_sockfd;
        struct sockaddr_in cli_addr;
        socklen_t sock_len;
        ssize_t n;
        char buf[MAXBUFLEN];

        int tid = *((int *)arg);
        free(arg);

        //cout << "thread " << tid << " created" << endl;
        for (; ;) {
            sock_len = sizeof(cli_addr);
            pthread_mutex_lock(&accept_lock);
            cli_sockfd = accept(client_as_server_sockfd, (struct sockaddr *)&cli_addr, &sock_len);
            pthread_mutex_unlock(&accept_lock);

           // cout << "thread " << tid << ": ";
           // cout << "remote client IP == " << inet_ntoa(cli_addr.sin_addr);
           // cout << ", port == " << ntohs(cli_addr.sin_port) << endl;

            while ((n = read(cli_sockfd, buf, MAXBUFLEN)) > 0) {
                buf[n] = '\0';
                cout << buf << endl;
                //write(cli_sockfd, buf, strlen(buf));
            }
            if (n == 0) {
                cout << "client closed" << endl;
            } else {
                cout << "something wrong" << endl;
            }
            close(cli_sockfd);
        }

}

void * client::message_handler(void *arg) {
    int sockfd;
    ssize_t n;
    char buf[MAXBUFLEN];

    sockfd = *((int *)arg);
    free(arg);

    pthread_detach(pthread_self());
    while ((n = read(sockfd, buf, MAXBUFLEN)) > 0) {
        cout << buf << endl;
        //write(sockfd, buf, strlen(buf));
    }
    if (n == 0) {
        cout << "client closed" << endl;
    } else {
        cout << "something wrong" << endl;
    }
    close(sockfd);
    return(NULL);
}
void * client::process_connection(void *arg) {
    //_client = this;
    int n;
    char buf[MAXBUFLEN];
    //response response;
    pthread_detach(pthread_self());
    while (1) {
       // cout << login_status <<endl;//communication occurs
        n = read(sockfd, buf,MAXBUFLEN);
        //n = recvfrom(sockfd,response, sizeof(struct response),0,)
        if (n <= 0) {
            if (n == 0) {
                cout << "server closed" << endl;
                _client->begin(_client->configuration_file_path);
                _client->run();
                //pthread_exit(0);
            } else {
                cout << "something wrong" << endl;
            }
            close(sockfd);
            // we directly exit the whole process.
            return(NULL);
        }
        buf[n] = '\0';
        cout << buf << endl;
        vector<string> locs_friends_commands;
        //vector<string>locs_commands;
        vector<string> commands;
        commands = _client->split(buf,'$');
        if(commands[0] == "loggedin") {
	   /* cout << "*************online Friends****************" << endl;
        for(int i=0; i<_client->friends_list.size(); i++) {
            cout << _client->friends_list[i].username << " "
                 <<  _client->friends_list[i].ip_addr << endl;
		}*/
            current_username = commands[1];
            login_status = true;
           // _client->handle_loggedin_command_from_server();
            if(commands.size() > 2) {
                locs_friends_commands = _client->split(commands[2],'|');
                for(int cnt =0; cnt < locs_friends_commands.size(); cnt++) {
                    vector<string>locs_commands;
                    locs_commands = _client->split(locs_friends_commands[cnt],':');
                    if(locs_commands[0] == "locs") {
                        friends_info new_friend;
                        new_friend.ip_addr = locs_commands[1];
                        new_friend.username = locs_commands[2];
                       // cout << locs_commands[1] << endl;
                       // cout << locs_commands[2] << endl;
                        //cout << _client->friends_list.size() << endl;
                        if(!_client->exists(locs_commands[2])) {
                            client::friends_list.push_back(new_friend);
                        }

                        //_client->friends_list.push_back(new_friend);
                    }

                }

            }


        }
        else if(commands[0] == "loggedout") {
            _client->evict_user_from_friends(commands[1]);
	    int to_close_sockfd = get_sockfd_from_username(commands[1]);
            if(to_close_sockfd > 0) close(to_close_sockfd);

        }
        else {
            vector<string>locs_split;
            vector<string> locs;
            locs_split = _client->split(buf, '|');
            locs = _client->split(locs_split[0],':');
            if (locs[0] == "locs") {
                friends_info new_friend;
                new_friend.ip_addr = locs[1];
                new_friend.username = locs[2];
                //cout << locs_commands[1] << endl;
                //cout << locs_commands[2] << endl;
                //cout << _client->friends_list.size() << endl;
                if (!_client->exists(locs[2])) {
                    client::friends_list.push_back(new_friend);
                }

            }
        }

        //cout << commands[0] << endl;
        //cout << commands[1] << endl;

        //locs_commands = _client->split(commands[1],':');
        if(login_status == true) {
		cout << "*************online Friends****************" << endl;
        for(int i=0; i<_client->friends_list.size(); i++) {
            cout << _client->friends_list[i].username << " "
                 <<  _client->friends_list[i].ip_addr << endl;
        }

	}

	/*cout << "*************online Friends****************" << endl;
        for(int i=0; i<_client->friends_list.size(); i++) {
            cout << _client->friends_list[i].username << " "
                 <<  _client->friends_list[i].ip_addr << endl;
	}*/

        
        sleep(1);
        //handle_command_from_server(string buf);
    }
}
void client::evict_user_from_friends(string username){
    this->friends_list.erase(
            std::remove_if(this->friends_list.begin(), this->friends_list.end(), [&](friends_info const & usr) {
                return usr.username == username;
            }),
            this->friends_list.end());
};
template<typename Out>
void client :: split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

int client::exists(string username) {
    int flag = 0;
    for(int cnt=0; cnt<_client->friends_list.size(); cnt++) {
        if(_client->friends_list[cnt].username == username) {
            flag = 1;
        }
    }
    if(flag == 1) return 1;
    else return 0;
}
string client::get_friend_address(string username) {
    for(int i=0; i<friends_list.size(); i++) {
        if(friends_list[i].username == username) return friends_list[i].ip_addr;
    }
}

void client::handle_loggedin_command_from_server() {
    struct sockaddr_in serv_addr;
    int port = 5000, number_thread;
    pthread_t tid;
    int *tid_ptr;
    number_thread = 5;
    client_as_server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((void*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    bind(client_as_server_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));//we do not pass it in thread.so serv_sockfd is global.

    listen(client_as_server_sockfd, 5);

    int i;
    for (i = 0; i < number_thread; ++i) {
        tid_ptr = (int *)malloc(sizeof(int));
        *tid_ptr = i;
        pthread_create(&tid, NULL, &process_connection_client, (void *)tid_ptr);
    }

    /*for (;;)
        sleep(1);*/
}
std::vector<std::string> client :: split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int client::begin(std::string config_file_path) {
    _client = this;
    this->configuration_file_path = config_file_path;
    return EXIT_SUCCESS;
}
config client :: split_string_config(string s, string delimiter){
    config conf;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        conf.name = token;
        s.erase(0, pos + delimiter.length());
    }
    conf.argv = s;
    return conf;
}
int client::get_sockfd_from_username(string username) {
    int flag = -1;
    for(int i=0; i<_client->friends_sockfd.size(); i++) {
        if(_client->friends_sockfd[i].username == username) flag = _client->friends_sockfd[i].sockfd;
    }
    return flag;
}
vector<config> client :: get_configurations(string filename){
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
int client ::is_connected(string username) {
    int flag = 0;
    for(int i=0; i< this->friends_sockfd.size(); i++) {
        if(friends_sockfd[i].username.compare(username) == 0) {
            flag = friends_sockfd[i].sockfd;
        }
    }
    if(flag > 0) return flag;
    else return -1;
}
int client::run() {
    int rv, flag;
    ssize_t n;
    struct addrinfo hints, *res, *ressave;
    pthread_t tid;


    //cout << get_configurations(this->configuration_file_path)[0].argv << " "
      //   << get_configurations(this->configuration_file_path)[1].argv << endl;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(get_configurations(this->configuration_file_path)[0].argv.c_str() ,
                          get_configurations(this->configuration_file_path)[1].argv.c_str() , &hints, &res)) != 0) {
        cout << "getaddrinfo wrong: " << gai_strerror(rv) << endl;
        EXIT_SUCCESS;
    }

    ressave = res;
    flag = 0;
    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
            continue;
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            flag = 1;
            break;
        }
        close(sockfd);
    } while ((res = res->ai_next) != NULL);
    freeaddrinfo(ressave);

    if (flag == 0) {
        fprintf(stderr, "cannot connect\n");
        EXIT_SUCCESS;
    }
    pthread_create(&tid, NULL, &process_connection, NULL);

    string oneline;
    while (getline(cin, oneline)) {
        //vector<string> commands;
        //commands = split(oneline,':');
        if (oneline == "exit") {
	    for(int i=0; i<_client->friends_sockfd.size();i++){
                close(friends_sockfd[i].sockfd);
            }

	    close(client_as_server_sockfd);
            //write(sockfd, oneline.c_str(), oneline.length());
            close(sockfd);
	    
            //close(sockfd);
            break;
        }
        else if(oneline == "r") {
            cout << "Type Username : ";
            string username;
            cin >> username;
            cout << "Type Password :";
            string password;
            cin >> password;
            string temp = "r:"+username+":"+password;
            write(sockfd, temp.c_str(), MAXBUFLEN);
        }
        else if(oneline == "l") {
            cout << "Type Username : ";
            string username;
            cin >> username;
            cout << "Type Password :";
            string password;
            cin >> password;
            string temp = "l:"+username+":"+password;
            write(sockfd, temp.c_str(), MAXBUFLEN);
            sleep(1);
            //cout << login_status;
            if(login_status == true) {
                _client->handle_loggedin_command_from_server();
            }
            //vector<string> commands;
            //commands = _client->split(buf,'$');
            /*if(commands[0] == "loggedin") {
                //handle_loggedin_command_from_server();
            }*/

        }
	else if(oneline == "logout:") {
	    for(int i=0; i<_client->friends_sockfd.size();i++){
                close(friends_sockfd[i].sockfd);
            }
            close(client_as_server_sockfd);
            write(sockfd, oneline.c_str(), oneline.length());
	    close(sockfd);
            _client->friends_sockfd.clear();
            client client;
            _client = this;
            _client->login_status = 0;
            _client->friends_list.clear();
            _client->begin("configurations");
            _client->run();
            //sleep(2);
            //            close(sockfd);
        }
        else if(oneline == "m") {
            int temp_sockfd;
            string port = "5000";
            cout << "type friends name : ";
            string friend_name;
	    getline(cin,friend_name);
            //cin >> friend_name;
            cout << "type message : ";
            string message;
	    getline(cin, message);
            //cin >> friend_name;
            //cin >> message;
            get_friend_address(friend_name);
           // cout << get_friend_address(friend_name) << endl;
           // cout << login_status << endl;
            int *sock_ptr;
            int sckfd;
            string final_message = client::current_username+">>"+message;
            sckfd = is_connected(friend_name);
            if(sckfd > 0) {
                write(sckfd,final_message.c_str(),final_message.length());
            }
            else {
               
                int rv, flag;
                ssize_t n;
                struct addrinfo hints, *res, *ressave;
                pthread_t tid;
                //int *sock_ptr;
                sock_ptr = (int *)malloc(sizeof(int));


                bzero(&hints, sizeof(struct addrinfo));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;
                string addr_of_frnd = get_friend_address(friend_name);
               

                if ((rv = getaddrinfo(addr_of_frnd.c_str(),
                                      port.c_str(), &hints, &res)) != 0) {
                    cout << "getaddrinfo wrong: " << gai_strerror(rv) << endl;
                    EXIT_SUCCESS;
                }

                ressave = res;
                flag = 0;
                do {
                    temp_sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                    *sock_ptr = temp_sockfd;
                    //cout << res->ai_addr->sa_data << endl;
                    if (temp_sockfd < 0)
                        continue;
                    if (connect(temp_sockfd, res->ai_addr, res->ai_addrlen) == 0) {
                        //cout << res->ai_addr->sa_data << endl;
                        flag = 1;
                        break;
                    }
                    close(temp_sockfd);
                } while ((res = res->ai_next) != NULL);
                freeaddrinfo(ressave);

                if (flag == 0) {
                    fprintf(stderr, "cannot connect\n");
                    EXIT_SUCCESS;
                }
                connected_friends con;
                con.username = friend_name;
                con.sockfd = temp_sockfd;
                friends_sockfd.push_back(con);
		string final_message = client::current_username+">>"+message;
                write(temp_sockfd,final_message.c_str(),final_message.length());
                pthread_create(&tid, NULL, &message_handler, (void *)sock_ptr);
               

            }

            //sleep(1);


        }
        else {
            //char buf[MAXBUFLEN];
            write(sockfd, oneline.c_str(), oneline.length());
        }

    }
    EXIT_SUCCESS;

}
