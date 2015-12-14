#pragma once
#include "shape.h"

class Torus : public Shape
{
public:
	Torus(float innerRadius = 0.25f, float outerRadius = 1.0f, int sides = 64, int rings = 64);
protected:
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();

	float inner_radius;
	float outer_radius;
	int sides;
	int rings;
};

