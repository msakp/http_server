#ifndef SERV_REQUEST_HPP
#define SERV_REQUEST_HPP
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <iostream>

namespace hsimple{
constexpr int REQUEST_BUFFER_SIZE = 50000;

class Request{
	private:
		char _buffer[REQUEST_BUFFER_SIZE];
		std::string _type;
		std::string _path;
		std::string _user_agent;
		std::string _version;
		void flush();
	public:
		Request();
		void parse_from(int fd);

		std::string type();
		std::string path();
		std::string user_agent();
		std::string version();
};

}


#endif
