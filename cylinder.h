#pragma once
#include "shape.h"

class Cylinder : public Shape
{
public:
	Cylinder(int slices, int stacks, float span);
	void Draw(bool draw_normals = false);

protected:
	bool PreGLInitialize();
	void NonGLTakeDown();

	int slices;
	int stacks;
	float span;
	bool is_partial_span;
};
