#include "DNAConstellation.h"
#include <vector>

using namespace glm;


bool DNAConstellation::Initialize(int number_of_objects)
{
	//NOTE: This is meant to represent a diamond-type shape that can have an infinite number of faces 
	
	//number of shapes in between
	int shapes_in_between = 0;
	if (number_of_objects <= 8)
		shapes_in_between = 1;
	else if (number_of_objects <= 32)
		shapes_in_between = 2;
	else if (number_of_objects <= 128)
		shapes_in_between = 4;
	else
		shapes_in_between = 5;
	number_of_objects = number_of_objects / 2;
	int size = int(number_of_objects - number_of_objects % shapes_in_between);
	int num_kites = size / shapes_in_between;
	this->Clear();
	this->Resize((size* 2 - num_kites + 2));
	//the delta represents the distance between two shapes
	float delta = 1.0f / shapes_in_between;

	
	std::vector<vec3> nGon_points = std::vector<vec3>(num_kites);
	
	vec3 vec3_for_spin = vec3(0.0f, 0.0f, 0.0f);
	//this part of the code was inspired from http://statweb.stanford.edu/~susan/phylo/nGon.java
	//creates an nGon for the number of kites possible
	float delta_for_points_on_circle = (2.0f * glm::pi<float>()) / num_kites;
	for (int i = 0; i < num_kites; i++)
	{
		nGon_points[i].x = glm::cos(delta_for_points_on_circle * i);
		nGon_points[i].y = glm::tan(delta_for_points_on_circle * i);// * z_vs_y_sin;
		nGon_points[i].z = glm::sin(delta_for_points_on_circle * i);// *(1 - z_vs_y_sin);
	}
	//end of Susan's ideas
	this->positions[0] = PositionData(vec3(0.0f, 1.0f, 0.0f), vec3_for_spin);
	unsigned int count = 1;
	//this part is used to find the locations of the cylinders from (0,1,0) to each vertex of our nGon
	for (int i = 0; i < num_kites; i++)
	{
		for (float t = delta; t <= 1; t+=delta)
		{
			//vec3_for_spin = vec3(sinf(t), 0.0f, tanf(i));
			this->positions[count++] = PositionData(glm::mix(positions[0].location, nGon_points[i], t), -nGon_points[i]);
			//positions[count - 1].location.z *= 50;
		}
	}
	vec3 bottom_vertex_of_diamond(0.0f, -1.0f, 0.0f);
	//vec3_for_spin = vec3(1.0f, 1.0f, 1.0f);
	positions[count++] = PositionData(bottom_vertex_of_diamond, vec3_for_spin);
	for (int i = 0; i < num_kites; i++)
	{
		for (float t = delta; t <= (1 - delta); t += delta)
		{
			//vec3_for_spin = vec3(sinf(t), 0.0f, tanf(i));
			this->positions[count++] = PositionData(glm::mix(bottom_vertex_of_diamond, nGon_points[i], t), -nGon_points[i]);
			positions[count - 1].location.z *= 50;
		}
	}
	return true;
}

bool DNAConstellation::Reinitialize(int number_of_objects)
{
	return Initialize(number_of_objects);
}
