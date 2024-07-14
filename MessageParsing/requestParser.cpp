#include "requestParser.hpp"
#include <string>

void hsimple::Request::flush(){
	memset(_buffer, 0, REQUEST_BUFFER_SIZE);
}

hsimple::Request::Request(){
	flush();
}

void hsimple::Request::parse_from(int fd){
	read(fd, _buffer, REQUEST_BUFFER_SIZE);
	std::string bufstr(_buffer);
	std::string header = bufstr.substr(0, bufstr.find("\r\n"));
	std::vector<std::string> rich_info{};
	int start = 0;
	//"GET /favicon.ico HTTP/1.1"
	//std::cout << header << '\n';
	for (int i = 0; i < header.length(); i++){
		if (header[i] == ' '){
			rich_info.push_back(header.substr(start, i - start));
			start = i + 1;
		}
		else if (i == header.length() - 1){
			rich_info.push_back(header.substr(start, i - start + 1));
		}
	}
	_path = rich_info[0];
	_version = rich_info[1];
	_type = rich_info[2];
}

std::string hsimple::Request::type(){
	return _type;
}

std::string hsimple::Request::path(){
	return _path;
}

std::string hsimple::Request::version(){
	return _version;
}

std::string hsimple::Request::user_agent(){
	return _user_agent;
}
