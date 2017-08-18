#include <iostream>
#include "imGtp.h"

using namespace gtp;

int main(int argc, const char ** argv) {
	Controller c(19, 6.5f);
	c.attach_engine("gnugo --mode gtp");
	c.play_move(0, 0, PLAYER_BLACK);
	std::cout << c.gen_move(PLAYER_WHITE).to_string() << std::endl;
	return 0;
}
