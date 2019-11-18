//
// Created by ahad on 11/14/18.
//
#include <iostream>
#include <vector>

using namespace std;
#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#endif //CLIENT_CLIENT_H
struct config {
    string name;
    string argv;
};
struct friends_info {
    string username;
    string ip_addr;
};
struct connected_friends {
    string username;
    int sockfd;
};
//vector<connected_friends> con[10];
class client {
public:
    int begin(std::string config_file_path);
    int run();
    int exit();

private:
    static string current_username;
    static bool login_status;
    static void * message_handler(void *arg);
    static void * process_connection(void *arg);
    static void * process_connection_client(void *arg);
    static vector<friends_info> friends_list;
    static vector<connected_friends> friends_sockfd;
    int is_connected(string username);
    vector<config> get_configurations(std::string filename);
    config split_string_config(string s, string delimiter);
    int exists(string username);
    //vector<string> split(const std::string &s, char delim);
    //user_info split_string_user_info(string s, string delimiter);
    template<typename Out> void split(const std::string &s, char delim, Out result);
    vector<string> split(const std::string &s, char delim);
    static int get_sockfd_from_username(string username);
    string configuration_file_path;
    void handle_loggedin_command_from_server();
    string get_friend_address(string username);
    void evict_user_from_friends(string username);
    static client *_client;

};
