#pragma once

#include <vector>
#include <assert.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

class Constellation
{
public:
	const int MAXIMUM_NUMBER_OF_OBJECTS = 525;

	class PositionData
	{
	public:
		PositionData()
		{
			this->location = glm::vec3();
			this->outward_direction_vector = glm::vec3();
		}

		PositionData(glm::vec3 & l , glm::vec3 & o)
		{
			this->location = l;
			this->outward_direction_vector = o;
		}

		glm::vec3 location;
		glm::vec3 outward_direction_vector;
	};

	Constellation()
	{
		this->positions.reserve(MAXIMUM_NUMBER_OF_OBJECTS);
	}

	// Return size of positions vector. Useful for dermining if this constellation has been initialized yet
	// as size will be zero if uninitialized.
	size_t Size()
	{
		return this->positions.size();
	}

	std::vector<PositionData> positions;

	PositionData& operator[] (const int nIndex)
	{
		return this->positions[nIndex];
	}

	std::vector<PositionData> & GetPositionData()
	{
		return this->positions;
	}

	void PushBack(glm::vec3 & l , glm::vec3 & o)
	{
		this->positions.push_back(PositionData(l, o));
	}

	void PushBack(PositionData & p)
	{
		this->positions.push_back(p);
	}

	void Clear()
	{
		this->positions.clear();
	}

	void Resize(int number_of_objects)
	{
		this->positions.resize(number_of_objects);
	}

	virtual bool Initialize(int number_of_objects) = 0;
};
