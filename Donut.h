#pragma once
#include "plane.h"

class Donut : public Plane
{
public:
	Donut(int slices, int stacks, float span, float back_radius, float front_radius);

protected:
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();

	int slices;
	int stacks;
	float span;
	float fr;
	float br;
	bool is_partial_span;
};