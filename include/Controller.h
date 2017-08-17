#ifndef IMGTP_CONTROLLER_H
#define IMGTP_CONTROLLER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "Command.h"
#include "Argument.h"

namespace gtp {

class Controller {
public:
	Controller(unsigned BoardSize, float Komi, std::string Engine);
	~Controller();

	/**
	 * \fn void attach_engine(std::string Engine);
	 * Setup a new controller to interface with an engine.
	 * This will initialize a new game with the given engine
	 * program (name and arguments given as string), using the
	 * provided board size and komi.
	 */
	void attach_engine(std::string Engine);

private:
	const unsigned board_size;
	const float komi;
	FILE * engine;
};

}

#endif
