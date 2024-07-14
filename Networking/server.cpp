#include "server.hpp"
#include <asm-generic/socket.h>
#include <sys/socket.h>

std::vector<hsimple::Server*> hsimple::SERVERS{};


void hsimple::on_termination(int sig){
	std::cout << "\b\b==============================================\n";
	std::cout << "- C^ catched,\n";
	for (Server* serv: SERVERS)
		serv->~Server();
	exit(0);
}

void hsimple::Server::test_socket(){
	if (socket_failed()){
		std::cerr << "socket failed\n";
		abort();
	}
}

void hsimple::Server::test_connection(){
	if (connection_failed()){
		std::cerr << "connection failed\n";
		abort();
	}
}

void hsimple::Server::test_listening(){
	if (listening_failed()){
		std::cerr << "listening failed\n";
		abort();
	}
}

int hsimple::Server::bridge(){
	return bind(_socket_fd, (struct sockaddr*)&_address, sizeof(_address));
}

hsimple::Server::Server(int port){
	// init current request sub-class
	_request = new Request();

	// saving human-readable port
	_port = port;

	// making socket file descriptor
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	test_socket();
	
	// setting socket for immediate re-use after destruct;
	int opt_on = 1;
	setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt_on, sizeof(opt_on));

	// setting up address for network bridging
	_address.sin_port = htons(port);
	_address.sin_family = AF_INET;
	
	_address.sin_addr.s_addr = htonl(INADDR_ANY); 
	
	// network bridging
	_connected = bridge();
	test_connection();

	// setting up for listening
	_listening = listen(_socket_fd, 20);
	test_listening();

	// adding server to global listing for destructing purposes
	SERVERS.push_back(this);
}

hsimple::Server::~Server(){
	delete _request;
	close(_socket_fd);
	std::cout << "socket closed.\n";
}


struct sockaddr_in hsimple::Server::get_address(){
	return _address;
}

int hsimple::Server::get_socket(){
	return _socket_fd;
}

void hsimple::Server::next_request(){
	int addrlen = sizeof(_address); // ulong to int convertion for socklen_t* cast;
	_data_fd = accept(_socket_fd, (struct sockaddr*)&_address, (socklen_t*)&addrlen);	
	if (_data_fd < 0){
		std::cerr << "pending a request failed";
		abort();
	}
	_request->parse_from(_data_fd);
}

void hsimple::Server::handle_request(){
  	std::cout << _request->type() << '\n';
		std::cout << _request->path() << '\n';
		std::cout << _request->version() << '\n';
		write(_data_fd, index_html.c_str(), strlen(index_html.c_str()));
}
void hsimple::Server::run(){
	signal(SIGINT, on_termination);
	printf("** working port %d **  |  (Ctrl+C to quit)\n", _port);
	std::cout << "============ waiting for requests ============\n";
	while (true){
		next_request(); // accepting next request on _data_fd;
	  handle_request(); // handling request after parsing;
		close(_data_fd); // ending cycle;
		std::cout << "-------------------\n";
	}
}



