/// \file	Texture.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 

#ifndef _TEXTURE_
#define _TEXTURE_

#include "stdafx.h"



/// \class CTexture
///
/// \brief	to manage a texture : to load / bind / unbind it 
///

class CTexture
{
protected:
	/// \brief constructor
	CTexture();
public:
	/// \brief destructor
	~CTexture();

	/// \brief to load texture from Bitmap file 
	///
	/// \param[in]	p_path	the file path (including the extension)
	///
	static CTexture *loadFromBMP(std::string p_path);

	/// \brief to bind the texture on the given channel
	///
	/// \param[in]	p_channel	the channel on which the texture must be used
	///
	/// \return	true only if the texture is valid (ID !=0)
	///
	bool use(int p_channel);

	/// \brief to get the path of the texture
	///
	/// \return the path of the texture
	///
	std::string getPath();

	/// \brief to unbind the texture on the given channel
	///
	/// \param[in]	p_channel	the channel on which we want to unbind the texture
	static void doNotUseTexture(int p_channel);

	/// \brief to delete all the textures still in memory
	static void deleteAllTextures();

protected:
	GLuint		m_id;									///< id of the texure (OpenGL)
	std::string	m_path;									///< path of the texture (easier to check if it has been already loaded)

	static std::vector<CTexture*> m_textureList;		///< list of textures in memory
};

#endif