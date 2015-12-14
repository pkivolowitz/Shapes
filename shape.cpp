#include <iostream>
#include "shape.h"

using namespace glm;
using namespace std;

bool Shape::PostGLInitialize()
{
	return true;
}

void Shape::GLTakeDown()
{
}

void Shape::Draw(bool draw_normals)
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
	this->GLReturnedError("Shape::Draw() - exiting");
}

bool Shape::CommonGLInitialization()
{
	// If there are no vertices defined, then we have no work to do.
	if (this->data.vertices.size() == 0)
		return false;

	// If there are coordinates defined for visualizing the normals, then
	// define the OpenGL data structures needed. This is outside the vector
	// processing below because the visualization of the normals is a different
	// object than the shape itself.

	if (this->data.normal_visualization_coordinates.size() > 0)
	{
		glGenVertexArrays(1, &this->normal_array_handle);
		glBindVertexArray(normal_array_handle);
		glGenBuffers(1, &this->normal_visualization_handle);
		assert(this->normal_visualization_handle != (GLuint)-1);
		glBindBuffer(GL_ARRAY_BUFFER, this->normal_visualization_handle);
		glBufferData(GL_ARRAY_BUFFER, this->data.normal_visualization_coordinates.size() * sizeof(glm::vec3), &this->data.normal_visualization_coordinates[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		glBindVertexArray(0);
	}

	// Begin shape related structures.
	// The vertex array handle is an umbrella OpenGL construct which is used
	// to refer to all the rest of the set up.
	glGenVertexArrays(1, &vertex_array_handle);
	glBindVertexArray(vertex_array_handle);

	// Shove the index array down once. Thereafter, specify a nullptr for the
	// base address of the index array (such as in DrawElements).
	glGenBuffers(1, &this->index_array_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_array_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->data.indices.size() * sizeof(unsigned int), &this->data.indices[0], GL_DYNAMIC_DRAW);

	this->si.push_back(ShapeInfo(VERTICES, BAD_GL_VALUE, (this->data.vertices.size() > 0 ? &this->data.vertices[0] : nullptr), sizeof(vec3), this->data.vertices.size(), GL_FLOAT, 3));
	this->si.push_back(ShapeInfo(COLORS, BAD_GL_VALUE, (this->data.colors.size() > 0 ? &this->data.colors[0] : nullptr), sizeof(vec4), this->data.colors.size(), GL_FLOAT, 4));
	this->si.push_back(ShapeInfo(NORMALS, BAD_GL_VALUE, (this->data.normals.size() > 0 ? &this->data.normals[0] : nullptr), sizeof(vec3), this->data.normals.size(), GL_FLOAT, 3));
	this->si.push_back(ShapeInfo(TEXTURE_COORDINATES, BAD_GL_VALUE, (this->data.textures.size() > 0 ? &this->data.textures[0] : nullptr), sizeof(vec2), this->data.textures.size(), GL_FLOAT, 2));

	for (unsigned int i = 0; i < this->si.size(); i++)
	{
		if (this->si[i].number_of_elements > 0)
		{
			glGenBuffers(1, &this->si[i].handle);
			assert(this->si[i].handle != (GLuint)-1);
			glBindBuffer(GL_ARRAY_BUFFER, this->si[i].handle);
			glBufferData(GL_ARRAY_BUFFER, this->si[i].number_of_elements * this->si[i].sizeof_element, this->si[i].data, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(this->si[i].channel_number);
			glVertexAttribPointer(this->si[i].channel_number, this->si[i].number_of_type, this->si[i].type, GL_FALSE, 0, (GLubyte *)NULL);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	this->GLReturnedError("Shape::CommonGLInitialization - exiting.");
	return true;
}

/*	min and max now specify a range of OFFSETS to a previous color.
*/

glm::vec4 Shape::RandomColor(vec4 & previous_color, float min , float max)
{
	return clamp(vec4(Random(min , max) , Random(min , max) , Random(min , max), 1.0f) + previous_color, vec4(0.0f) , vec4(1.0f));
}

//	blob may be null
void Shape::UpdateValues(void(*Update)(struct Data & data, float current_time, void * blob), float current_time, void * blob)
{
	if (Update == nullptr)
		throw std::invalid_argument("Shape::UpdateValues() called with null update function");

	(*Update)(this->data, current_time, blob);
	this->RecomputeNormals();

	if (this->data.normal_visualization_coordinates.size() > 0)
	{
		glBindVertexArray(normal_array_handle);
		glBindBuffer(GL_ARRAY_BUFFER, this->normal_visualization_handle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, this->data.normal_visualization_coordinates.size() * sizeof(vec3), &this->data.normal_visualization_coordinates[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	glBindVertexArray(vertex_array_handle);
	for (unsigned int i = 0; i < this->si.size(); i++)
	{
		if (this->si[i].number_of_elements > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->si[i].handle);
			glBufferSubData(GL_ARRAY_BUFFER, 0, this->si[i].number_of_elements * this->si[i].sizeof_element, this->si[i].data);
			glEnableVertexAttribArray(this->si[i].channel_number);
			glVertexAttribPointer(this->si[i].channel_number, this->si[i].number_of_type, this->si[i].type, GL_FALSE, 0, (GLubyte *)NULL);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool Shape::GLReturnedError(char * s)
{
	bool return_error = false;
#ifdef DEBUG
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}
#endif
	return return_error;
}
