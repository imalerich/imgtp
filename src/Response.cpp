#include <boost/lexical_cast.hpp>
#include <iostream>

#include "ParserUtil.h"
#include "Response.h"

namespace gtp {

Response read_response(FILE * engine, Command Type, int ID) {
	return Response(engine, Type, ID);
}

Response:: Response(FILE * engine, Command Type, int ID) :
		id{ID}, type{Type} {
	// read a line from engine
	char * buffer = NULL;
	size_t size;
	response = "";

	// find a non-ignored line containing the response
	bool found_result = false;
	while (!found_result) {
		if (getline(&buffer, &size, engine) != -1) {
			response = buffer;
			response = preproc_line(response);

			if (ignore_line(response)) {
				response = "";
				continue;
			}

			found_result = true;

			// rip leading white space if present
			while (isspace(response[0])) {
				response = response.erase(0, 1);
			}

			response = response.erase(0, 1); // remove the '=' character

			// rip the command id from the response if it was provided
			if (id >= 0) {
				// make sure the command starts with the correct string id
				std::string strid = boost::lexical_cast<std::string>(id);
				std::string comp = response.substr(0, strid.size());
				if (strid != comp) {
					std::cerr << "Unexpected response id found!" << std::endl;
					exit(1);
				}

				response = response.substr(strid.size());
			}

			// rip the remaining leading white space to get the actual response
			while (isspace(response[0])) {
				response = response.erase(0, 1);
			}
	
			// now just rip the trailing white space
			while (isspace(response[response.size()-1])) {
				response = response.erase(response.size()-1, 1);
			}

		} else {
			// error - nothing left to read
			break;
	}}

	delete[] buffer;
}

}
