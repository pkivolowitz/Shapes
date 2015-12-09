#include <vector>
#include "BBox.h"
#include "plane.h"

using namespace glm;
using namespace std;

bool BBox::Initialize(int x)
{
	positions.resize(x);

	int size = x / 6;
	size = int(sqrt(size));
	
	int divisionsX;
	int divisionsY;

	divisionsY = divisionsX = size - 1;

	float deltaX = 2.0f / float(divisionsX);
	float deltaY = -2.0f / float(divisionsY);

	vec3 v(-1.0f, 1.0f, 0.0f);

	vector<vec3> points;

	for (int y = 0; y < divisionsY + 1; y++)
	{
		for (int x = 0; x < divisionsX + 1; x++)
		{
			points.push_back(v);
			v.x = v.x + deltaX;
		}
		v.y += deltaY;
		v.x = -1.0f;
	}

	mat4 R;
	mat4 T = translate(mat4(), vec3(0.0, 0.0, 1.0));
	vec3 d(0.0, 0.0, 1.0);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < int(points.size()); j++)
		{
			vec3 q(R * vec4(d, 1.0f));
			vec3 p(R * T * vec4(points[j], 1.0f));

			positions.push_back(PositionData(p, q));
		}

		R = rotate(R, pi<float>() / 2.0f, vec3(0.0, 1.0, 0.0));
	}

	T = translate(mat4(), vec3(0.0, -1.0, 0.0));
	R = mat4();
	mat4 S = rotate(mat4(), pi<float>() / 2.0f, vec3(1.0f, 0.0f, 0.0f));
	d = vec3(0.0, -1.0, 0.0);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < int(points.size()); j++)
		{
			vec3 q(R * vec4(d, 1.0f));
			vec3 p(R * T * S * vec4(points[j], 1.0f));

			positions.push_back(PositionData(p, q));
		}
		
		R = rotate(R, pi<float>(), vec3(1.0, 0.0, 0.0));
	}

	return true;
}
