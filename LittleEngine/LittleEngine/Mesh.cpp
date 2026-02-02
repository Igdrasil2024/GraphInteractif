/// \file	Mesh.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#include "stdafx.h"
#include "Mesh.h"

std::vector<CMesh*> CMesh::m_meshList;


CMesh::CMesh(std::vector<vertex> &p_vertexArray, std::vector<face> &p_faceArray, meshFlag p_flags) :
	m_flags(p_flags),
	m_vao(0),
	m_vboArrayBuffer(0),
	m_vboElementArrayBuffer(0),
	m_nbFaces(0),
	m_texture(nullptr)
{

	if (!createVAOFromArrays(p_vertexArray, p_faceArray))
	{
		m_vertexArray = p_vertexArray;
		m_faceArray = p_faceArray;
	}
	m_nbFaces = p_faceArray.size();

	m_meshList.push_back(this);
}


CMesh::~CMesh()
{
	std::vector<CMesh*>::iterator found = std::find(m_meshList.begin(), m_meshList.end(), this);
	if(found!=m_meshList.end())
		m_meshList.erase(found);

	deleteVAO();
}

bool CMesh::createVAOFromArrays(std::vector<vertex> &p_vertexArray, std::vector<face> &p_faceArray, meshFlag p_flags)
{
	if (!p_vertexArray.size() || !p_faceArray.size())
		return false;

	// TRY TO CREATE A VAO
	// Check extension using glew 
	if (!glewIsExtensionSupported("GL_ARB_vertex_array_object GL_ARB_vertex_buffer_object"))
		return false;	// mesh is loaded but not stored into video RAM as VAO or VBO


	// FOKS LAB ---
	
	// glGenVertexArrays
	glGenVertexArrays(1, &m_vao);
	if (m_vao > 0)
	{
		// glBindVertexArray
		glBindVertexArray(m_vao);

		// glGenBuffers
		// glBindBuffer GL_ARRAY_BUFFER / GL_ELEMENT_ARRAY_BUFFER
		// glBufferData GL_STATIC_DRAW / GL_DYNAMIC_DRAW / GL_STREAM_DRAW
		glGenBuffers(1, &m_vboArrayBuffer); // pon peut tester le résultat ... quand on a le temps :) 
		glBindBuffer(GL_ARRAY_BUFFER, m_vboArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, p_vertexArray.size() * sizeof(vertex), &p_vertexArray[0], GL_STATIC_DRAW);
		glGenBuffers(1, &m_vboElementArrayBuffer); // idem : test du résultat
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboElementArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_faceArray.size() * sizeof(face), &p_faceArray[0], GL_STATIC_DRAW);

		// glEnableClientState	GL_VERTEX_ARRAY / GL_TEXCOORD_ARRAY / GL_NORMAL_ARRAY
		// glVertexPoint / glTexCoordPointer / glNormalPointer
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(vertex), (void*)0);  // dernier paramètre = offset à partir du début du tableau (cf. la déclaration de la structure vertex)

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), (void*)64);

		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(vertex), (void*)12);

		glBindVertexArray(0);
	}



	return false;
}


int getVec4(std::string &p_str, glm::vec4 &p_float4)
{
	int nbValues = 0;
	p_float4 = glm::vec4(0.0);

	std::string buf = p_str;
	do{
		size_t pos = 0;
		float value = std::stof(buf, &pos);
		buf = buf.substr(pos);
		if (pos == std::string::npos)
			break;
		
		p_float4[nbValues] = value;
		nbValues++;
		
	} while (nbValues <4 && buf.compare(""));

	return nbValues;
}

int getFaceElement(std::string &p_str, std::vector<std::string> &p_elements)
{
	if (p_elements.size())
		p_elements.clear();

	size_t pos = 0;
	std::string buf = p_str;
	while (pos != std::string::npos)
	{
		pos = p_str.find_first_not_of(OBJ_DELIMITER, pos);
		if (pos != std::string::npos)
		{
			size_t pos2 = p_str.find_first_of(OBJ_DELIMITER, pos);
			buf = p_str.substr(pos, pos2-pos+1);
			p_elements.push_back(buf);
			pos = pos2;
		}
	}
	return p_elements.size();
}

