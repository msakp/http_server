#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include "../MessageParsing/requestParser.hpp"
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <signal.h>
#include <vector>

namespace hsimple{


void on_termination(int sig); // SIGINT signals handler

class Server{
	private:
		Request* _request;	
		int _socket_fd;
		int _data_fd; 
		int _port; // human readable port num;
		struct sockaddr_in _address;
		int _connected;
		int _listening;
		std::string index_html = "hello from server";

		inline int socket_failed(){
			return (_socket_fd < 0)? true: false;
		}
		inline int connection_failed(){
			return (_connected < 0)? true: false;
		}
		inline int listening_failed(){
			return (_listening < 0)? true: false;
		}
		int bridge();
		int start_listening();
		void next_request();
		void handle_request();

		void test_socket();
		void test_connection();
		void test_listening();


	public:
		Server(int port);
		~Server();
		void run();
		struct sockaddr_in get_address();
		int get_socket();

	};

extern std::vector<Server*> SERVERS;

}

#endif
