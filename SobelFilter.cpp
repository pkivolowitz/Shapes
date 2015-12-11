#include "SobelFilter.h"

using namespace std;

void SobelFilter::UnUse()
{
	assert(this->is_used == true);
	this->GLReturnedError("SobelFilter::UnUse() - entering");
	Shader::UnUse();
}

void SobelFilter::CustomSetup()
{
	Shader::Use();
	uniforms.image_size = glGetUniformLocation(this->program_id, "image_size");
	uniforms.texture = glGetUniformLocation(this->program_id, "RenderTex");
	Shader::UnUse();
}

void SobelFilter::SF(SobelFilter & sf, FrameBufferObject & fbo, bool copy_back, int image_size, int number_of_samples)
{
	const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	assert(fbo.number_of_color_attachments >= 3);
	assert(fbo.framebuffer_handle != BAD_GL_VALUE);

	sf.Use();
	glUniform1i(sf.uniforms.image_size, sf.image_size = image_size);
	glUniform1i(sf.uniforms.texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.framebuffer_handle);

	if (copy_back)
	{
		// The idea here is that color attachment 2 has the contents of the current frame
		// Copy it to color attachment 0 and blur it, which leaves it again in color attachment
		// two. Color attachment 2 can be blitted to the screen, for example.
		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, sf.image_size, sf.image_size, 0, 0, sf.image_size, sf.image_size, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
	// Color attachment 0 now has the image to be blurred.

	// Next, we bind to texture handle 0 which has been assigned to texture unit 0 above. It is going
	// to be the source texture for the first pass of the blur shader.
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);

	// Next we set color attachment 1 as the output of the first pass of the blur shader. After being
	// set as the draw buffer, the buffer must be cleared.
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glClearBufferfv(GL_COLOR, 0, clear_color);

	// Just this next line is all that's necessary to invoke the blur shader as everything needed is
	// hardcoded into the shader such as texture coordinates and vertex coordinates. It is designed
	// to render a quad.
	glDrawArrays(GL_QUADS, 0, 4);

	// Color attachment 2 should have the finished blur. Gracefully unwind.
	glReadBuffer(0);
	glDrawBuffer(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	sf.UnUse();
}
