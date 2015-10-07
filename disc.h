#pragma once
#include "shape.h"

class Disc : public Shape
{
public:
	Disc(int slices, float span, float outer_radius, float inner_radius = 0.0f);
	void Draw(bool draw_normals = false);

protected:
	bool PreGLInitialize();
	void NonGLTakeDown();

	int slices;
	float span;
	float inner_radius;
	float outer_radius;
	bool is_partial_span;
};
