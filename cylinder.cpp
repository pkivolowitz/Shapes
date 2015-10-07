#include <iostream>
#include <stdexcept>
#include "cylinder.h"

//#define	TRIANGLES

using namespace std;
using namespace glm;

Cylinder::Cylinder(int slices, int stacks, float span) : Shape()
{
	float full_span = pi<float>() * 2.0f;

	if (span == 0)
		throw std::invalid_argument("bad span value");

	if (stacks < 1)
		throw std::invalid_argument("bad stack value");

	this->slices = abs(slices);
	this->stacks = stacks;
	this->span = min<float>(full_span, abs(span));
	this->is_partial_span = this->span != full_span;
}

bool Cylinder::PreGLInitialize()
{
	vec3 z_axis(0.0f, 0.0f, 1.0f);

	// Cylinders are built with radius 1 and 1 unit long from -0.5 to 0.5 along the z axis.
	vec4 p(1.0f, 0.0f, 0.0f, 1.0f);

	// How far to advance between rings. 
	vec3 delta_z(0.0f, 0.0f, 1.0f / float(this->stacks));

	// Rotation angle between slices.
	float theta = this->span / float(this->slices);

	//
	vec3 e(1.0f + 0.02f, 1.0f + 0.02f, 1.0f);

	float scale_factor_for_normals = 1.0f + 1.0f / 6.0f;
	int real_number_of_slices = this->slices + (this->is_partial_span ? 1 : 0);

	this->vertices.reserve(real_number_of_slices * (this->stacks + 1));
	this->normals.reserve(real_number_of_slices * (this->stacks + 1));

	mat4 m = translate(mat4(), vec3(0.0f, 0.0f, -0.5f));

	for (int j = 0; j < this->stacks + 1; j++)
	{
		mat4 m2 = m;

		for (int i = 0; i < real_number_of_slices; i++)
		{
			this->vertices.push_back(vec3(m2 * p));
			this->normals.push_back(*(this->vertices.end() - 1));
			this->colors.push_back(vec4(this->RandomColor(), 1.0f));
			this->normal_visualization_coordinates.push_back(*(this->vertices.end() - 1) * e);
			this->normal_visualization_coordinates.push_back(*(this->vertices.end() - 1) * vec3(scale_factor_for_normals, scale_factor_for_normals, 1.0f));
			m2 = rotate(m2, theta, z_axis);
		}

		m = translate(m, delta_z);
	}


	// There will be two triangles per slice.
	// First is: current outer, current inner, next outer
	// Second is: next outer, current inner, next inner
	for (int j = 0; j < this->stacks; j++)
	{
		for (int i = 0; i < this->slices; i++)
		{
			#ifdef TRIANGLES
			// First
			this->indices.push_back(j * real_number_of_slices + i);
			this->indices.push_back((j + 1) * real_number_of_slices + i);
			this->indices.push_back(j * real_number_of_slices + (i + 1) % real_number_of_slices);
			// Second
			this->indices.push_back(j * real_number_of_slices + (i + 1) % real_number_of_slices);
			this->indices.push_back((j + 1) * real_number_of_slices + i);
			this->indices.push_back((j + 1) * real_number_of_slices + (i + 1) % real_number_of_slices);
			#else
			this->indices.push_back(j * real_number_of_slices + i);
			this->indices.push_back((j + 1) * real_number_of_slices + i);
			this->indices.push_back((j + 1) * real_number_of_slices + (i + 1) % real_number_of_slices);
			this->indices.push_back(j * real_number_of_slices + (i + 1) % real_number_of_slices);
			#endif
		}
	}
	return true;
}

void Cylinder::NonGLTakeDown()
{

}

void Cylinder::Draw(bool draw_normals)
{
	this->GLReturnedError("Disc::Draw() - entering");

	if (this->vertices.size() == 0)
	{
		this->PreGLInitialize();
		this->CommonGLInitialization();
	}

	if (draw_normals)
	{
		glBindVertexArray(this->normal_array_handle);
		glDrawArrays(GL_LINES, 0, this->normal_visualization_coordinates.size());
		glBindVertexArray(0);
	}
	else
	{
		glBindVertexArray(this->vertex_array_handle);
		#ifdef TRIANGLES
		glDrawElements(GL_TRIANGLES , this->indices.size() , GL_UNSIGNED_INT , &this->indices[0]);
		#else
		glDrawElements(GL_QUADS, this->indices.size(), GL_UNSIGNED_INT, &this->indices[0]);
		#endif
		glBindVertexArray(0);
	}
	this->GLReturnedError("Disc::Draw() - exiting");
}