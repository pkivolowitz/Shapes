#include <iostream>
#include <stdexcept>
#include <vector>
#include "Star.h"

using namespace std;
using namespace glm;

Star::Star(float dimension) : Shape()
{
	if (dimension == 0.0f)
		throw std::invalid_argument("bad dimension");

	this->d = dimension;
}

bool Star::PreGLInitialize()
{
	InitializeStar();

	for (unsigned int i = 0; i < this->data.vertices.size(); i++)
		this->data.indices.push_back(i);

	this->data.vbackup = this->data.vertices;
	return true;
}

void Star::AddTextureCoordinates()
{
	//This sets up the texture cordinates for each triangle on a single face
	//Triangle One
	this->data.textures.push_back(vec2(0.0f, 0.0f));
	this->data.textures.push_back(vec2(0.5f, 0.5f));
	this->data.textures.push_back(vec2(1.0f, 0.0f));

	//Triangle Two
	this->data.textures.push_back(vec2(1.0f, 0.0f));
	this->data.textures.push_back(vec2(0.5f, 0.5f));
	this->data.textures.push_back(vec2(1.0f, 1.0f));

	//Triangle Three
	this->data.textures.push_back(vec2(1.0f, 1.0f));
	this->data.textures.push_back(vec2(0.5f, 0.5f));
	this->data.textures.push_back(vec2(0.0f, 1.0f));

	//Triangle four
	this->data.textures.push_back(vec2(0.0f, 1.0f));
	this->data.textures.push_back(vec2(0.5f, 0.5f));
	this->data.textures.push_back(vec2(0.0f, 0.0f));
}

void Star::StarFace(vec3 point0, vec3 point1, vec3 point2, vec3 point3, vec3 point4)
{
	//uses the passed in points to push back the vertices that form each triangle on a single face
	//Triangle One
	this->data.vertices.push_back(point0);
	this->data.vertices.push_back(point2);
	this->data.vertices.push_back(point1);

	//Triangle Two
	this->data.vertices.push_back(point0);
	this->data.vertices.push_back(point3);
	this->data.vertices.push_back(point2);

	//Triangle Three
	this->data.vertices.push_back(point0);
	this->data.vertices.push_back(point4);
	this->data.vertices.push_back(point3);

	//Triangle Four
	this->data.vertices.push_back(point0);
	this->data.vertices.push_back(point1);
	this->data.vertices.push_back(point4);
}

