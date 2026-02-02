/// \file	Mesh.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#ifndef _MESH_
#define _MESH_

#include "stdafx.h"
#include "Texture.h"

// offset "value" bytes from the 0 address
#define OFFSET(value)	((char*)nullptr+(value))

// delimiter to read into OBJ file
#define OBJ_DELIMITER	" \t"
#define OBJ_DELIMITER2	"/"

// to enumerate the UVs channels 
enum {
	UV0,
	UV1,
	UV2,
	UV3,
	NB_UVS
};


#define MESH_HAS_NORMAL		0x01
#define MESH_HAS_TANGENT	0x02
#define MESH_HAS_COLOR		0x04
#define MESH_HAS_TEXCOORD0	0x08
#define MESH_HAS_TEXCOORD1	0x10
#define MESH_HAS_TEXCOORD2	0x20
#define MESH_HAS_TEXCOORD3	0x40

#define MESH_HAS_ALL		0xFF

#define FLAG(v,f)			( ((v)&f) == (f))

typedef unsigned char meshFlag;


#pragma pack(push,1)			// to limit the memory used on video RAM

struct vertex {
	glm::vec3 m_position;			///< the position of the vertex			(OFFSET 0)
	glm::vec3 m_normal;				///< the normal							(OFFSET 12)
	glm::vec3 m_tangent;			///< the tangent 						(OFFSET 24)
	glm::vec3 m_bitangent;			///< the bitangent						(OFFSET 36)
	glm::vec4 m_color;				///< the color							(OFFSET 48)
	glm::vec2 m_texcoord[NB_UVS];	///< the UV coordinates on each channel	(OFFSET 64)

	/// \brief constructor of the vertex structure to set default values
	vertex()
	{
		m_position =	glm::vec3(0.0f);
		m_tangent =		glm::vec3(1.0f, 0.0f, 0.0f);
		m_bitangent =	glm::vec3(0.0f, 1.0f, 0.0f);
		m_normal =		glm::vec3(0.0f, 0.0f, 1.0f);
		m_color =		glm::vec4(1.0f);

		for (size_t i = 0; i < NB_UVS; i++)
			m_texcoord[i] = glm::vec2(0.0f);
	}
};

union face {
		unsigned int index[3];				///< the indices of the vertices in the vertices array to create a triangle 
		struct{
			unsigned int a, b, c;
		};
};

#pragma pack(pop)


/// \class CMesh
/// \brief to manage the mesh
///
/// can load the mesh from OBJ file & create a VAO to optimize the rendering speed
///

class CMesh
{
public:
	/// \brief constructor
	///
	/// \param[in]	p_vertexArray	the array of vertex data (position, normal, tangent, ...)
	/// \param[in]	p_faceArray		the array of faces (3 indices per face)
	///
	CMesh(std::vector<vertex> &p_vertexArray, std::vector<face> &p_faceArray, meshFlag p_flags = MESH_HAS_ALL);
	/// \brief destructor
	~CMesh();

	
	/// \brief to draw the mesh 
	void draw(glm::mat4 &p_model, glm::mat4 &p_view, glm::mat4 &p_projection);

	/// \brief to set a texture on Mesh
	///
	/// \param[in]	p_texture	texture to set on mesh
	///
	void setTexture(CTexture* p_texture);

	/// \brief to load a mesh from an OBJ file
	///
	/// \param[in] p_path	the OBJ file path
	///
	static CMesh * loadFromOBJ(std::string p_path);


	/// \brief method to delete all the lasting meshes in memory. 
	static void deleteAllMeshes();

protected:

	/// \brief to generate the VAO from the arrays 
	///
	/// \return true if the creation has succedded
	///
	bool createVAOFromArrays(std::vector<vertex> &p_vertexArray, std::vector<face> &p_faceArray, meshFlag p_flags = MESH_HAS_ALL);

	/// \brief to delete the vao/vbo if created
	void deleteVAO();

	GLuint				m_vboArrayBuffer;			///< VBO array buffer id (OpenGL)
	GLuint				m_vboElementArrayBuffer;	///< VBO element array buffer id (OpenGL)
	GLuint				m_vao;						///< VAO id (OpenGL)
	std::vector<vertex> m_vertexArray;				///< array of vertices 
	std::vector<face>	m_faceArray;				///< array of faces
	unsigned int		m_nbFaces;					///< the number of faces to draw with VAO

	CTexture*			m_texture;					///< texture to apply on mesh

	meshFlag			m_flags;					///< combination of flags to indicate which parameters are to be used (normals, texcoord0, ...)

	static std::vector<CMesh*> m_meshList;			///< list containing all the meshes in memory
};

#endif