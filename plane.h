#pragma once
#include "shape.h"

class Plane : public Shape
{
public:
	Plane(int divisionsX = 2, int divisionsY = 2);

	void Draw(bool draw_normals = false);
	glm::ivec2 Dimensions() { return glm::ivec2(this->divisionsX , this->divisionsY); }

protected:
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();

	int divisionsX;
	int divisionsY;
};

class Cube : public Shape
{
public:
	Cube() {}
	void Draw(bool draw_normals = false);
protected:
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();
};