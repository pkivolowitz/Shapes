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
			this->positions[i++] = PositionData(p , vec3(0.0f, 0.0f, 1.0f));
			p.x += delta;
		}
		p.x = -1.0f;
		p.y += delta;
	}
	return true;
}

bool SpiralConstellation::Initialize(int number_of_objects)
{
	float delta_y = 0.005f;

	mat4 m;
	m = translate(m , vec3(0.0f , -delta_y * number_of_objects / 2.0f , 0.0f));

	vec4 p(1.0f , 0.0f , 0.0f , 1.0f);
	this->Resize(number_of_objects);

	for (int i = 0; i < number_of_objects; i++)
	{
		this->positions[i] = PositionData(vec3(m * p) , vec3(p.x , 0.0f , p.z));
		m = rotate(m , radians(5.0f) , vec3(0.0f , 1.0f , 0.0f));
		m = translate(m , vec3(0.0f , delta_y , 0.0f));
	}
	return true;
}

bool SphereConstellation::Initialize(int number_of_objects)
{
	// number_of_objects is a single paramter as (for now) the function call is generic. So,
	// synthesize a reasonable row and column count.
	int size = int(sqrt(number_of_objects));
	this->Clear();
	this->Resize(size * size);
	float theta = 0, phi = 0;
	for (size_t i = 0; i < positions.size(); i++)
	{
		float scale = float(rand()) / float(RAND_MAX) / 2.0f + 0.5f;
		theta = 2.0f * glm::pi<float>() * float(rand()) / float(RAND_MAX);
		// corrrect
		phi = acos(2.0f * float(rand()) / float(RAND_MAX) - 1.0f);
		vec3 p(cos(theta)*sin(phi) * scale, sin(theta)*sin(phi) * scale, cos(phi) * scale);
		positions[i] = PositionData(p, -p);
	}
	//this->positions[i++] = PositionData(p, vec3(0.0f, 0.0f, 1.0f));
	return true;
}
