#include "Controller.h"

namespace gtp {

Controller::Controller(unsigned BoardSize, float Komi) 
		: board_size{BoardSize}, komi{Komi} {

	engine = nullptr;
}

Controller::~Controller() {
	if (engine) { pclose(engine); }
}

void Controller::attach_engine(std::string Engine) {
	// initialize the engine, and give it some information about 
	// the game we are asking it to play
	engine = popen(Engine.c_str(), "w");
	fprintf(engine, "boardsize %d", board_size);
	fprintf(engine, "komi %f", komi);
	fprintf(engine, "clear_board");
}

}
