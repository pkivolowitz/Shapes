#pragma once
#include "constellation.h"
#include <ctime>
class DiamondConstellation : public Constellation
{
public:
	DiamondConstellation() : Constellation()
	{
		switch_constellations = false;
	}
	bool Initialize(int number_of_objects);
	bool Reinitialize(int number_of_objects);
	bool getSwitchConstellations() { return switch_constellations; };
	void setSwitchConstellations(bool sc) { switch_constellations = sc; };
private:
	bool switch_constellations;
};

