#include <iostream>
#include <stdexcept>
#include "fan.h"

//Lave these defined for some reason
#define TRIANGLES

using namespace std;
using namespace glm;

Fan::Fan() {
	//trying to make this fan within a 2 pt box
	//and 3 pts stretch from mid box, so lets make it uniformed
	//we are starting at .4, -.4

	delta = 2.0f / 5.0f;
	numFins = 4;
	zCoord = 0.075f;
}

bool Fan::PreGLInitialize()
{
	//variables to create shape
	bool isInverseX, isInverseY, isZero, wasZero;
	float nX, nY, nZ = 0.0f;

	//Gonna Need a for loop to create the second face
	for (unsigned int t = 0; t < 6; t++)
	{
		//set the variables, need to do this twice since theres 2 faces
		vec3 startPt(delta / 2, -delta / 2, zCoord);
		isInverseX = true;
		isInverseY = true;
		isZero = true;
		wasZero = false;

		//for loop for creating the vertices, textures, normals, colors, normal vis
		//setup for each of the 4 blades
		for (unsigned int i = 0; i < numFins; i++)
		{
			//delta isZero if no movement (if vertical blade, then isZero is true)
			if (i == 0 || i == 2)
			{
				isZero = true;
			}
			else
			{
				isZero = false;
			}

			//make the blade, which has 4 points each
			for (unsigned int a = 0; a < numFins; a++)
			{
				//setup default values
				nX = 0.0f, nY = 0.0f, nZ = 0.0f;

				//setup vertices, textures, normals, etc
				this->data.vertices.push_back(startPt);
				this->data.textures.push_back(vec2(startPt.x / 2.0f + 0.5f, startPt.y / 2.0f + 0.5f));
				this->data.colors.push_back(this->RandomColor((this->data.colors.size() > 0 ? *(this->data.colors.end() - 1) : vec4(0.5f, 0.5f, 0.5f, 1.0f)), -0.2f, 0.2f));

				//NORMALS
				//Setup the the two planes (normal with respect to z)
				if (t == 0)
				{
					nZ = 1.0f;
				}
				else if (t == 1)
				{
					nZ = -1.0f;
				}

				//normals for the sides
				if (t == 2 || t == 3)
				{
					if (isZero || wasZero)
					{
						if (startPt.x > 0)
							nX = 1.0f;
						else
							nX = -1.0f;
					}
					else if (a == 1 || a == 2)
					{
						if (startPt.x > 0)
							nX = 1.0f;
						else
							nX = -1.0f;
						if (startPt.y > 0)
							nY = 1.0f;
						else
							nY = -1.0f;

						if (a == 2)
						{
							nY = -nY;
						}
					}
				}
				//normals for top and bottom
				if (t == 4 || t == 5)
				{
					if (!isZero || wasZero)
					{
						if (startPt.y > 0)
							nY = 1.0f;
						else
							nY = -1.0f;
					}
					else if (a == 1 || a == 2)
					{
						if (startPt.x > 0)
							nX = 1.0f;
						else
							nX = -1.0f;
						if (startPt.y > 0)
							nY = 1.0f;
						else
							nY = -1.0f;

						if (a == 2)
						{
							nX = -nX;
						}
					}
				}

				//need this for interior normals
				if (a == 3)
					wasZero = true;
				else
					wasZero = false;

				if (a == 0 && t > 3 && i == 0)
				{
					nY = -1;
					nX = 0;
				}


				//push the normals back
				this->data.normals.push_back(vec3(nX, nY, nZ));
				this->data.normal_visualization_coordinates.push_back(*(this->data.vertices.end() - 1));
				this->data.normal_visualization_coordinates.push_back(*(this->data.vertices.end() - 1) + vec3(nX, nY, nZ) / NORMAL_LENGTH_DIVISOR);

				//UPDATE 
				//SPECIAL CASE switching to other half of blade
				if (a == 1)
				{
					//update coords where we hit tip of the blade
					startPt.x = startPt.x + getDelta(isInverseX, false);
					startPt.y = startPt.y + getDelta(isInverseY, false);

					//come back from tip of blade back to center
					if (isZero)
						isInverseY = !isInverseY;
					else
						isInverseX = !isInverseX;
				}
				else
				{
					//Update coords per usual
					startPt.x = startPt.x + getDelta(isInverseX, isZero);
					startPt.y = startPt.y + getDelta(isInverseY, !isZero);
					startPt.z = zCoord;
				}
			}
		}

		//change the second z coord for second plane
		if (t == 0 || t == 2 || t == 4)
		{
			zCoord = -0.075f;
		}
		else
		{
			zCoord = 0.075f;
		}
	}

	//Planes



	//INDICIES
	//vertex holder
	int v = 0;

	//FIRST PLANE
	//Make indices now
	for (unsigned int d = 0; d < 4; d++)
	{
		//1st triangle
		this->data.indices.push_back(v);
		this->data.indices.push_back(v + 1);
		this->data.indices.push_back(v + 3);

		//2nd triangle
		this->data.indices.push_back(v + 1);
		this->data.indices.push_back(v + 2);
		this->data.indices.push_back(v + 3);

		//3rd triangle w special case
		this->data.indices.push_back(v + 3);
		if (v != 12)
			this->data.indices.push_back(v + 4);
		else
			this->data.indices.push_back(0);
		this->data.indices.push_back(v);

		//increase the vertex counter by 4, since theres 4 vertices per "plane"
		v = v + 4;
	}

	//inside box triangles
	v = 0;

	//first triangle of the plane
	this->data.indices.push_back(v);
	this->data.indices.push_back(v + 4);
	this->data.indices.push_back(v + 8);

	//second triangle of the plane
	this->data.indices.push_back(v);
	this->data.indices.push_back(v + 8);
	this->data.indices.push_back(v + 12);


	//SECOND PLANE
	//Make indices now
	v = 16;
	for (unsigned int d = 0; d < 4; d++)
	{
		//1st triangle
		this->data.indices.push_back(v);
		this->data.indices.push_back(v + 3);
		this->data.indices.push_back(v + 1);

		//2nd triangle
		this->data.indices.push_back(v + 1);
		this->data.indices.push_back(v + 3);
		this->data.indices.push_back(v + 2);

		//3rd triangle w special case
		this->data.indices.push_back(v + 3);
		this->data.indices.push_back(v);
		if (v != 28)
			this->data.indices.push_back(v + 4);
		else
			this->data.indices.push_back(16);

		//increase the vertex counter by 4, since theres 4 vertices per "plane"
		v = v + 4;
	}

	//inside box triangles
	v = 16;

	//first triangle of the plane
	this->data.indices.push_back(v);
	this->data.indices.push_back(v + 8);
	this->data.indices.push_back(v + 4);

	//second triangle of the plane
	this->data.indices.push_back(v);
	this->data.indices.push_back(v + 12);
	this->data.indices.push_back(v + 8);

	////CONNECTING THE SIDES
	int temp = 0;
	//(these vertices are 32 - 64)
	for (unsigned int c = 32; c < 48; c++)
	{
		if (drawIt(c))
		{
			//first triangle of the plane
			this->data.indices.push_back(c);
			this->data.indices.push_back(c + 16);
			this->data.indices.push_back(c + 17);

			//second triangle of the plane
			this->data.indices.push_back(c);
			this->data.indices.push_back(c + 17);
			this->data.indices.push_back(c + 1);
		}
		else
		{
			temp = c;
			c = c + 32;
			if (temp != 47)
			{//first triangle of the plane
				this->data.indices.push_back(c);
				this->data.indices.push_back(c + 16);
				this->data.indices.push_back(c + 17);

				//second triangle of the plane
				this->data.indices.push_back(c);
				this->data.indices.push_back(c + 17);
				this->data.indices.push_back(c + 1);
			}
			else
			{
				//first triangle of the plane
				this->data.indices.push_back(c);
				this->data.indices.push_back(c + 16);
				this->data.indices.push_back(c + 1);

				//second triangle of the plane
				this->data.indices.push_back(c);
				this->data.indices.push_back(c + 1);
				this->data.indices.push_back(64);
			}
			c = temp;
		}
	}
	//int c = 79;

	//				//first triangle of the plane
	//			this->data.indices.push_back(c);
	//			this->data.indices.push_back(c + 16);
	//			this->data.indices.push_back(c + 1);

	//			//second triangle of the plane
	//			this->data.indices.push_back(c);
	//			this->data.indices.push_back(c + 1);
	//			this->data.indices.push_back(64);

	//make backup because Perry said so. not really though. still would be good to have
	this->data.vbackup = this->data.vertices;
	return true;
}

void Fan::NonGLTakeDown()
{

}

void Fan::RecomputeNormals()
{

}

float Fan::getDelta(bool isInverse, bool isZero)
{
	//return 0 if no movement on that coord
	if (isZero)
		return 0;
	//else give inverse delta or not
	else
	{
		if (isInverse)
			return -delta;
		else
			return delta;
	}
}

float Fan::getNormalX(bool isInverse, bool isZero)
{

	if (isInverse)
		return 1;
	else
		return -1;
}

float Fan::getNormalY(bool isInverse, bool isZero)
{
	if (isZero)
	{
		return 0;
	}
	else
		if (isInverse)
			return -1;
		else
			return 1;
}

bool Fan::drawIt(int check)
{
	if (check == 33)
	{
		return false;
	}
	else if (check == 36)
	{
		return false;
	}
	else if (check == 38)
	{
		return false;
	}
	else if (check == 39)
	{
		return false;
	}
	else if (check == 41)
	{
		return false;
	}
	else if (check == 44)
	{
		return false;
	}
	else if (check == 46)
	{
		return false;
	}
	else if (check == 47)
	{
		return false;
	}
	else
		return true;
}