#pragma once
#include "constellation.h"

class TodConstellation : public Constellation
{
public:
	TodConstellation() : Constellation()
	{
	}

	bool Initialize(int number_of_objects);
};