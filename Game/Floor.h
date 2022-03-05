#pragma once
#include "Box.h"
#include <string>
class Floor :
	public Box
{
public:
	Floor(std::string path);
	~Floor();
};