bool getIndices(std::string &p_str, glm::ivec3 &p_int3, bool *p_hasTexcoord, bool *p_hasNormal)
{
	p_hasTexcoord = p_hasNormal = false;

	p_int3 = glm::ivec3(0);
	size_t pos = 0;
	pos = p_str.find_first_not_of(OBJ_DELIMITER);

	if (pos == std::string::npos)
		return false;

	std::string index = p_str;
	index = index.substr(pos);
	
	int i = 0;
	while (i<3 && pos != std::string::npos)
	{
		pos = index.find_first_of(OBJ_DELIMITER2);
		if (pos > 0)
		{
			p_int3[i] = std::stoi(index.substr(0, pos));
			switch (i)
			{
				// TEXCOORD
				case 1 :
					if (p_hasTexcoord)
						*p_hasTexcoord = true;
					break;
				// NORMAL
				case 2:
					if (p_hasNormal)
						*p_hasNormal = true;
					break;
			}
		}
		index = index.substr(pos + 1);
		i++;
	}

	return true;
}


CMesh * CMesh::loadFromOBJ(std::string p_path)
{

	std::ifstream file(p_path);

	// To store the line (reading line by line)
	std::string buf;
	// to store all the data from the OBJ file
	std::vector<glm::vec3>	positions;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec3>	texcoords;
	std::vector<face>		facesPositions;
	std::vector<face>		facesTexcoords;
	std::vector<face>		facesNormals;
	// to check if there are texture coordinates or normals
	bool texcoord = true;
	bool normal = true;

	// read all the lines in the OBJ file
	while (std::getline(file, buf)) {

		// tokenize to get the token & the value
		size_t pos = buf.find_first_of(OBJ_DELIMITER);
		std::string token = "";
		if (pos != std::string::npos)
		{
			token = buf.substr(0, pos);
			buf = buf.substr(pos);
		}
		else
		{
			token = buf;
			buf = "";
		}

		// if no token found goto next line
		if (!token.compare(""))
			continue;

		switch (token.at(0)) {
			case '#':
				//comment line
				break;

			case 'v':	//VERTEX
			{
					
				glm::vec4 data;
				int nbData = getVec4(buf, data);
				// v = POSITION
				if (token.size() == 1)
				{
					//vertex 3  components
					positions.push_back(data.xyz);
					assert(nbData == 3);
					break;;
				}

				switch (token.at(1)) {

					case 'n':	// vn = NORMAL
						//normal, 3 components
						normals.push_back(data.xyz);
						assert(nbData == 3);
						break;

					case 't':	// vt = TEXTURE COORDINATES
						//texcoord, 2 or 3 components, only 2 will be used next in the code
						texcoords.push_back(data.xyz);
						assert(nbData > 1 && nbData < 4);
						break;
				}
				break;
			}
			case 'f':	// f = FACE
			{
					//face
					std::vector<std::string> elements;
					if (getFaceElement(buf, elements)<3)
					{
						std::cout << "Error while reading face : not enought vertices to create a triangle\n";
						break;
					}
					if (getFaceElement(buf, elements)>3)
					{
						std::cout << "Error while reading face : only triangle primitives are accepted\n";
						break;
					}


					// Face indices :
					//
					//		A/B/C
					//
					//		A = position			(0)
					//		B = texture coordinates (1)
					//		C = normal				(2)
					// 
					// each vertex+attributes can be defined as :
					//		A		only position
					//		A/B		position + texture coordinates
					//		A//C	position + normal
					//		A/B/C	position + texturecoordinates + normal
					//

					face face[3];
					glm::ivec3 indices;
					if (!getIndices(elements[0], indices, nullptr, nullptr))
					{
						std::cout << "Error while reading face : no way to get position index\n";
						break;
					}

					// 0 = position
					// 1 = texcoord
					// 2 = normal
					for (size_t k = 0; k < 3; k++)
					{
						face[k].index[0] = indices[k];
					}

					if (texcoord)
					{
						if (!getIndices(elements[1], indices, nullptr, nullptr))
						{
							std::cout << "Error while reading face : no way to get texcoord index\n";
							break;
						}

						// 0 = position
						// 1 = texcoord
						// 2 = normal
						for (size_t k = 0; k < 3; k++)
						{
							face[k].index[1] = indices[k];
						}
					}

					if (normal)
					{
						if (!getIndices(elements[2], indices,nullptr, nullptr))
						{
							std::cout << "Error while reading face : no way to get normal index\n";
							break;
						}

						// 0 = position
						// 1 = texcoord
						// 2 = normal
						for (size_t k = 0; k < 3; k++)
						{
							face[k].index[2] = indices[k];
						}
					}


					facesPositions.push_back(face[0]);
					if(texcoord)
						facesTexcoords.push_back(face[1]);
					if (normal)
						facesNormals.push_back(face[2]);

					break;
			}

			case 's':
			case 'g':
			case 'u':
				//all presently ignored
			default:
				break;
		};
	}

	file.close();


	std::vector<vertex> vertexArray;
	std::vector<face>	faceArray;


	// FOKS LAB --- 
	
	// fill vertexArray and faceArray from arrrays read in the OBJ file

	for (int i = 0; i < (int)facesPositions.size(); i++)
	{
		vertex v1;
		v1.m_position = positions[facesPositions[i].a-1];
		v1.m_texcoord[0] = texcoords[facesTexcoords[i].a - 1];
		v1.m_normal = normals[facesNormals[i].a - 1];
		vertexArray.push_back(v1);

		vertex v2;
		v2.m_position = positions[facesPositions[i].b - 1];
		v2.m_texcoord[0] = texcoords[facesTexcoords[i].b - 1];
		v2.m_normal = normals[facesNormals[i].b - 1];
		vertexArray.push_back(v2);

		vertex v3;
		v3.m_position = positions[facesPositions[i].c - 1];
		v3.m_texcoord[0] = texcoords[facesTexcoords[i].c - 1];
		v3.m_normal = normals[facesNormals[i].c - 1];
		vertexArray.push_back(v3);

		face f;
		f.a = i * 3 + 0;
		f.b = i * 3 + 1;
		f.c = i * 3 + 2;
		faceArray.push_back(f);
	}




	CMesh * m = new CMesh(vertexArray, faceArray);
	
	return m;
}


