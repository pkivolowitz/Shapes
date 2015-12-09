#pragma once
#include "shape.h"
#include <vector>
using std::vector;
using glm::vec3;

class Dodecahedron : public Shape
{
public:
	Dodecahedron();
protected:
	const int NUM_FACES = 12;
	const int VERTICES_PER_FACE = 5;

	//constant used for calculating some of the points
	const double D = ((1.0 + sqrt(5.0)) / 2.0);

	vector<vec3> v; //all the vertices
	//all the vertices for each face
	vector<vec3> face0;
	vector<vec3> face1;
	vector<vec3> face2;
	vector<vec3> face3;
	vector<vec3> face4;
	vector<vec3> face5;
	vector<vec3> face6;
	vector<vec3> face7;
	vector<vec3> face8;
	vector<vec3> face9;
	vector<vec3> face10;
	vector<vec3> face11;
	vector< vector<vec3> > faces; //vector of all the faces
	struct DODEC
	{
		vec3 position;
		glm::vec2 tc; //texture coordinates
	};
	void AddTextureCoordinates();
	bool PreGLInitialize();
	void NonGLTakeDown();
	void RecomputeNormals();
};