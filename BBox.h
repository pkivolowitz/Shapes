#pragma once
#include "constellation.h"

// I hate putting code in headers. But I'm doing it anyway. May God have mercy on my soul.

class BBox : public Constellation
{
public:
	BBox() : Constellation()
	{
	}

	bool Initialize(int x);
};