#include <boost/lexical_cast.hpp>
#include "Controller.h"
#include "Response.h"

namespace gtp {

Controller::Controller(unsigned BoardSize, float Komi) 
		: board_size{BoardSize}, komi{Komi} {

	cmdid = 0;
	pid = -1;
}

Controller::~Controller() {
	kill(pid, 0); // kill the engine process
}

void Controller::attach_engine(std::string engine) {
	// create and fork a new process
	pipe(engine_read);
	pipe(engine_write);
	pid = fork();

	// execute the engine on the newly forked child process
	if (pid == 0) {
		dup2(engine_read[1], STDOUT_FILENO); // pipe<-child (writing) as stdout
		dup2(engine_write[0], STDIN_FILENO); // pipe->child (reading) as stdin

		close(engine_read[0]); // read
		close(engine_write[1]); // write

		// replace the current process image with a new one
		// this will be our engine
		execl("/bin/gnugo", "/bin/gnugo", "--mode", "gtp", NULL);
		exit(1); // if we fail to run the engine, kill this process
	}

	close(engine_write[0]); // pipe->child (read)
	close(engine_read[1]); // pipe<-child (write)

	// get file pointers that we can conveniently use to talk to the engine
	engine_file_read = fdopen(engine_read[0], "r");
	engine_file_write = fdopen(engine_write[1], "w");
	
	// let the engine now some basic information about the game we will be playing.
	auto id = send_command("boardsize " + boost::lexical_cast<std::string>(board_size));
	read_response(engine_file_read, CMD_BOARDSIZE, id);

	id = send_command("komi " + boost::lexical_cast<std::string>(komi));
	read_response(engine_file_read, CMD_KOMI, id);

	id = send_command("clear_board");
	read_response(engine_file_read, CMD_CLEAR_BOARD, id);
}

void Controller::play_move(int x, int y, Player color) {
	std::string command = "play " + player_to_string(color) + " " + Vertex(x, y).to_string();
	auto id = send_command(command);
	read_response(engine_file_read, CMD_PLAY, id);
}

Vertex Controller::gen_move(Player color) {
	std::string command = "genmove " + player_to_string(color);
	auto id = send_command(command);
	std::string resp = read_response(engine_file_read, CMD_GENMOVE, id).response;
	return Vertex(resp);
}

void Controller::quit() {
	auto id = send_command("quit");
	read_response(engine_file_read, CMD_QUIT, id);
}

int Controller::send_command(const std::string &command) {
	// include the command id in the message we send
	std::string send = boost::lexical_cast<std::string>(++cmdid) + " " + command + "\n";
	write(engine_write[1], send.c_str(), strlen(send.c_str()));
	return cmdid;
}

}