void CMesh::setTexture(CTexture* p_texture)
{
	m_texture = p_texture;
}




void CMesh::draw(glm::mat4 &p_model, glm::mat4 &p_view, glm::mat4 &p_projection)
{
	glBindTexture(GL_TEXTURE_2D, m_texture ? m_texture->use(0): 0);

	// VAO ================================
	if (m_vao)
	{
		// VAO has been created
		// bind VAO
		glBindVertexArray(m_vao);
		// glDrawElements
		glDrawElements(GL_TRIANGLES, m_nbFaces * 3, GL_UNSIGNED_INT, 0);
		// unbind VAO
		glBindVertexArray(0);
	}
	else
	// VERTEX-AT-IME =====================
	{
		// set color
		glColor3f(1.0f, 1.0f, 1.0f);
		
		//glBegin GL_TRIANGLES (primitive)
		glBegin(GL_TRIANGLES);
		{
		// 
		// Loop on all the faces => set each vertex (glVertex3f at the end of vertex description : color/texcoord/normal/vertex)
			std::vector<face>::iterator it = m_faceArray.begin();
			for (; it != m_faceArray.end(); it++)
			{
				face f = *it;
				for (int k = 0; k < 3; k++)
				{
					glNormal3fv(&m_vertexArray[f.index[k]].m_normal.x);
					glTexCoord2fv( &m_vertexArray[f.index[k]].m_texcoord[0].x );
					glVertex3fv( &m_vertexArray[f.index[k]].m_position.x);
				}
			}
		}
		glEnd();
		// 
		// glEnd
	}
}



void CMesh::deleteAllMeshes()
{
	while (m_meshList.size() != 0)
	{
		delete m_meshList.back();
	}

}

void CMesh::deleteVAO()
{
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
	
	if (m_vboArrayBuffer)
		glDeleteBuffers(1, &m_vboArrayBuffer);

	if (m_vboElementArrayBuffer)
		glDeleteBuffers(1, &m_vboElementArrayBuffer);

	m_vao = 0;
	m_vboArrayBuffer = m_vboElementArrayBuffer = 0;

}