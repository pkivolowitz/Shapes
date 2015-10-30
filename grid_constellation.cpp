#include "grid_constellation.h"

using namespace glm;

bool GridConstellation::Initialize(int number_of_objects)
{
	// number_of_objects is a single paramter as (for now) the function call is generic. So,
	// synthesize a reasonable row and column count.
	int size = int(sqrt(number_of_objects));
	this->Clear();
	this->Resize(size * size);
	// The grid is designed to be over a -1 to 1 square. The consumer can scale these as desired.
	float delta = 2.0f / (size - 1.0f);

	vec3 p(-1.0f , -1.0f , 0.0f);

	int i , y;
	for (y = 0, i = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			this->positions[i++] = PositionData(p , vec3(0.0f , 0.0f , 1.0f));
			p.x += delta;
		}
		p.x = -1.0f;
		p.y += delta;
	}
	return true;
}
