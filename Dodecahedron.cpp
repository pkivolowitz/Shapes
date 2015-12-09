#include "Dodecahedron.h"
using namespace glm;

//Constructor
Dodecahedron::Dodecahedron()
{
	//fill all the vectors of vertices

	//all vertices
	v = {
		{ 0.2, 0.2, 0.2 },               // 00
		{ 0.2, 0.2, -0.2 },              // 01
		{ 0.2, -0.2, 0.2 },              // 02
		{ -0.2, 0.2, 0.2 },              // 03
		{ -0.2, -0.2, 0.2 },             // 04
		{ -0.2, 0.2, -0.2 },             // 05
		{ 0.2, -0.2, -0.2 },             // 06
		{ -0.2, -0.2, -0.2 },            // 07
		{ 0.0, 0.2 / D, 0.2 * D },       // 08
		{ 0.0, -0.2 / D, 0.2 * D },      // 09
		{ 0.0, 0.2 / D, -0.2 * D },      // 10
		{ 0.0, -0.2 / D, -0.2 * D },		 // 11
		{ 0.2 / D, 0.2*D, 0 },			 // 12
		{ -0.2 / D, 0.2*D, 0 },			 // 13
		{ 0.2 / D, -0.2*D, 0 },			 // 14
		{ -0.2 / D, -0.2*D, 0 },			 // 15
		{ 0.2*D, 0, 0.2 / D },			 // 16
		{ -0.2*D, 0, 0.2 / D },			 // 17
		{ 0.2*D, 0, -0.2 / D },			 // 18
		{ -0.2*D, 0, -0.2 / D }			 // 19
	};

	//the faces, one at a time
	face0 = {
		{0.2, 0.2, 0.2},
		{0.2 / D, 0.2 * D, 0.0},
		{0.2, 0.2, -0.2 },
		{0.2 * D, 0.0, -0.2 / D},
		{0.2 * D, 0.0, 0.2 / D}
	};

	face1 = {
		{0.0, 0.2 / D, 0.2 * D},
		{-0.2, 0.2, 0.2 },
		{-0.2 / D, 0.2 * D, 0.0},
		{0.2 / D, 0.2 * D, 0.0},
		{0.2, 0.2, 0.2}
	};

	face2 = {
		{ 0.2 / D, 0.2*D, 0 },
		{ -0.2 / D, 0.2*D, 0 },
		{ -0.2, 0.2, -0.2 },			
		{ 0.0, 0.2 / D, -0.2 * D },	
		{ 0.2, 0.2, -0.2 }			
	};

	face3 = {
		{-0.2 * D, 0.0, -0.2 / D},
		{-0.2, -0.2, -0.2},
		{0.0, -0.2 / D, -0.2 * D},
		{0.0, 0.2 / D, -0.2 * D},
		{-0.2, 0.2, -0.2}
	};

	face4 = {
		{ -0.2, -0.2, -0.2 },
		{ -0.2*D, 0.0, -0.2 / D },
		{ -0.2*D, 0.0, 0.2 / D },
		{ -0.2, -0.2, 0.2 },
		{ -0.2 / D, -0.2*D, 0.0 }
	};

	face5 = {
		{0.2 / D, -0.2 * D, 0.0},
		{0.2, -0.2, -0.2},
		{0.0, -0.2 / D, -0.2 * D},
		{-0.2, -0.2, -0.2},
		{-0.2 / D, -0.2 * D, 0.0}
	};

	face6 = {
		{ 0.0, -0.2 / D, -0.2 * D },
		{ 0.2, -0.2, -0.2 },
		{ 0.2*D, 0, -0.2 / D },
		{ 0.2, 0.2, -0.2 },
		{ 0.0, 0.2 / D, -0.2 * D }
	};

	face7 = {
		{0.2 / D, -0.2 * D, 0.0},
		{0.2, -0.2, 0.2},
		{0.2 * D, 0.0, 0.2 / D},
		{0.2 * D, 0.0, -0.2 / D},
		{0.2, -0.2, -0.2}
	};

	face8 = {
		{-0.2 * D, 0.0, 0.2 / D},
		{-0.2 * D, 0.0, -0.2 / D},
		{-0.2, 0.2, -0.2},
		{-0.2 / D, 0.2 * D, 0.0},
		{-0.2, 0.2, 0.2}
	};

	face9 = {
		{0.0, 0.2 / D, 0.2 * D},
		{0.0, -0.2 / D, 0.2 * D},
		{-0.2, -0.2, 0.2},
		{-0.2 * D, 0.0, 0.2 / D},
		{-0.2, 0.2, 0.2}
	};

	face10 = {
		{0.2 * D, 0.0, 0.2 / D},
		{0.2, -0.2, 0.2},
		{0.0, -0.2 / D, 0.2 * D},
		{0.0, 0.2 / D, 0.2 * D},
		{0.2, 0.2, 0.2}
	};

	face11 = {
		{ -0.2, -0.2, 0.2 },
		{ 0.0, -0.2 / D, 0.2 * D },
		{ 0.2, -0.2, 0.2 },
		{ 0.2 / D, -0.2*D, 0 },
		{ -0.2 / D, -0.2*D, 0 }
	};

	//all the faces to have another way to access them
	faces = { face0, face1, face2, face3, face4, face5, face6, face7, face8, face9, face10, face11 };
}


