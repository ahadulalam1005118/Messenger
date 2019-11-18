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
struct event_info {
    string hostname;
    string date;
    string start_time;
    string end_time;
    string event_desc;
};
//vector<connected_friends> con[10];
class client {
public:
    void write_to_sockfd(string str);
    //vector<event> current_events;
    //vector<event_info>get_events();
    vector<friends_info>get_online_friends();
    //static int sockfd;
    int begin(std::string config_file_path);
    int run();
    int exit();
    //void write_to_sockfd(string str);
    static bool login_status;
    static bool reg_status;
    static string username;
    static vector<event_info> current_events;
    //void handle_message_command();

private:
    //static vector<event> current_events;
    static string current_username;
    //static bool login_status;
    static void * message_handler(void *arg);
    static void * process_connection(void *arg);
    static void * process_connection_client(void *arg);
    static vector<friends_info> friends_list;
    static vector<connected_friends> friends_sockfd;
    int is_connected(string username);
    vector<config> get_configurations(std::string filename);
    //void write_to_sockfd(string str);
    config split_string_config(string s, string delimiter);
    int exists(string username);
    //vector<string> split(const std::string &s, char delim);
    //user_info split_string_user_info(string s, string delimiter);
    template<typename Out> void split(const std::string &s, char delim, Out result);
    vector<string> split(const std::string &s, char delim);
    void write_events_to_file(vector<event_info>events);
    string configuration_file_path;
    void handle_loggedin_command_from_server();
    string get_friend_address(string username);
    void evict_user_from_friends(string username);
    static client *_client;

};
