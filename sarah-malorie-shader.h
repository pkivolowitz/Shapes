#pragma once
#include "shader.h"

class SarahMalorieShader : public Shader
{
public:
	void Use(glm::mat4 &model_matrix, glm::mat4 &view_matrix, glm::mat4 &projection_matrix);
	void UnUse();
	void SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient);
	void SetLightPosition(glm::vec3 light_position);
	void SelectSubroutine(int subroutine_index);
	void CustomSetup();
	void SetGlobalTime(float global_time);
	void EnableTexture(ILContainer & ilcontainer, GLuint texture_unit);
	void SetSaturationValue(float value);

	enum SubroutineIndices
	{
		CONSTANT = 0,
		BASIC_PHONG = 1,
		PHONG_WITH_TEXTURE = 2,
		SHADER_TOY_1 = 3,
		SHADER_TOY_2 = 4,
		SHADER_TOY_3 = 5,
		SHADER_TOY_4 = 6,
		SHADER_TOY_5 = 7,
		FIRE_AND_ICE = 8
	};


private:

	struct
	{
		GLuint modelview_matrix;
		GLuint view_matrix;
		GLuint normal_matrix;
		GLuint projection_matrix;
		GLuint diffuse_albedo;
		GLuint specular_albedo;
		GLuint specular_power;
		GLuint ambient;
		GLuint light_position;
		GLuint base_texture_location;
		GLuint global_time;
		GLuint saturation_value;
	} uniforms;

	std::vector<GLuint> subroutine_indices;
};