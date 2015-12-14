#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/noise.hpp>
#include "Shape.h"

class Star : public Shape
{
public:
	Star(float dimension = 1.0f);

private:
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();
	void StarFace(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);
	void InitializeStar();
	void AddTextureCoordinates();
	void RecomputeNormalsSingleTriangle(std::vector<glm::vec3>& v, std::vector<glm::vec3>& n, std::vector<glm::vec3>& p, int c, int d, int e, int i, int& j);

	float d;
};