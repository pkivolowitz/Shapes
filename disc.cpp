#include <iostream>
#include <stdexcept>
#include "disc.h"

using namespace std;
using namespace glm;

Disc::Disc(int slices, float span, float outer_radius, float inner_radius) : Shape()
{
	float full_span = pi<float>() * 2.0f;

	if (span == 0)
		throw std::invalid_argument("bad span value");
	
	this->slices = abs(slices);
	this->span = min<float>(full_span, abs(span));
	this->inner_radius = inner_radius;
	this->outer_radius = outer_radius;

	this->is_partial_span = this->span != full_span;
}

bool Disc::PreGLInitialize()
{
	float scale_factor_for_normals = 1.0f / 10.0f;
	int real_number_of_slices = this->slices + (this->is_partial_span ? 1 : 0);

	// If the inner radius is greater than zero, two rings of vertices will be created.
	// The inner and outer ring will be connected via two triangles per slice.
	//
	// If the inner radius is zero, a triangle fan will be created.

	this->vertices.reserve(real_number_of_slices + (this->inner_radius == 0.0f ? 1 : real_number_of_slices));
	this->normals.reserve(real_number_of_slices + (this->inner_radius == 0.0f ? 1 : real_number_of_slices));

	mat4 m;
	vec4 p(this->outer_radius, 0.0f, 0.0f, 1.0f);
	vec3 n(0.0f, 0.0f, 1.0f);
	vec3 z(0.0f, 0.0f, 1.0f);

	float theta = this->span / float(this->slices);

	// An outer ring is requires in all cases.
	for (int i = 0; i < real_number_of_slices; i++)
	{
		this->vertices.push_back(vec3(m * p));
		this->normals.push_back(n);
		this->normal_visualization_coordinates.push_back(vec3(m * p) + vec3(0.0f, 0.0f, 0.001f));
		this->normal_visualization_coordinates.push_back(vec3(m * p) + n * scale_factor_for_normals);
		m = rotate(m, theta, z);
	}

	if (this->inner_radius == 0.0)
	{
		// Only the center needs to be added.
		this->vertices.push_back(vec3(0.0f, 0.0f, 0.0f));
		this->normals.push_back(n);
		this->normal_visualization_coordinates.push_back(vec3(0.0f, 0.0f, epsilon<float>()));
		this->normal_visualization_coordinates.push_back(n * scale_factor_for_normals);

		// There will be one triangle per slice. The vertices
		// are defined as current outer, center, next outer.
		for (int i = 0; i < real_number_of_slices - 1; i++)
		{
			this->indices.push_back(i);
			this->indices.push_back(this->vertices.size() - 1);
			this->indices.push_back((i + 1) % real_number_of_slices);
		}
	}
	else
	{
		m = mat4();
		p = vec4(this->inner_radius, 0.0f, 0.0f, 1.0f);

		// Instanciate the second ring.
		for (int i = 0; i < real_number_of_slices; i++)
		{
			this->vertices.push_back(vec3(m * p));
			this->normals.push_back(n);
			this->normal_visualization_coordinates.push_back(vec3(m * p) + vec3(0.0f, 0.0f, 0.001f));
			this->normal_visualization_coordinates.push_back(vec3(m * p) + n * scale_factor_for_normals);
			m = rotate(m, theta, z);
		}

		// There will be two triangles per slice.
		// First is: current outer, current inner, next outer
		// Second is: next outer, current inner, next inner
		for (int i = 0; i < this->slices; i++)
		{
			// First
			this->indices.push_back(i);
			this->indices.push_back(real_number_of_slices + i);
			this->indices.push_back((i + 1) % real_number_of_slices);
			// Second
			this->indices.push_back((i + 1) % real_number_of_slices);
			this->indices.push_back(real_number_of_slices + i);
			this->indices.push_back(real_number_of_slices + (i + 1) % real_number_of_slices);
		}
	}
	return true;
}

void Disc::NonGLTakeDown()
{

}

void Disc::Draw(bool draw_normals)
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
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, &this->indices[0]);
		glBindVertexArray(0);
	}
	this->GLReturnedError("Disc::Draw() - exiting");
}