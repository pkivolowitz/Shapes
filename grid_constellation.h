#pragma once
#include "constellation.h"

// I hate putting code in headers. But I'm doing it anyway. May God have mercy on my soul.

class GridConstellation : public Constellation
{
public:
	GridConstellation() : Constellation()
	{
	}

	bool Initialize(int number_of_objects);
};