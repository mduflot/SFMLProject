#pragma once

#include <GL/glew.h>

class BMP
{
	public:

		static unsigned char *loadBMP_raw(const char *imagepath, unsigned int &width, unsigned int &height); // BGR buffer
		static GLuint loadBMP_texture(const char *imagepath, unsigned int &width, unsigned int &height);
};
