#include <iostream>
#include "imGtp.h"

using namespace gtp;

int main(int argc, const char ** argv) {
	Controller c(9, 6.5f);
	c.attach_engine("/bin/gnugo --mode gtp");
	
	// have gnugo play against itself
	while (true) {
		c.gen_move(PLAYER_BLACK);
		c.gen_move(PLAYER_WHITE);
	}
	
	return 0;
}
