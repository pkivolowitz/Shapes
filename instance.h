#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

class Instance
{
public:
	Instance(glm::vec3 p, float o, float r, glm::vec3 d) : position(p), offset(o), rate(r), diffuse(d) {};
	static void DefineInstances(std::vector<Instance> & instances, int n = 32);

	glm::vec3 position;
	float offset;
	float rate;
	glm::vec3 diffuse;
};
