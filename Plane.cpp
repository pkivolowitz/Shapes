#include <iostream>
#include <stdexcept>
#include "Plane.h"


using namespace std;
using namespace glm;

bool Plane::PreGLInitialize()
{
	// Position vertices and push them onto ->vertices.
	// Define colors if we want and push them onto ->colors.
	// Define normals - this is easy - they are all the same. Push these on to normals.
	// Define all the triangles by specifying indices making up top triangles and bottom triangles.
	float deltaX = 2.0f/float(this->divisionsX);
	float deltaY = -2.0f / float(this->divisionsY);
	vec3 v(-1.0f , 1.0f , 0.0f);

	for (int y = 0; y < this->divisionsY; y++)
	{
		for (int x = 0; x < this->divisionsX; x++)
		{
			this->vertices.push_back(v);
			this->normals.push_back(vec3(0.0f,0.0f,1.0f));
			v.x = v.x + deltaX;
		}
		v.x = -1.0f;
		v.y = v.y + deltaY;
	}

	int i = 0;
	for (int y = 0; y < this->divisionsY; y++)
	{
		for (int x = 0; x < this->divisionsX; x++)
		{
			this->indices.push_back(i);
			this->indices.push_back(i + 1);
			this->indices.push_back(i + divisionsX + 1);
			i++;
		}

		i++;

	}
}

void Plane::NonGLTakeDown()
{

}

void Plane::Draw(bool draw_normals)
{
	if (this->vertices.size() == 0)
	{
		this->PreGLInitialize();
		this->CommonGLInitialization();
	}

	if (draw_normals && this->normal_visualization_coordinates.size() > 0)
	{
		glBindVertexArray(this->normal_array_handle);
		glDrawArrays(GL_LINES , 0 , this->normal_visualization_coordinates.size());
	}
	else
	{
		glBindVertexArray(this->vertex_array_handle);
		glDrawElements(GL_TRIANGLES , this->indices.size() , GL_UNSIGNED_INT , nullptr);
		glDrawElements(GL_QUADS , this->indices.size() , GL_UNSIGNED_INT , nullptr);
	}
	glBindVertexArray(0);
	this->GLReturnedError("Plane::Draw() - exiting");
}