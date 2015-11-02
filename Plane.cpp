#include <iostream>
#include <stdexcept>
#include "Plane.h"


using namespace std;
using namespace glm;

// Width and height are one more than divisionsX and divisionsY.
// Example:
//		divisionsX = divisionsY = 2 yields a two by two grid 
//		with three verticies across and down (for a 9 by 9).
Plane::Plane(int divisionsX, int divisionsY)
{
	if (divisionsX < 1 || divisionsY < 1)
		throw std::invalid_argument("bad divisionX or divisionY value");

	this->divisionsX = divisionsX;
	this->divisionsY = divisionsY;
}

void Cube::AddTextureCoordinates()
{
	this->data.textures.push_back(vec2(0.0f , 0.0f));
	this->data.textures.push_back(vec2(1.0f , 0.0f));
	this->data.textures.push_back(vec2(1.0f , 1.0f));
	this->data.textures.push_back(vec2(0.0f , 1.0f));
}

// PreGLInitialize is used to establish an initial configuration of vertex
// attributes include position, normal, etc.
bool Cube::PreGLInitialize()
{
	// Front
	this->data.vertices.push_back(vec3(-1.0f,  1.0f,  1.0f));
	this->data.vertices.push_back(vec3( 1.0f,  1.0f,  1.0f));
	this->data.vertices.push_back(vec3( 1.0f, -1.0f,  1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f,  1.0f));
	for (int i = 0; i < 4; i++)
	{
		this->data.normals.push_back(vec3(0.0f , 0.0f , 1.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[0 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[0 + i] + this->data.normals[0 + i] / NORMAL_LENGTH_DIVISOR);
	}
	AddTextureCoordinates();
	// right (facing positive x axis)
	this->data.vertices.push_back(vec3(1.0f,  1.0f,  1.0f));
	this->data.vertices.push_back(vec3(1.0f,  1.0f, -1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f,  1.0f));
	for (int i = 0; i < 4; i++)
	{
		this->data.normals.push_back(vec3(1.0f , 0.0f , 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[4 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[4 + i] + this->data.normals[4 + i] / NORMAL_LENGTH_DIVISOR);
	}
	AddTextureCoordinates();
	// back (facing negative z axis)
	this->data.vertices.push_back(vec3( 1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	this->data.vertices.push_back(vec3( 1.0f, -1.0f, -1.0f));
	for (int i = 0; i < 4; i++)
	{
		this->data.normals.push_back(vec3(0.0f , 0.0f , -1.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[8 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[8 + i] + this->data.normals[8 + i] / NORMAL_LENGTH_DIVISOR);
	}
	AddTextureCoordinates();
	// left (facing negative x axis
	this->data.vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, 1.0f,  1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f,-1.0f));
	for (int i = 0; i < 4; i++)
	{
		this->data.normals.push_back(vec3(-1.0f , 0.0f , 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[12 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[12 + i] + this->data.normals[12 + i] / NORMAL_LENGTH_DIVISOR);
	}
	AddTextureCoordinates();
	// top (facing positive y axis)
	this->data.vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3( 1.0f, 1.0f, -1.0f));
	this->data.vertices.push_back(vec3( 1.0f, 1.0f,  1.0f));
	this->data.vertices.push_back(vec3(-1.0f, 1.0f,  1.0f));
	for (int i = 0; i < 4; i++)
	{
		this->data.normals.push_back(vec3(0.0f , 1.0f , 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[16 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[16 + i] + this->data.normals[16 + i] / NORMAL_LENGTH_DIVISOR);
	}
	AddTextureCoordinates();
	// bottom (facing negative y axis)
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
	this->data.vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	this->data.vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	for (int i = 0; i < 4; i++)
	{
		this->data.normals.push_back(vec3(0.0f , -1.0f , 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[20 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[20 + i] + this->data.normals[20 + i] / NORMAL_LENGTH_DIVISOR);
	}
	AddTextureCoordinates();
	
	// Faces complete.

	for (unsigned int i = 0; i < this->data.vertices.size(); i++)
		this->data.indices.push_back(i);

	this->data.vbackup = this->data.vertices;
	return true;
}

// Position vertices and push them onto ->vertices.
// Define colors if we want and push them onto ->colors.
// Define normals - this is easy - they are all the same. Push these on to normals.
// Define all the triangles by specifying indices making up top triangles and bottom triangles.
bool Plane::PreGLInitialize()
{
	float deltaX =  2.0f / float(this->divisionsX);
	float deltaY = -2.0f / float(this->divisionsY);
	vec3 v(-1.0f , 1.0f , 0.0f);

	for (int y = 0; y < this->divisionsY + 1; y++)
	{
		for (int x = 0; x < this->divisionsX + 1; x++)
		{
			// Sometimes it is useful to push the first coordinate out of its regular position
			// so that orientation of the plane can be discerned visually. When the following are
			// uncommented, the first vertex is handled differently to permit this.
			//if (y == 0 && x == 0)
			//	this->data.vertices.push_back(vec3(-2.0f, v.y, v.z));
			//else
			this->data.vertices.push_back(v);
			this->data.textures.push_back(vec2(v.x / 2.0f + 0.5f, v.y / 2.0f + 0.5f));
			this->data.normals.push_back(vec3(0.0f,0.0f,1.0f));
			this->data.colors.push_back(this->RandomColor((this->data.colors.size() > 0 ? *(this->data.colors.end() - 1) : vec4(0.5f , 0.5f , 0.5f , 1.0f)) , -0.2f , 0.2f));
			this->data.normal_visualization_coordinates.push_back(*(this->data.vertices.end() - 1));
			this->data.normal_visualization_coordinates.push_back(*(this->data.vertices.end() - 1) + vec3(0.0f, 0.0f, 1.0f) / NORMAL_LENGTH_DIVISOR);
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
			// "Top" Triangle - This associates this vertex with the one to its right with the one
			// beneath the current vertex. The order defines a clockwise winding.
			this->data.indices.push_back(i);
			this->data.indices.push_back(i + 1);
			this->data.indices.push_back(i + divisionsX + 1);
			// Bottom triangle - This associates the vertex to our right with the one below it and
			// the one below the current vertex. This order defines a clockwise winding.
			this->data.indices.push_back(i + 1);
			this->data.indices.push_back(i + divisionsX + 2);
			this->data.indices.push_back(i + divisionsX + 1);
			i++;
		}
		i++;
	}
	this->data.vbackup = this->data.vertices;
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
	vec3 sum;
	vec3 a;
	vec3 b;
	int i = 0;
	int w = divisionsX + 1;
	vector<vec3> & v = data.vertices;
	vector<vec3> & n = data.normals;
	vector<vec3> & p = data.normal_visualization_coordinates;

	// int i indexes the consecutive vertex indices. The row and col values are useful
	// to maintain a logical picture of the vertex index i represents. The approach here
	// is to examine every triangle in which vertex i participates. Average together all
	// the cross products (after normalizing both the input and the output) taken in 
	// clockwise order. We later determined that it should have been counter clockwise
	// order. Rather than changing all the calls to cross(), I negated the assignments
	// to data.normals (abbreviated by n).
	for (int row = 0; row <= this->divisionsY; row++)
	{
		for (int col = 0; col < w; col++, i++)
		{
			sum = vec3(0.0f);
			if (col == 0)
			{
				if (row == divisionsY)
				{
					// This is the left edge and bottom row
					a = normalize(v[i - w] - v[i]);
					b = normalize(v[i - w + 1] - v[i]);
					sum += normalize(cross(a , b));
					// Here and where possible below, the assignments of b to a recognizes that
					// we're examining triangles in a fixed order. Moving from one to the next
					// in order means we do not have to repeat one of the normalizations because
					// the line segment is shared between two adjacent triangles.
					a = b;
					b = normalize(v[i + 1] - v[i]);
					sum += normalize(cross(a , b));
					if (col == 0)
					{
						// This is the bottom row and left edge. We're done.
						n[i] = -sum / 2.0f;
					}
					else
					{
						// This is the bottom row and NOT the left edge. One more triangle to do.
						a = normalize(v[i - 1] - v[i]);
						b = normalize(v[i - w] - v[i]);
						sum += normalize(cross(a , b));
						n[i] = -sum / 3.0f;
					}
				}
				else
				{
					// This is the left edge but NOT the bottom row
					a = normalize(v[i + 1] - v[i]);
					b = normalize(v[i + w] - v[i]);
					sum += normalize(cross(a , b));
					if (row == 0)
					{
						// This is the top left corner
						n[i] = -sum;
					}
					else
					{
						// This is a left edge lower down than the top left corner
						a = normalize(v[i - w] - v[i]);
						b = normalize(v[i - w + 1] - v[i]);
						sum += normalize(cross(a , b));
						a = b;
						b = normalize(v[i + 1] - v[i]);
						sum += normalize(cross(a , b));
						// The triangle to the right and lower is already done.
						n[i] = -sum / 3.0f;
					}
				}
			}
			else if (col == w - 1)
			{
				// This is the right edge
				if (row < divisionsY)
				{
					// This is not the bottom row.
					a = normalize(v[i + w] - v[i]);
					b = normalize(v[i + w - 1] - v[i]);
					sum += normalize(cross(a , b));
					a = b;
					b = normalize(v[i - 1] - v[i]);
					sum += normalize(cross(a , b));
					if (row == 0)
					{
						// This is the upper right corner
						n[i] = -sum / 2.0f;
					}
					else
					{
						// The is the right edge in the middle.
						a = b;
						b = normalize(v[i - w] - v[i]);
						sum += normalize(cross(a , b));
						n[i] = -sum / 3.0f;
					}
				}
				else
				{
					// This is the lower right corner
					a = normalize(v[i - 1] - v[i]);
					b = normalize(v[i - w] - v[i]);
					n[i] = -normalize(cross(a , b));
				}
			}
			else
			{
				// This is in the middle horizontally - now consider the vertical
				if (row < divisionsY)
				{
					a = normalize(v[i + 1] - v[i]);
					b = normalize(v[i + w] - v[i]);
					sum += normalize(cross(a , b));
					a = b;
					b = normalize(v[i + w - 1] - v[i]);
					sum += normalize(cross(a , b));
					a = b;
					b = normalize(v[i - 1] - v[i]);
					sum += normalize(cross(a , b));
				}
				if (row == 0)
				{
					// We're in the middle horizontally and on the top row
					n[i] = -sum / 3.0f;
				}
				else
				{
					// We are in the middle in both dimensions. The calculations
					// below us are already done. Just do above us.
					a = normalize(v[i - 1] - v[i]);
					b = normalize(v[i - w] - v[i]);
					sum += normalize(cross(a , b));
					a = b;
					b = normalize(v[i - w + 1] - v[i]);
					sum += normalize(cross(a , b));
					a = b;
					b = normalize(v[i + 1] - v[i]);
					sum += normalize(cross(a , b));
					n[i] = -sum / (row == divisionsY ? 3.0f: 6.0f);
				}
			}
			// Attempt to update the normal visualization coordinates.
			// As we leave processing of the current vertex, we take the opportunity to
			// update the visualization vectors. The beginning of each visualization vector
			// is the position of the vertex itself. The other end of each visualization 
			// vector is the newly calculated normal modulated by a constant divisor added
			// to the position of the vertex.
			p[i * 2] = v[i];
			p[i * 2 + 1] = v[i] + n[i] / NORMAL_LENGTH_DIVISOR;
		}
	}
}

void Cube::RecomputeNormals()
{
	const int NCORNERS = 4;
	vector<vec3> & v = data.vertices;
	vector<vec3> & n = data.normals;
	vector<vec3> & p = data.normal_visualization_coordinates;
	vec3 a , b;

	int j = 0;
	// A cube is six faces of four vertices each. face_counter iterates
	// face indexes. As each face is comprised of four vertices, face_counter
	// * 4 is the base index of the 4 vertices in the face. These are i through
	// i + 3.
	// At each vertex, the line segment ahead is a where as the line segment behind
	// is b. cross(a,b) is actually backwards so the negation of the value is used
	// as the new normal.
	// Every vertex has two associated vertices in the normal visualization array.
	// These are indexed by j, allowed to post increment its way up.
	for (size_t face_counter = 0; face_counter < n.size() / NCORNERS; face_counter++)
	{
		int i = face_counter * NCORNERS;
		a = normalize(v[i + 1] - v[i + 0]);
		b = normalize(v[i + 3] - v[i + 0]);
		n[i + 0] = -normalize(cross(a , b));
		p[j++] = v[i + 0];
		p[j++] = v[i + 0] + n[i + 0] / NORMAL_LENGTH_DIVISOR;
		a = normalize(v[i + 2] - v[i + 1]);
		b = normalize(v[i + 0] - v[i + 1]);
		n[i + 1] = -normalize(cross(a , b));
		p[j++] = v[i + 1];
		p[j++] = v[i + 1] + n[i + 1] / NORMAL_LENGTH_DIVISOR;
		a = normalize(v[i + 3] - v[i + 2]);
		b = normalize(v[i + 1] - v[i + 2]);
		n[i + 2] = -normalize(cross(a , b));
		p[j++] = v[i + 2];
		p[j++] = v[i + 2] + n[i + 2] / NORMAL_LENGTH_DIVISOR;
		a = normalize(v[i + 0] - v[i + 3]);
		b = normalize(v[i + 2] - v[i + 3]);
		n[i + 3] = -normalize(cross(a , b));
		p[j++] = v[i + 3];
		p[j++] = v[i + 3] + n[i + 3] / NORMAL_LENGTH_DIVISOR;
	}
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
