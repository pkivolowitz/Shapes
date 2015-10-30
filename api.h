#pragma once
#include <vector>
#include "shape.h"

class Transition
{
public:
	Transition(unsigned int flags);

	enum TRANSITION_FLAGS
	{
		OUTGOING_ONLY		= 1 << 0,	// Transition can be used only for outgoing image
		INCOMING_ONLY		= 1 << 1,	// Transition can only be used for incoming image
		BIDIRECTIONAL_ONLY	= 1 << 2,	// Transition must handle both directions
		OUTGOING			= 1 << 3,	// Transition can handle outgoing image
		INCOMING			= 1 << 4,	// Transition can handle incoming image
		BIDIRECTIONAL		= 1 << 5,	// Transition can handle both directions
		FILLS_SCREEN		= 1 << 6	// Transition blocks view behind at some point
	};

	typedef struct MaterialSpecificationBlob
	{
		glm::vec3 diffuse_albedo;
		glm::vec3 specular_albedo;
		glm::vec3 ambient_color;
		float specular_power;
	} MaterialSpecification;

	// Identify is called after the effect is loaded. It returns flags to the caller indicating various attributes
	// about this effect. For example, if this transitional effect were a zoom fade, this function would return:
	// BIDIRECTIONAL_ONLY | FILLS_SCREEN
	// Parameters
	// float *			minimum_time	If not null, return the minimum time (in seconds) that this transition likes to run for.
	// float *			maximum_time	If not null, return the maximum time (in seconds) that this transition likes to run for.
	// If these are set, the caller will never go outside these bounds for 'time' in Render(). 

	virtual TRANSITION_FLAGS Identify(float * minimum_time, float * maximum_time) = 0;

	// Prepare is called when a transitional effect is selected for the outgoing or incoming or both effects.
	// Parameters:
	// unsigned int		flags			Chosen from TRANSITION_FLAGS
	// Shape *			s				nullptr or a pointer to the shape that should be animated.
	// vector<GLuint>	texture_handles	If empty, no texturing is to be used.
	//									If size is 1 or more, use [0] as the effect's incoming texture
	//									If size is 2 or more, use [1] as the effect's outgoing texture
	// void	fptr		Update			If not nullptr, this function should be called at the end of every Render.
	//									It will rearrange the geometry of the shape and recalculate lighting.

	virtual bool Prepare(unsigned int flags, Shape * s, std::vector<GLuint> & texture_handles, void(*Update)(struct Data & data, float current_time, void * blob), float current_time, void * blob) = 0;

	virtual bool Render(unsigned int flags, float time, MaterialSpecification & materials) = 0;
	virtual bool Finish(unsigned int flags) = 0;

protected:
	unsigned int flags;
};