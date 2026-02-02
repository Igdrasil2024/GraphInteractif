/// \file	Texture.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#include "stdafx.h"
#include "Texture.h"


std::vector<CTexture*> CTexture::m_textureList;


CTexture::CTexture():
	m_id(0)
{
	m_textureList.push_back(this);
}


CTexture::~CTexture()
{
	std::vector<CTexture*>::iterator found = std::find(m_textureList.begin(), m_textureList.end(), this);
	if (found != m_textureList.end())
		m_textureList.erase(found);

	if (m_id > 0)
		glDeleteTextures(1, &m_id);
}


CTexture *CTexture::loadFromBMP(std::string p_path)
{
	if (!std::filesystem::exists(p_path))
		return nullptr;

	// Create an empty texture object
	CTexture * tex = new CTexture();
	tex->m_path = p_path;

	SDL_Surface* bmp = nullptr;
	// FOKS LAB ---
	// Load bitmap from BMP file
	// SDL_LoadBMP
	bmp = SDL_LoadBMP(p_path.c_str());

	// delete texture if not loaded properly by FreeImage
	if (!bmp)
	{
		delete tex;
		return nullptr;
	}


	// NB : bitmap texture will be a texture is a GL_TEXTURE_2D 
	// Generate the texture ID on the video card
	// glGenTextures
	glGenTextures(1, &tex->m_id);
	if (tex->m_id != 0)
	{
		// bind it 
		// glBindTexture
		glBindTexture(GL_TEXTURE_2D, tex->m_id);
		// Set the filters on the sampler
		// glTexParameteri GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, ...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		// set anisotropic filtering if available
		if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic"))
		{
			float anisotropic = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
		}

		// invert image vertically ( BMP :) )
		unsigned char* data = new unsigned char[bmp->w * bmp->h * 3];
		unsigned char* dataSrc = (unsigned char*)bmp->pixels;
		for (int row = 0; row < bmp->h; row++)
			for (int col = 0; col < bmp->w; col++)
			{
				data[((bmp->h - 1 - row) * bmp->w + col) * 3 + 0] = dataSrc[(row * bmp->w + col) * 3 + 0];
				data[((bmp->h - 1 - row) * bmp->w + col) * 3 + 1] = dataSrc[(row * bmp->w + col) * 3 + 1];
				data[((bmp->h - 1 - row) * bmp->w + col) * 3 + 2] = dataSrc[(row * bmp->w + col) * 3 + 2];
			}

		// Load data in video memory 
		// glTexImage2D
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp->w, bmp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

		delete[] data; // unload data used for texture creation

		// to generate mimapping for the loaded texture (only for OpenGL >=3.0)
		// this function should be called after loading a texture (or rendering a texture throught FBO) to generate the mipmapping levels
		// mipmapping levels can be configured using glTexParameter function 
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	SDL_FreeSurface(bmp); // unlaod SDL surface

	return tex;
}


bool CTexture::use(int p_channel)
{
	glActiveTexture(GL_TEXTURE0+p_channel);			// select the channel
	glBindTexture(GL_TEXTURE_2D,m_id);	// bind the texture using its OpenGL ID

	return (m_id != 0);
}

std::string CTexture::getPath()
{
	return m_path;
}


void CTexture::doNotUseTexture(int p_channel)
{
	glActiveTexture(GL_TEXTURE0+p_channel);			// select the channel
	glBindTexture(GL_TEXTURE_2D, 0);	// unbind the current texture
}


void CTexture::deleteAllTextures()
{
	while (m_textureList.size() != 0)
	{
		delete m_textureList.back();
	}
}