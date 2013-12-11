#include "common.h"
#include "TextureOGL.h"

void TextureOGL::LoadFromFile(string filename) {
	LoadPNG(filename);
}

void TextureOGL::LoadPNG(string filename) {
	//
	// Load and decode a PNG file so we can make a texture out of it.
	//

	std::vector< unsigned char > rawImage;
	LodePNG::loadFile( rawImage, filename.c_str() );

	LodePNG::Decoder decoder;
	std::vector< unsigned char > image;
	decoder.decode( image, rawImage.empty() ? 0 : &rawImage[0], (unsigned)rawImage.size() );

	m_Height = decoder.getHeight();
	m_Width = decoder.getWidth();

	//
	// Flip and invert the PNG image since OpenGL likes to load everything
	// backwards from what is considered normal!
	//

	unsigned char *imagePtr = &image[0];
	int halfTheHeightInPixels = decoder.getHeight() / 2;
	int heightInPixels = decoder.getHeight();

	// Assuming RGBA for 4 components per pixel.
	int numColorComponents = 4;

	// Assuming each color component is an unsigned char.
	int widthInChars = decoder.getWidth() * numColorComponents;

	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;

	for( int h = 0; h < halfTheHeightInPixels; ++h )
	{
		top = imagePtr + h * widthInChars;
		bottom = imagePtr + (heightInPixels - h - 1) * widthInChars;

		for( int w = 0; w < widthInChars; ++w )
		{
			// Swap the chars around.
			temp = *top;
			*top = *bottom;
			*bottom = temp;

			++top;
			++bottom;
		}
	}

	//
	// Create the OpenGL texture and fill it with our PNG image.
	//

	// Allocates one texture handle
	glGenTextures( 1, &m_ID );

	// Binds this texture handle so we can load the data into it
	glBindTexture( GL_TEXTURE_2D, m_ID );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, decoder.getWidth(), decoder.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0] );
}