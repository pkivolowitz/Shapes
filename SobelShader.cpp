#include "SobelShader.h"

void SobelShader::Sobel(SobelShader & SobS, FrameBufferObject & fbo, bool copy_back, int image_size, int number_of_samples)
{
	const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	assert(fbo.number_of_color_attachments >= 3);
	assert(fbo.framebuffer_handle != BAD_GL_VALUE);

	SobS.Use();
	glUniform1i(SobS.uniforms.image_size, SobS.image_size = image_size);
	glUniform1i(SobS.uniforms.number_of_samples, SobS.number_of_samples = number_of_samples);
	glUniform1i(SobS.uniforms.source_texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.framebuffer_handle);

	if (copy_back)
	{
		// The idea here is that color attachment 2 has the contents of the current frame
		// Copy it to color attachment 0 and Sobel it, which leaves it again in color attachment
		// two. Color attachment 2 can be blitted to the screen, for example.
		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, SobS.image_size, SobS.image_size, 0, 0, SobS.image_size, SobS.image_size, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
	// Color attachment 0 now has the image to be Sobelred.

	// Next, we bind to texture handle 0 which has been assigned to texture unit 0 above. It is going
	// to be the source texture for the first pass of the Sobel shader.
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);

	// Next we set color attachment 1 as the output of the first pass of the Sobel shader. After being
	// set as the draw buffer, the buffer must be cleared.
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glClearBufferfv(GL_COLOR, 0, clear_color);

	// Just this next line is all that's necessary to invoke the Sobel shader as everything needed is
	// hardcoded into the shader such as texture coordinates and vertex coordinates. It is designed
	// to render a quad.
	glDrawArrays(GL_QUADS, 0, 4);

	// Now prepare for the second pass by making color attachment 1 the source texture and assigning
	// color attachment two as the draw buffer.
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[1]);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glClearBufferfv(GL_COLOR, 0, clear_color);

	// Invoke the second pass of the Sobel.
	glDrawArrays(GL_QUADS, 0, 4);

	// Color attachment 2 should have the finished Sobel. Gracefully unwind.
	glReadBuffer(0);
	glDrawBuffer(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SobS.UnUse();
}

void SobelShader::CustomSetup()
{
	Shader::Use();
	uniforms.image_size = glGetUniformLocation(this->program_id, "image_size");
	uniforms.number_of_samples = glGetUniformLocation(this->program_id, "number_of_samples");
	uniforms.source_texture = glGetUniformLocation(this->program_id, "source_texture");
	Shader::UnUse();
}

void SobelShader::UnUse()
{
	assert(this->is_used == true);
	this->GLReturnedError("BlurShader::UnUse() - entering");
	Shader::UnUse();
}