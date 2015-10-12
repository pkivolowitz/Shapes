#include "instance.h"

using namespace std;
using namespace glm;

inline float Random(float min, float max)
{
	return float(rand()) / float(RAND_MAX) * (max - min) + min;
}

void Instance::DefineInstances(vector<Instance> & instances, int n)
{
	float d = 8.0f;

	for (int i = 0; i < n; i++)
	{
		mat4 m = rotate(mat4(), Random(0.0f, pi<float>() * 2.0f), vec3(0.0f, 1.0f, 0.0f));
		m = rotate(m, Random(0.0f, pi<float>() * 2.0f), vec3(1.0f, 0.0f, 0.0f));
		float z = Random(-8.0f, 8.0f);
		z += (z > 0) ? 2.0f : -2.0f;

		vec4 p = m * vec4(0.0f, 0.0f, z, 1.0f);

		vec3 diffuse(Random(0.2f, 1.0f), Random(0.2f, 1.0f), Random(0.2f, 1.0f));
		instances.push_back(Instance(
			vec3(p),
			Random(0.0f, pi<float>() * 2.0f),
			Random(-360.0f, 360.0f),
			diffuse));
	}
}
