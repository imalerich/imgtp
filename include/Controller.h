#ifndef IMGTP_CONTROLLER_H
#define IMGTP_CONTROLLER_H

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "Command.h"
#include "Argument.h"
#include "Move.h"

namespace gtp {

class Controller;

typedef std::shared_ptr<Controller> Controller_p;

class Controller {
public:
	Controller(unsigned BoardSize, float Komi);
	~Controller();

	/**
	 * \fn void attach_engine(std::string Engine);
	 * Setup a new controller to interface with an engine.
	 * This will initialize a new game with the given engine
	 * program (name and arguments given as string), using the
	 * provided board size and komi.
	 */
	void attach_engine(std::string Engine);

	/**
	 * \fn void play_move(int x, int y, Player color);
	 * \brief Tell the engine that an external move was made
	 * by the given player at the given position.
	 */
	void play_move(int x, int y, Player color);

	/**
	 * \fn Vertex gen_move(Player color);
	 * \brief Tell the engine to make a move for the given color.
	 * Then, read the response, and return the Vertex that the engine chose.
	 */
	Vertex gen_move(Player color);

	/**
	 * \fn void quit
	 * \brief Terminate the current session with the engine.
	 */
	void quit();

private:
	const unsigned board_size;
	const float komi;
	int cmdid;

	int engine_read[2];
	int engine_write[2];
	FILE * engine_file_read;
	FILE * engine_file_write;
	pid_t pid;

	/**
	 * Send's the input command string as input to the
	 * engine's input buffer.
	 * This will return the command id used for that command,
	 * this is added automatically and should not be included
	 * in the command.
	 * Carriage returns are also not necessary,
	 * but shouldn't hurt anything.
	 */
	int send_command(const std::string &command);
};

}

#endif