void Star::InitializeStar()
{
	const int ITERATIONS = 12;
	const double LENGTH_OF_POINT = this->d * 6.0;
	//Pass in 5 points for each face to form the 12 vertices that make up all the triangles
	//Then set up the texture coordinates for each face
	//finally push back room in the normals vector and visualization vector to calculate normals later
	//Front Face
	StarFace(vec3(0, 0, LENGTH_OF_POINT), vec3(-this->d, this->d, this->d), vec3(-this->d, -this->d, this->d), vec3(this->d, -this->d, this->d), vec3(this->d, this->d, this->d));
	AddTextureCoordinates();
	for (int i = 0; i < ITERATIONS; i++)
	{
		this->data.normals.push_back(vec3(0.0f, 0.0f, 1.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[0 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[0 + i] + this->data.normals[0 + i] / NORMAL_LENGTH_DIVISOR);
	}
	//Right Face
	StarFace(vec3(LENGTH_OF_POINT, 0, 0), vec3(this->d, this->d, this->d), vec3(this->d, -this->d, this->d), vec3(this->d, -this->d, -this->d), vec3(this->d, this->d, -this->d));
	AddTextureCoordinates();
	for (int i = 0; i < ITERATIONS; i++)
	{
		this->data.normals.push_back(vec3(1.0f, 0.0f, 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[12 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[12 + i] + this->data.normals[12 + i] / NORMAL_LENGTH_DIVISOR);
	}
	//Back Face
	StarFace(vec3(0, 0, -LENGTH_OF_POINT), vec3(this->d, this->d, -this->d), vec3(this->d, -this->d, -this->d), vec3(-this->d, -this->d, -this->d), vec3(-this->d, this->d, -this->d));
	AddTextureCoordinates();
	for (int i = 0; i < ITERATIONS; i++)
	{
		this->data.normals.push_back(vec3(0.0f, 0.0f, -1.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[24 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[24 + i] + this->data.normals[24 + i] / NORMAL_LENGTH_DIVISOR);
	}
	//Left Face
	StarFace(vec3(-LENGTH_OF_POINT, 0, 0), vec3(-this->d, this->d, -this->d), vec3(-this->d, -this->d, -this->d), vec3(-this->d, -this->d, this->d), vec3(-this->d, this->d, this->d));
	AddTextureCoordinates();
	for (int i = 0; i < ITERATIONS; i++)
	{
		this->data.normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[36 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[36 + i] + this->data.normals[36 + i] / NORMAL_LENGTH_DIVISOR);
	}
	//Top Face
	StarFace(vec3(0, LENGTH_OF_POINT, 0), vec3(-this->d, this->d, -this->d), vec3(-this->d, this->d, this->d), vec3(this->d, this->d, this->d), vec3(this->d, this->d, -this->d));
	AddTextureCoordinates();
	for (int i = 0; i < ITERATIONS; i++)
	{
		this->data.normals.push_back(vec3(0.0f, 1.0f, 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[48 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[48 + i] + this->data.normals[48 + i] / NORMAL_LENGTH_DIVISOR);
	}
	//Bottom Face
	StarFace(vec3(0, -LENGTH_OF_POINT, 0), vec3(-this->d, -this->d, this->d), vec3(-this->d, -this->d, -this->d), vec3(this->d, -this->d, -this->d), vec3(this->d, -this->d, this->d));
	AddTextureCoordinates();
	for (int i = 0; i < ITERATIONS; i++)
	{
		this->data.normals.push_back(vec3(0.0f, -1.0f, 0.0f));
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[60 + i]);
		this->data.normal_visualization_coordinates.push_back(this->data.vertices[60 + i] + this->data.normals[60 + i] / NORMAL_LENGTH_DIVISOR);
	}
	//Compute the normals the first time
	RecomputeNormals();
}

void Star::NonGLTakeDown()
{

}

void Star::RecomputeNormals()
{
	const int NVERTICES = 12;
	vector<vec3> & v = data.vertices;
	vector<vec3> & n = data.normals;
	vector<vec3> & p = data.normal_visualization_coordinates;

	int posInVisVector = 0;

	for (size_t face_counter = 0; face_counter < n.size() / NVERTICES; face_counter++)
	{
		//i determines the beginning vertices of each face
		int i = face_counter * NVERTICES;
		//computes the normals and visualizations for each triangle on the current face
		//0 - 11 corresponds with the index number of each vertex in the context of one face
		RecomputeNormalsSingleTriangle(v, n, p, 0, 1, 2, i, posInVisVector);
		RecomputeNormalsSingleTriangle(v, n, p, 3, 4, 5, i, posInVisVector);
		RecomputeNormalsSingleTriangle(v, n, p, 6, 7, 8, i, posInVisVector);
		RecomputeNormalsSingleTriangle(v, n, p, 9, 10, 11, i, posInVisVector);
	}
}

void Star::RecomputeNormalsSingleTriangle(vector<vec3>& v, vector<vec3>& n, vector<vec3>& p, int indexOfVertexOne, int indexOfVertexTwo, int indexOfVertexThree, int i, int& posInVisVector)
{
	vec3 a = normalize(v[i + indexOfVertexTwo] - v[i + indexOfVertexOne]);
	vec3 b = normalize(v[i + indexOfVertexThree] - v[i + indexOfVertexOne]);
	n[i + indexOfVertexTwo] = n[i + indexOfVertexThree] = n[i + indexOfVertexOne] = normalize(cross(b, a));
	p[posInVisVector++] = v[i + indexOfVertexOne];
	p[posInVisVector++] = v[i + indexOfVertexOne] + n[i + indexOfVertexOne] / NORMAL_LENGTH_DIVISOR;

	p[posInVisVector++] = v[i + indexOfVertexTwo];
	p[posInVisVector++] = v[i + indexOfVertexTwo] + n[i + indexOfVertexOne] / NORMAL_LENGTH_DIVISOR;

	p[posInVisVector++] = v[i + indexOfVertexThree];
	p[posInVisVector++] = v[i + indexOfVertexThree] + n[i + indexOfVertexOne] / NORMAL_LENGTH_DIVISOR;
}