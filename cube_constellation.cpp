#include "cube_constellation.h"
#include <vector>

using namespace glm;

bool CubeConstellation::Initialize(int number_of_objects)
{
	// number_of_objects is a single paramter as (for now) the function call is generic. So,
	// synthesize a reasonable row and column count.
	int size = int(sqrt(number_of_objects / 6.0));
	this->Clear();
	this->Resize((size * size) * 6);
	// The grid is designed to be over a -1 to 1 square. The consumer can scale these as desired.
	float delta = 2.0f / (size - 1.0f);

	std::vector < PositionData > temp;

	vec3 p(-1.0f, -1.0f, 0.0f);
	vec3 D(0.0f, 0.0f, 1.0f);
	int i = 0;
	for (int y = 0; y < size; y++)
	{
		//i = 0;
		for (int x = 0; x < size; x++)
		{
			temp.push_back(PositionData(p, D));
			p.x += delta;
		}
		p.x = -1.0f;
		p.y += delta;
	}

	float deltat = 1.25f;

	mat4 T = translate(mat4(), vec3(0.0f, 0.0f, deltat));
	mat4 R;
	mat4 S = rotate(mat4(), pi < float >() / -2.0f, vec3(1.0f, 0.0f, 0.0f));
	mat4 Q;

	for (int k = 0; k < 4; k++)
	{
		for (unsigned int j = 0; j < temp.size(); j++)
		{
			vec3 d = vec3(R * vec4(0.0f, 0.0f, 1.0f, 0.0f));
			vec3 p = vec3(R * T * vec4(temp[j].location, 1.0f));
			this->positions[i++] = PositionData(p, d);
		}
		// R spins around the Y axis to make the four sides.
		R = rotate(R, pi <float>() / 2.0f, vec3(0.0f, 1.0f, 0.0f));
		Q = rotate(R, pi <float>() / (((k%2 == 0) ? -1 : 1) * 2.0f), vec3(0.0f, 1.0f, 0.0f));
	}

	T = translate(mat4(), vec3(0.0f, -deltat, 0.0f));
	R = mat4();
	for (int m = 0; m < 2; m++)
	{
		for (unsigned int j = 0; j < temp.size(); j++)
		{
			vec3 d = vec3(R * S * vec4(temp[j].outward_direction_vector, 1.0f));
			vec3 p = vec3(R * T * S * vec4(temp[j].location, 1.0f));
			this->positions[i++] = PositionData(p, d);
		}
		R = rotate(R, pi <float>(), vec3(1.0f, 0.0f, 0.0f));
	}

	return true;
}
