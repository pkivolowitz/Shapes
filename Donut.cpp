#include <iostream>
#include <stdexcept>
#include "Donut.h"
#include "plane.h"

using namespace std;
using namespace glm;

Donut::Donut(int slices, int stacks, float span, float back_radius, float front_radius) : Plane(slices, stacks)
{
	float full_span = pi<float>() * 2.0f;

	if (span == 0)
		throw std::invalid_argument("bad span value");

	if (stacks < 1)
		throw std::invalid_argument("bad stack value");

	if (back_radius == 0.0f && front_radius == 0.0f)
		throw std::invalid_argument("bad radii");

	if (abs(back_radius) <= 0.00001f)
		back_radius = 0.00001f;

	if (abs(front_radius) <= 0.00001f)
		front_radius = 0.00001f;

	this->slices = abs(slices);
	this->stacks = stacks;
	this->fr = front_radius;
	this->br = back_radius;
	this->span = min<float>(full_span, abs(span));
	this->is_partial_span = this->span != full_span;
}

bool Donut::PreGLInitialize()
{
	// Use the Plane implementation for making a flat plane.
	this->Plane::PreGLInitialize();

	// Now move the vertices into the shape of a cylinder.
	/*float derivative;
	vec3 z_axis(0.0f, 0.0f, 1.0f);
	vec4 p(this->br, 0.0f, 0.0f, 1.0f);*/

	// The t coordinate marches from -0.5 to 0.5 along z.
	/*float t = 1.0f;
	float delta_t = 1.0f / float(this->stacks);
	vec3 delta_z(0.0f, 0.0f, delta_t);*/

	// How far to advance between rings. 
	// Rotation angle between slices.
	float theta = this->span / float(this->slices);
	mat4 m = translate(mat4(), vec3(0.0f, 0.0f, -0.5f));

	vec3 * v = &this->data.vertices[0];
	vec3 * vn = &this->data.normal_visualization_coordinates[0];

	//derivative = fr - br;

	//for (int ring_index = 0; ring_index < this->stacks + 1; ring_index++)
	//{
	//	// Mix from back radius to front radius to derive
	//	// the x coordinate of the point used to project the
	//	// surface.
	//	p.x = mix(br, fr, t);
	//	// Given the slope from back to front, compute normal angle.
	//	float rho = atan(derivative);
	//	//cout << rho / pi<float>() * 180.0f << endl;
	//	vec4 rotated_y_axis = rotate(mat4(), -rho, vec3(0.0f, 1.0f, 0.0f)) * vec4(1.0f, 0.0f, 0.0f, 1.0f);

	//	t -= delta_t;

	//	// Reset m2 to nothing but back to front translation.
	//	mat4 m2 = m;
	//	mat4 m3;

	//	for (int slice_index = 0; slice_index <= this->slices; slice_index++)
	//	{
	//		vec3 n = vec3(m3 * rotated_y_axis);
	//		*(vn++) = *v = vec3(m2 * p);
	//		*(vn++) = *v + n / this->NORMAL_LENGTH_DIVISOR;
	//		m2 = rotate(m2, -theta, z_axis);
	//		m3 = rotate(m3, -theta, z_axis);
	//		v++;
	//	}

	//	m = translate(m, delta_z);
	//}
	//this->data.vbackup = this->data.vertices;

	//Donut Code
	vector <vec3> singleCircle;
	vec4 P(fr, 0.0f, 0.0f, 1.0f);
	mat4 M;

	for (int i = 0; i <= stacks; i++)
	{
		singleCircle.push_back(vec3(M * P));
		M = rotate(M, theta, vec3(0.0f, 0.0f, 1.0f));
	}

	mat4 T = translate(mat4(), vec3(br, 0.0, 0.0));
	mat4 R;
	vec3 * V = &data.vertices[0];

	for (int i = 0; i <= this->slices; i++)
	{
		for (size_t j = 0; j < singleCircle.size(); j++)
		{
			*(V++) = vec3(R * T * vec4(singleCircle[j], 1.0f));
		}

		R = rotate(R, theta, vec3(0.0f, 1.0f, 0.0f));
	}

	// Finally, rely upon the ultra cool RecomputeNormals from Plane to do all the hard work
	// for initially setting the normals.
	this->data.vbackup = this->data.vertices;

	this->RecomputeNormals();
	return true;
}

void Donut::NonGLTakeDown()
{
}

void Donut::RecomputeNormals()
{
	this->Plane::RecomputeNormals();

	vector<vec3> & n = this->data.normals;
	vector<vec3> & v = this->data.vertices;
	vector<vec3> & vn = this->data.normal_visualization_coordinates;
	int w = this->slices + 1;

	if (!this->is_partial_span)
	{
		// Fix the normals of the first and last columns;
		for (int ring_index = 0; ring_index < this->stacks + 1; ring_index++)
		{
			n[ring_index * w + this->slices] = n[ring_index * w + 0] = (n[ring_index * w + 0] + n[ring_index * w + this->slices]) / 2.0f;

			vn[(ring_index * w + this->slices) * 2 + 0] = v[ring_index * w + this->slices];
			vn[(ring_index * w + this->slices) * 2 + 1] = v[ring_index * w + this->slices] + n[ring_index * w + this->slices] / Shape::NORMAL_LENGTH_DIVISOR;

			vn[(ring_index * w + 0) * 2 + 0] = v[ring_index * w + 0];
			vn[(ring_index * w + 0) * 2 + 1] = v[ring_index * w + 0] + n[ring_index * w + 0] / Shape::NORMAL_LENGTH_DIVISOR;
		}

		// Fix normals of first and last row.
		int h = this->stacks + 1;

		for (int slice_index = 0; slice_index < this->slices + 1; slice_index++)
		{
			n[slice_index + w * (h - 1)] = n[slice_index] = (n[slice_index] + n[slice_index + w * (h-1)]) / 2.0f;

			// Bottom
			vn[slice_index * 2 + 0] = v[slice_index];
			vn[slice_index * 2 + 1] = v[slice_index] + n[slice_index] / Shape::NORMAL_LENGTH_DIVISOR;

			// Top
			vn[(slice_index + w * (h - 1)) * 2 + 0] = v[(slice_index + w * (h - 1))];
			vn[(slice_index + w * (h - 1)) * 2 + 1] = v[(slice_index + w * (h - 1))] + n[slice_index] / Shape::NORMAL_LENGTH_DIVISOR;
		}
	}
}