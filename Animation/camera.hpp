#ifndef camera_H
#define camera_H

#include "movable.hpp"
#include <string>

class camera : public movable
{
	public:

	camera();
	~camera();

	void init();
	void look();

	float radius;	
};

#endif
