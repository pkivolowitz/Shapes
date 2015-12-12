#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "OutlineShader.h"

using namespace std;
using namespace glm;

void OutlineShader::UnUse()
{
	assert(this->is_used == true);
	this->GLReturnedError("OutlineShader::UnUse() - entering");
	Shader::UnUse();
	this->GLReturnedError("OutlineShader::UnUse() - exiting");
}

void OutlineShader::CustomSetup()
{
	Shader::Use();
	uniforms.image_size = glGetUniformLocation(this->program_id, "image_size");
	uniforms.texture = glGetUniformLocation(this->program_id, "RenderTex");
	Shader::UnUse();
}

void OutlineShader::Outline(OutlineShader & os, FrameBufferObject & fbo, bool copy_back, int image_size, int number_of_samples)
{
	const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	assert(fbo.number_of_color_attachments >= 3);
	assert(fbo.framebuffer_handle != BAD_GL_VALUE);
	
	os.Use();
	glUniform1i(os.uniforms.image_size, os.image_size = image_size);
	glUniform1i(os.uniforms.texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.framebuffer_handle);

	if (copy_back)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, os.image_size, os.image_size, 0, 0, os.image_size, os.image_size, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);

	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glClearBufferfv(GL_COLOR, 0, clear_color);

	glDrawArrays(GL_QUADS, 0, 4);

	glReadBuffer(0);
	glDrawBuffer(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	os.UnUse();
}