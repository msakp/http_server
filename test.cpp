#include "hsslibc++.hpp"
#include <iostream>

using namespace hsimple;

int main(){
	auto app = Server(8080);
	app.run();
	return 0;
}
