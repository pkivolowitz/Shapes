#pragma once
#include "shape.h"
#include "plane.h"

class Fan : public Shape
{
public:
	Fan();

protected:
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();

	float getDelta(bool isInverse, bool isZero);
	float getNormalX(bool isInverse, bool isZero);
	float getNormalY(bool isInverse, bool isZero);
	bool drawIt(int check);

	float delta;
	float zCoord;
	unsigned int numFins;
};