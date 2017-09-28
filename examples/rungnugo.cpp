#include <iostream>
#include "imGtp.h"

using namespace gtp;

int main(int argc, const char ** argv) {
	Controller c(9, 6.5f);
	c.attach_engine("gnugo --mode gtp");
	
	// have gnugo play against itself
	while (true) {
		std::cout << c.gen_move(PLAYER_BLACK) << std::endl;
		std::cout << c.gen_move(PLAYER_WHITE) << std::endl;
	}
	
	return 0;
}
