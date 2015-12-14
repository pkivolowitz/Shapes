#include "TodConstellation.h"

using namespace std;
using namespace glm;

// Tod - what shape is a Tod?
bool TodConstellation::Initialize(int number_of_objects)
{
	// number_of_objects is a single paramter as (for now) the function call is generic. So,
	// synthesize a reasonable row and column count.
	int size = int(sqrt(number_of_objects));
	float theta = pi<float>() * 2.0f / float(size);
	this->Clear();
	// Todd this should not be here if there is a push back below.
	//this->Resize(size * size);

	// The grid is designed to be over a -1 to 1 square. The consumer can scale these as desired.
	vector <vec3> singleCircle;
	vec4 P(0.5f, 0.0f, 0.0f, 1.0f);
	mat4 M;

	for (int i = 0; i < size; i++)
	{
		singleCircle.push_back(vec3(M * P));
		M = rotate(M, theta, vec3(0.0f, 0.0f, 1.0f));
	}

	mat4 T = translate(mat4(), vec3(3.0f, 0.0, 0.0));
	mat4 R;

	// bug here too.
	for (int i = 0; i < size; i++)
	{
		// bug here too.
		for (size_t j = 0; j < singleCircle.size(); j++)
		{
			// the 4th coordinate being zero is an issue.
			vec3 p = vec3(R * T * vec4(singleCircle[j], 0.0f));

			this->positions.push_back(PositionData(p, normalize(-p))); //make 0.0f for coin, was 1.0f
		}

		R = rotate(R, theta, vec3(1.0f, 1.0f, 1.0f)); //made x and z 1.0f from 0.0f
	}
	return true;
}