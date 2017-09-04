#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <string>
#include <sstream>
#include <ctype.h>
#include <iostream>

#include "Vertex.h"

namespace gtp {

Vertex::Vertex(std::string v) {
	iArgument::type = ARG_VERTEX;
	transform(v.begin(), v.end(), v.begin(), ::toupper);

	boost::algorithm::to_upper(v);

	// check if this is a passing move
	if (v == "PASS") {
		pass = true;
		h_pos = v_pos = -1;
		return;
	}

	// not a passing move, read the move as a stone
	char h;
	std::istringstream iss(v);
	iss >> h >> v_pos;
	h_pos = toupper(h) - 'A';
	// I is not a valid stone location, decrement if we're above it
	if (h > 'I') { h_pos--; }
	// Convert from a 1 based coordinate system to our internal 0 based one.
	v_pos--;
}

Vertex::Vertex(int x, int y, bool Pass) {
	iArgument::type = ARG_VERTEX;
	h_pos = x;
	v_pos = y;
	pass = Pass;
}

std::pair<int, int> Vertex::coords() {
	return std::make_pair(h_pos, v_pos);
}

std::string Vertex::to_string() {
	// passing moves are easy - simply use the text 'pass'
	if (is_pass()) { return "PASS"; }

	// board positions are in the range A->T excluding I
	// add one to skip I if necessary
	char arr[] = { (char)('A' + h_pos + (h_pos >= 8 ? 1 : 0)), '\0' };
	return std::string(arr) + std::to_string(v_pos + 1);
}

}
