#ifndef IMGTP_MOVE_H
#define IMGTP_MOVE_H

#include "Argument.h"
#include "Player.h"
#include "Vertex.h"

namespace gtp {

/** Implements the Move argument type. */
class ArgMove : public iArgument {
public:
	ArgMove(std::string color, std::string vertex);
	ArgMove(Color color, Vertex vertex);

	/** Encapsulated Color data. */
	Color color;
	/** Encapsulated Vertex data. */
	Vertex vertex;
};

}

#endif
