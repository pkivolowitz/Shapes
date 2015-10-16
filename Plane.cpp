#include <iostream>
#include <stdexcept>
#include "Plane.h"


using namespace std;
using namespace glm;

Plane::Plane(int divisionsX, int divisionsY)
{
	if (divisionsX < 1 || divisionsY < 1)
		throw std::invalid_argument("bad divisionX or divisionY value");

	this->divisionsX = divisionsX;
	this->divisionsY = divisionsY;
}

bool Cube::PreGLInitialize()
{
	// NOTE - this is somewhat untested.

	// Front
	this->data.vertices.push_back(vec3(-1.0f,  1.0f,  1.0f));
	this->data.vertices.push_back(vec3( 1.0f,  1.0f,  1.0f));
	this->data.vertices.push_back(vec3( 1.0f, -1.0f,  1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f,  1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f, 1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f, 1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f, 1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f, 1.0f));
	// right (facing positive x axis)
	this->data.vertices.push_back(vec3(1.0f,  1.0f,  1.0f));
	this->data.vertices.push_back(vec3(1.0f,  1.0f, -1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f,  1.0f));
	this->data.normals.push_back(vec3(1.0f, 0.0f, 0.0f));
	this->data.normals.push_back(vec3(1.0f, 0.0f, 0.0f));
	this->data.normals.push_back(vec3(1.0f, 0.0f, 0.0f));
	this->data.normals.push_back(vec3(1.0f, 0.0f, 0.0f));
	// back (facing negative z axis)
	this->data.vertices.push_back(vec3( 1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	this->data.vertices.push_back(vec3( 1.0f, -1.0f, -1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f,-1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f,-1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f,-1.0f));
	this->data.normals.push_back(vec3(0.0f, 0.0f,-1.0f));
	// left (facing negative x axis
	this->data.vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, 1.0f,  1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f,-1.0f));
	this->data.normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
	this->data.normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
	this->data.normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
	this->data.normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
	// top (facing positive y axis)
	this->data.vertices.push_back(vec3(-1.0f, 1.0f,   1.0f));
	this->data.vertices.push_back(vec3( 1.0f, 1.0f,   1.0f));
	this->data.vertices.push_back(vec3( 1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	this->data.normals.push_back(vec3(0.0f, 1.0f, 0.0f));
	this->data.normals.push_back(vec3(0.0f, 1.0f, 0.0f));
	this->data.normals.push_back(vec3(0.0f, 1.0f, 0.0f));
	this->data.normals.push_back(vec3(0.0f, 1.0f, 0.0f));
	// bottom (facing negative y axis)
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	this->data.normals.push_back(vec3(0.0f, -1.0f, 0.0f));
	this->data.normals.push_back(vec3(0.0f, -1.0f, 0.0f));
	this->data.normals.push_back(vec3(0.0f, -1.0f, 0.0f));
	this->data.normals.push_back(vec3(0.0f, -1.0f, 0.0f));

	for (unsigned int i = 0; i < this->data.vertices.size(); i++)
		this->data.indices.push_back(i);
	return true;
}

bool Plane::PreGLInitialize()
{
	// Position vertices and push them onto ->vertices.
	// Define colors if we want and push them onto ->colors.
	// Define normals - this is easy - they are all the same. Push these on to normals.
	// Define all the triangles by specifying indices making up top triangles and bottom triangles.
	float deltaX =  2.0f / float(this->divisionsX);
	float deltaY = -2.0f / float(this->divisionsY);
	vec3 v(-1.0f , 1.0f , 0.0f);

	for (int y = 0; y < this->divisionsY + 1; y++)
	{
		for (int x = 0; x < this->divisionsX + 1; x++)
		{
			//if (y == 0 && x == 0)
			//	this->data.vertices.push_back(vec3(-2.0f, v.y, v.z));
			//else
				this->data.vertices.push_back(v);
			this->data.normals.push_back(vec3(0.0f,0.0f,1.0f));
			this->data.colors.push_back(this->RandomColor((this->data.colors.size() > 0 ? *(this->data.colors.end() - 1) : vec4(0.5f , 0.5f , 0.5f , 1.0f)) , -0.2f , 0.2f));
			this->data.normal_visualization_coordinates.push_back(*(this->data.vertices.end() - 1));
			this->data.normal_visualization_coordinates.push_back(*(this->data.vertices.end() - 1) + vec3(0.0f, 0.0f, 1.0f) / 8.0f);
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
			// "Top" Triangle
			this->data.indices.push_back(i);
			this->data.indices.push_back(i + 1);
			this->data.indices.push_back(i + divisionsX + 1);
			// Bottom triangle
			this->data.indices.push_back(i + 1);
			this->data.indices.push_back(i + divisionsX + 2);
			this->data.indices.push_back(i + divisionsX + 1);
			i++;
		}
		i++;
	}
	return true;
}

void Plane::NonGLTakeDown()
{
}

void Cube::NonGLTakeDown()
{
}

void Plane::RecomputeNormals()
{
}

void Cube::RecomputeNormals()
{
}

void Cube::Draw(bool draw_normals)
{
	if (this->data.vertices.size() == 0)
	{
		this->PreGLInitialize();
		this->CommonGLInitialization();
	}
	if (draw_normals && this->data.normal_visualization_coordinates.size() > 0)
	{
		glBindVertexArray(this->normal_array_handle);
		glDrawArrays(GL_LINES, 0, this->data.normal_visualization_coordinates.size());
	}
	else
	{
		glBindVertexArray(this->vertex_array_handle);
		glDrawElements(GL_QUADS, this->data.indices.size(), GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);
	this->GLReturnedError("Cube::Draw() - exiting");
}

void Plane::Draw(bool draw_normals)
{
	if (this->data.vertices.size() == 0)
	{
		this->PreGLInitialize();
		this->CommonGLInitialization();
	}

	if (draw_normals && this->data.normal_visualization_coordinates.size() > 0)
	{
		glBindVertexArray(this->normal_array_handle);
		glDrawArrays(GL_LINES , 0 , this->data.normal_visualization_coordinates.size());
	}
	else
	{
		glBindVertexArray(this->vertex_array_handle);
		glDrawElements(GL_TRIANGLES , this->data.indices.size() , GL_UNSIGNED_INT , nullptr);
	}
	glBindVertexArray(0);
	this->GLReturnedError("Plane::Draw() - exiting");
}