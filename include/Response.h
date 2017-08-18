#ifndef IMGTP_RESPONSE_H
#define IMGTP_RESPONSE_H

#include <iostream>
#include <string>
#include <cctype>

#include "Command.h"

namespace gtp {

class Response;

/**
 * C-style call to create a response.
 * Useful if you don't need to actually keep the response around.
 */
Response read_response(FILE * engine, Command Type, int ID = -1);

class Response {
public:
	/**
	 * Read the next response provided by the input engine,
	 * then create a new response object from that data.
	 * If the command sent an id, provide that in the third argument,
	 * if not the default value of -1 indiciates that it will be missing.
	 * The command type will indicate how to read the response value.
	 */
	Response(FILE * engine, Command Type, int ID = -1);

	const int id; 
	const Command type;
	std::string response;
};

}

#endif
