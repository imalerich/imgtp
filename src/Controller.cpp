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
	if (pid < 0) {
		std::cerr << "ERROR - Fork Failed... Exiting" << std::endl;
		exit(-1);
	} else if (pid == 0) { // child process
		dup2(engine_read[1], STDOUT_FILENO); // pipe<-child (writing) as stdout
		dup2(engine_write[0], STDIN_FILENO); // pipe->child (reading) as stdin

		close(engine_read[0]); // read
		close(engine_write[1]); // write

		// block of data containing each of our arguments
		// each argument is NULL termianted, with the array itself being null terminated
		// add 2 to icnlude terminating NULL of the c_string as well as terminating NULL of array
		char * args = (char *)malloc(sizeof(char) * (engine.size()+2));
		std::strcpy(args, engine.c_str());

		// replace white space characters with NULL characters
		for (char * c=args; *c != '\0'; ++c) {
			if (isspace(*c)) { *c = '\0'; }
		}
		args[engine.size()+1] = '\0'; // NULL terminate the array

		// next, we need to count how many arguments we have
		size_t num_args = 0;
		for (char * arg = args; arg[0] != '\0'; arg += (strlen(arg) + 1)) {
			++num_args;
		}

		// then create an array pointing to each starting character for each argument
		char ** arr = (char **)malloc(sizeof(char *) * (num_args + 1));
		int i=0;
		for (char * arg = args; arg[0] != '\0'; arg += (strlen(arg) + 1)) {
			arr[i++] = arg;
		}
		arr[num_args] = NULL;

		// replace the current process image with a new one
		// this will be our engine
		execvp(arr[0], arr);
		std::cerr << "ERROR - Failed to Run Engine... Exiting" << std::endl;
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
