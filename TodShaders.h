#pragma once
#include "shader.h"

class TodShaders : public Shader
{
public:
	void Use(glm::mat4 &model_matrix, glm::mat4 &view_matrix, glm::mat4 &projection_matrix);
	void UnUse();
	void SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient) {}
	void SelectSubroutine(int subroutine_index);
	void CustomSetup();
	void SetGlobalTime(float global_time);
	void EnableTexture(ILContainer & ilcontainer, GLuint texture_unit);

	enum SubroutineIndices
	{
		SPACE,
		FIRE,
		NOISE,
		FRACTAL,
		NUM_FOO
	};

private:

	struct
	{
		GLuint modelview_matrix;
		GLuint view_matrix;
		GLuint normal_matrix;
		GLuint projection_matrix;
		GLuint base_texture_location;
		GLuint global_time;
	} uniforms;

	std::vector<GLuint> subroutine_indices;
};