void Dodecahedron::AddTextureCoordinates()
{
	//order: bottom right, bottom left, far left, top, far right
	this->data.textures.push_back(vec2(1.0f / D + 0.2f, 0.0f));
	this->data.textures.push_back(vec2(0.2f, 0.0f));
	this->data.textures.push_back(vec2(0.0f, 1.0f / D));
	this->data.textures.push_back(vec2(0.5f, 1.0f));
	this->data.textures.push_back(vec2(1.0f, 1.0f / D));
}

//Establishes initial vertex attributes (normals, positions, etc)
bool Dodecahedron::PreGLInitialize()
{
	//a and b are used to get n, which determines the direction of a face's normals
	vec3 a;
	vec3 b;
	vec3 n;

	//used to determine which indices of the vertices vector to grab from to get the triangles
	//to draw each face
	int index = 0;

	//set vertices and attributes, one face at a time
	for (int i = 0; i < NUM_FACES; i++)
	{
		//compute normals
		a = normalize(faces[i][1] - faces[i][0]);
		b = normalize(faces[i][2] - faces[i][0]);
		n = normalize(cross(b, a) / 8.0f);

		//loop through each point and store its location, normal, etc.
		for (int j = 0; j < VERTICES_PER_FACE; j++)
		{
			data.vertices.push_back(faces[i][j]);
			data.normals.push_back(n);
			data.colors.push_back(vec4(0.0f, i / 12.0f, 0.0f, 1.0f));
			data.normal_visualization_coordinates.push_back(*(data.vertices.end() - 1));
			data.normal_visualization_coordinates.push_back(*(data.vertices.end() - 1) + n / NORMAL_LENGTH_DIVISOR);
		}
		//store the other important piece of data about each vertex
		AddTextureCoordinates();

		//figure out the triangles each face is made of
		//Triangle 1
		data.indices.push_back(index);
		data.indices.push_back(index + 1);
		data.indices.push_back(index + 2);
		//Triangle 2
		data.indices.push_back(index);
		data.indices.push_back(index + 2);
		data.indices.push_back(index + 3);
		//Triangle 3
		data.indices.push_back(index);
		data.indices.push_back(index + 3);
		data.indices.push_back(index + 4);
		index += VERTICES_PER_FACE;
	}
	data.vbackup = data.vertices;
	return true;
}

void Dodecahedron::NonGLTakeDown()
{
}

void Dodecahedron::RecomputeNormals()
{
	//because using one letter is much easier than typing out the full name each time
	vector<vec3> & v = data.vertices;
	vector<vec3> & n = data.normals;
	vector<vec3> & p = data.normal_visualization_coordinates;
	
	//a and b are used to compute n2, which is the direction for a given face's normals
	vec3 a, b, n2;

	//incrementor variables
	int j = 0;
	int k = 0;

	for (int i = 0; i < NUM_FACES; i++)
	{
		//compute the normals for face i
		a = normalize(v[k + 1] - v[k]);
		b = normalize(v[k + 2] - v[k]);
		n2 = normalize(cross(b, a) / 8.0f);
		
		//update the normals for each vertex on face i to the newly computed value
		for (int ver = 0; ver < VERTICES_PER_FACE; ver++)
		{
			n[k] = n2;
			k++;
		}

		//update the visualization for the normals
		p[j++] = v[i];
		p[j++] = v[i] + n[i] / NORMAL_LENGTH_DIVISOR;
	}
}