#pragma once
#include "shape.h"

class Plane : public Shape
{
public:
	Plane(int divisionsX = 2 , int divisionsY = 2) : divisionsX(divisionsX) , divisionsY(divisionsY) 
	{
	};

	void Draw(bool draw_normals = false);

protected:
	bool PreGLInitialize();
	void NonGLTakeDown();

	int divisionsX;
	int divisionsY;
};