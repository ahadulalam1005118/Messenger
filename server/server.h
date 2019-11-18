//
// Created by ahad on 10/18/18.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include<string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
struct config {
    string name;
    unsigned int value;
};
struct user_info {
    string name;
    string password;
    vector<string> friends;
};
struct logged_in_user {
    string username;
    string ip_addr;
    int port;
    int sock_fd;
};
struct invitation {
    string from_username;
    string to_username;
};
class server {
public:
    int begin(std::string user_info_file_path, std::string config_file_path);
    int run();
    int exit_from_server();
    int server_sockfd;
    //void sigint_handler(int sig_num);
    /*vector<config> get_configurations(std::string filename);
    vector<user_info> get_users(string filename);
    user_info get_user_info_by_name(string username);*/
private:

    vector<invitation> invitations;
    bool is_logged_in(string username);
    void send_data_to_client(int sockfd, std::string command, std::string data);
    vector<config> get_configurations(std::string filename);
    vector<user_info> get_users(string filename);
    user_info get_user_info_by_name(string username);
    config split_string_config(string s, string delimiter);
    user_info split_string_user_info(string s, string delimiter);
    vector<user_info> current_users;
    vector<config> current_configurations;
    int is_available(string username, string password);
    int user_exists(string username, string password);
    template<typename Out> void split(const std::string &s, char delim, Out result);
    vector<string> split(const std::string &s, char delim);
    void add_logged_in_user(logged_in_user usr);
    vector<logged_in_user> get_online_users();
    vector<logged_in_user> online_users;
    vector<logged_in_user> get_logged_in_friends(logged_in_user new_user);
    string get_username_from_sockfd(int sock_fd);
    int get_sockfd_from_username(string username);
    void write_all_infos_to_file();
    void evict_user_from_online_users(string username);
    void send_logout_info_to_friends(string username);
    unsigned int port;
    //unsigned int get_port_from_configuration(std::string filename);
    logged_in_user get_logged_in_user_by_name(string username);
    std::vector<int> sockfds;
    int cli_sockfd;
    std::string user_info_file_path;
    std::string configuration_file_path;
    std::unordered_map<int, std::string> sockfd_to_username;
    std::string db_path;
    std::string file_contents;
    std::string get_fully_qualified_domain_name();
    int get_port_from_configuration_map();
    std::string get_db_path_from_configuration_map();
    void handle_command_from_client(int sockfd, std::vector<std::string> parsed_command);
    //void send_data_to_client(int sockfd, std::string command, std::string data);
    void send_location_info_to_clients(std::string username);
    void send_logout_info_to_clients(std::string username);
    static void sigint_handler(int signal);
    static server *_server;
    void broad_cast_data_to_other_clients(int in_sockfd, std::string command, std::string data);
    void pull_file_from_other_client(int in_sockfd);


};


#endif //SERVER_SERVER_H
