#ifndef IMGTP_VERTEX_H
#define IMGTP_VERTEX_H

#include <string>
#include <utility>

#include "Argument.h"

namespace gtp {

/**
 * Vertex representation for the Gnu Text Protocol.
 * This object make be initialized from a string in the format
 * A10, where A is the horizontal position of the vertex (Left->Right)
 * and 10 is the Vertical Position of the vertex (Bottom->Top).
 */
class Vertex : public iArgument {
public:
	/** Needed so we can store it as a non-reference in the move container. */
	Vertex() { }

	/**
	 * Constructs a vertex object from the string representation returned by 
	 * the Gnu Text Protocol.
	 */
	Vertex(std::string v);

	/**
	 * Construct a vertex from set x, y values.
	 * \param x X-Coordinate of the vertex, should be in the range [0, BOARD_SIZE)
	 * \param y Y-Coordinate of the vertex, should be in the range [0, BOARD_SIZE)
	 */
	Vertex(int x, int y, bool Pass = false);

	/**
	 * Returns the coordinate representation of this vertex as a
	 * pair of unsigned integers.
	 */
	std::pair<int, int> coords() const;

	/**
	 * Convert this vector into a string representation that
	 * may be output by the GTP engine.
	 */
	std::string to_string() const;

	friend std::ostream& operator<<(std::ostream &out, const Vertex &v);

	const inline bool is_pass() const { return pass; }
	const inline bool is_stone() const { return !pass; }
	const inline int get_x() const { return h_pos; }
	const inline int get_y() const { return v_pos; }

private:
	/// Horizontal position of this vertex where 0 is the left edge of the board.
	int h_pos;
	/// Vertical position of this vertex, where 0 is the bottom edge of the board.
	int v_pos;
	// if this is not a positional vertex, and is instead a pass, this will be true
	bool pass;
};

}

#endif
