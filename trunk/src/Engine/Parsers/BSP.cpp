/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*****************************************************************************************/ 
#include "BSP.h"
#include <GL/glext.h>
#include <GL/glfw.h>

#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'V')

const int OE::Parsers::BSP::ParseBSP(const char *path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");

	if(pFile==NULL)
	{
		std::cerr << "Error - Failed to load map \"" << path << "\", no such map!" << std::endl;
		return -1;
	}

	fclose(pFile);

	_iBSPType = BSP_TYPE_IBSP;

	//	Read the first 4 bytes for the magic number.
	/*
	char* magic = (char*)malloc(sizeof(char)*5);
	fread((char*)magic,sizeof(magic),1, pFile);
	magic[4] = '\0';
	fclose(pFile);
	if(strcmp(magic, "IBSP") == 0)
	{
	_iBSPType = BSP_TYPE_IBSP;
	free(magic);
	ParseIBSP(path);
	}
	else
	if(strcmp(magic, "VBSP") == 0)
	{
	_iBSPType = BSP_TYPE_VBSP;
	free(magic);
	ParseVBSP(path);
	}
	return 0;
	*/
	std::string extensions = std::string((const char*)glGetString(GL_EXTENSIONS));

	if(extensions.find_first_of("GL_EXT_texture_filter_anisotropic")!=-1)
	{
		std::cout << "Anisotropic supported on your video card!" << std::endl;
	}

	if(extensions.find_first_of("GL_ARB_multitexture")!=-1)
	{
		std::cout << "Multitexturing supported on your video card" << std::endl;
	}

	ParseIBSP(path);
	return 0;
}

void OE::Parsers::BSP::_IBSP_ParseVertices(FILE *file, _IBSP_HEADER *header)
{
	_iNumVertices = header->DirEntries[10].Length / sizeof(_IBSP_VERTEX);
	_vVertices = (_OBSP_VERTEX*)malloc(sizeof(_OBSP_VERTEX)*_iNumVertices);
	_IBSP_VERTEX *vertices = (_IBSP_VERTEX*)malloc(sizeof(_IBSP_VERTEX)*_iNumVertices);
	fseek(file, header->DirEntries[10].Offset, SEEK_SET);
	fread((_IBSP_VERTEX*)vertices, sizeof(_IBSP_VERTEX), _iNumVertices, file);

	for(int i = 0; i < _iNumVertices; i++)
	{
		int y = vertices[i].Position[2];
		int z = -vertices[i].Position[1];
		_OBSP_VERTEX tVertex;
		tVertex.Position[0] = vertices[i].Position[0];
		tVertex.Position[1] = y;
		tVertex.Position[2] = z;

		tVertex.TexCoord[0][0] = vertices[i].TexCoord[0][0];
		tVertex.TexCoord[0][1] = vertices[i].TexCoord[0][1];

		tVertex.TexCoord[1][0] = vertices[i].TexCoord[1][0];
		tVertex.TexCoord[1][1] = vertices[i].TexCoord[1][1];

		tVertex.Normal[0] = vertices[i].Normal[0];
		tVertex.Normal[1] = vertices[i].Normal[1];
		tVertex.Normal[2] = vertices[i].Normal[2];

		tVertex.Color[0] = vertices[i].Color[0];
		tVertex.Color[1] = vertices[i].Color[1];
		tVertex.Color[2] = vertices[i].Color[2];
		tVertex.Color[3] = vertices[i].Color[3];
		_vVertices[i] = tVertex;
	}

	free(vertices);
}

void OE::Parsers::BSP::_IBSP_ParseFaces(FILE* file, _IBSP_HEADER *header)
{
	_iNumFaces = header->DirEntries[13].Length / sizeof(_IBSP_FACE);
	_vFaces = (_OBSP_FACE*)malloc(sizeof(_OBSP_FACE)*_iNumFaces);
	_IBSP_FACE *faces = (_IBSP_FACE*)malloc(sizeof(_IBSP_FACE)*_iNumFaces);
	fseek(file, header->DirEntries[13].Offset, SEEK_SET);
	fread((_IBSP_FACE*)faces, sizeof(_IBSP_FACE), _iNumFaces, file);

	for(int i = 0; i < _iNumFaces; i++)
	{
		_OBSP_FACE tFace;
		tFace.NumVerts = faces[i].NumVertex;
		tFace.Texture = faces[i].Texture;
		tFace.Type = faces[i].Type;
		tFace.Vertex = faces[i].Vertex;
		tFace.MeshVert = faces[i].MeshVert;
		tFace.NumMeshVerts = faces[i].NumMeshVerts;
		tFace.Size[0] = faces[i].Size[0];
		tFace.Size[1] = faces[i].Size[1];
		tFace.LMIndex = faces[i].LMIndex;
		tFace.LMSize[0] = faces[i].LMSize[0];
		tFace.LMSize[1] = faces[i].LMSize[1];
		tFace.LMStart[0] = faces[i].LMStart[0];
		tFace.LMStart[1] = faces[i].LMStart[1];
		_vFaces[i] = tFace;
	}

	free(faces);
}

void OE::Parsers::BSP::_IBSP_ParseTextures(FILE* file, _IBSP_HEADER *header)
{
	_iNumTextures = header->DirEntries[1].Length / sizeof(_IBSP_TEXTURE);
	_vTextures = (_OBSP_TEXTURE*)malloc(sizeof(_OBSP_TEXTURE)*_iNumTextures);
	_IBSP_TEXTURE *textures = (_IBSP_TEXTURE*)malloc(sizeof(_IBSP_TEXTURE)*_iNumTextures);
	fseek(file, header->DirEntries[1].Offset, SEEK_SET);
	fread((_IBSP_TEXTURE*)textures, sizeof(_IBSP_TEXTURE), _iNumTextures, file);

	for(int i = 0; i < _iNumTextures; i++)
	{		
		std::string tPath = std::string("base/") + std::string(textures[i].Name);

		_vTextures[i].TextureIndex = -1;
		_vTextures[i].Contents = textures[i].Contents;
		_vTextures[i].Flags = textures[i].Flags;

		std::string testExt = tPath + std::string(".png");
		if(OE::Textures::TextureManager::DoesFileExist(testExt.c_str()))
		{
			_vTextures[i].TextureIndex = OE::Textures::TextureManager::LoadTextureFromPath(testExt.c_str());
			continue;
		}

		testExt = tPath + std::string(".tga");
		if(OE::Textures::TextureManager::DoesFileExist(testExt.c_str()))
		{
			_vTextures[i].TextureIndex = OE::Textures::TextureManager::LoadTextureFromPath(testExt.c_str());
			continue;
		}

		testExt = tPath + std::string(".jpeg");
		if(OE::Textures::TextureManager::DoesFileExist(testExt.c_str()))
		{
			_vTextures[i].TextureIndex = OE::Textures::TextureManager::LoadTextureFromPath(testExt.c_str());
			continue;
		}

		testExt = tPath + std::string(".jpg");
		if(OE::Textures::TextureManager::DoesFileExist(testExt.c_str()))
		{
			_vTextures[i].TextureIndex = OE::Textures::TextureManager::LoadTextureFromPath(testExt.c_str());
			continue;
		}
	}

	free(textures);
}

void OE::Parsers::BSP::_IBSP_ParseMeshVerts(FILE *file, _IBSP_HEADER *header)
{
	_iNumMeshVerts = header->DirEntries[11].Length / sizeof(_IBSP_MESHVERT);
	_vMeshVerts = (_OBSP_MESHVERT*)malloc(sizeof(_OBSP_MESHVERT)*_iNumMeshVerts);
	_IBSP_MESHVERT *meshVerts = (_IBSP_MESHVERT*)malloc(sizeof(_IBSP_MESHVERT)*_iNumMeshVerts);
	fseek(file, header->DirEntries[11].Offset, SEEK_SET);
	fread((_IBSP_MESHVERT*)meshVerts, sizeof(_IBSP_MESHVERT), _iNumMeshVerts, file);

	for(int i = 0; i < _iNumMeshVerts; i++)
	{
		_OBSP_MESHVERT t;
		t.Offset = meshVerts[i].Offset;
		_vMeshVerts[i] = t;
	}

	free(meshVerts);
}

void OE::Parsers::BSP::_IBSP_ParseLightmaps(FILE *file, _IBSP_HEADER *header)
{
	_iNumLightMaps = header->DirEntries[14].Length / sizeof(_IBSP_LIGHTMAP);
	_vLightMaps = (_OBSP_LIGHTMAP*)malloc(sizeof(_OBSP_LIGHTMAP)*_iNumLightMaps);
	_IBSP_LIGHTMAP *lightMaps = (_IBSP_LIGHTMAP*)malloc(sizeof(_IBSP_LIGHTMAP)*_iNumLightMaps);
	fseek(file, header->DirEntries[14].Offset, SEEK_SET);
	fread((_IBSP_LIGHTMAP*)lightMaps, sizeof(_IBSP_LIGHTMAP), _iNumLightMaps, file);

	for(int i = 0;  i < _iNumLightMaps; i++)
	{
		_OBSP_LIGHTMAP t;
		for(int x = 0; x < 128; x++)
		{
			for(int y = 0; y < 128; y++)
			{
				for(int c = 0; c < 3; c++)
				{
					t.map[x][y][c] = lightMaps[i].map[x][y][c];
				}
			}
		}

		_vLightMaps[i] = t;
	}

	for(int i = 0; i < _iNumLightMaps; i++)
	{
		_vLightMaps[i].TextureIndex = OE::Textures::TextureManager::LoadTextureFromRaw(&_vLightMaps[i].map[0][0][0]);
	}


	free(lightMaps);
}

const int OE::Parsers::BSP::ParseIBSP(const char* path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");

	_IBSP_HEADER *pHeader = (_IBSP_HEADER*)malloc(sizeof(_IBSP_HEADER));
	fread((_IBSP_HEADER*)pHeader,sizeof(_IBSP_HEADER),1, pFile);

	_IBSP_ParseVertices(pFile, pHeader);
	_IBSP_ParseFaces(pFile, pHeader);
	_IBSP_ParseTextures(pFile, pHeader);
	_IBSP_ParseMeshVerts(pFile, pHeader);
	_IBSP_ParseLightmaps(pFile, pHeader);

	fclose(pFile);

	free(pHeader);
	return 0;
}

//	Ignore VBSP implementation for now.
const int OE::Parsers::BSP::ParseVBSP(const char* path)
{
	return 0;
}

void OE::Parsers::BSP::DebugRender()
{
	if(_iBSPType == BSP_TYPE_IBSP)
	{
		for(int i = 0; i < _iNumFaces; i++)
		{
			if(_vFaces[i].Type != 1 && _vFaces[i].Type != 3)
			{
				continue;
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 1);
			glEnable(GL_TEXTURE_2D);

			int textureMapNumLevels = 1;
			int lightMapNumLevels = 1;
			GLint textureMapTexID = -1;
			GLint lightMapTexID = -1;

			int contentFlags = _vTextures[_vFaces[i].Texture].Contents;
			int surfaceFlags = _vTextures[_vFaces[i].Texture].Flags;

			if(surfaceFlags & 0x80)
				continue;

			if(surfaceFlags & 0x200)
				continue;

			if(surfaceFlags & 0x4)
				continue;

			//	Bind Light Map
			if(_vFaces[i].LMIndex > -1)
			{
				lightMapTexID = OE::Textures::TextureManager::GetTexturesID(_vLightMaps[_vFaces[i].LMIndex].TextureIndex);
				lightMapNumLevels = OE::Textures::TextureManager::GetTexturesNumLevels(_vLightMaps[_vFaces[i].LMIndex].TextureIndex);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, lightMapTexID);
				glEnable(GL_TEXTURE_2D);
			}

			//	Bind Texture Map
			if(_vTextures[_vFaces[i].Texture].TextureIndex > -1)
			{
				textureMapTexID = OE::Textures::TextureManager::GetTexturesID(_vTextures[_vFaces[i].Texture].TextureIndex);
				textureMapNumLevels = OE::Textures::TextureManager::GetTexturesNumLevels(_vTextures[_vFaces[i].Texture].TextureIndex);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureMapTexID);
				glEnable(GL_TEXTURE_2D);
			}

			if(textureMapNumLevels > 1 || textureMapTexID == -1)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}

			if(_vFaces[i].Type == 1)
			{
				glBegin(GL_POLYGON);
				for(int j = _vFaces[i].Vertex; j < _vFaces[i].Vertex + _vFaces[i].NumVerts; j++)
				{
					
					glMultiTexCoord2f(GL_TEXTURE0, _vVertices[j].TexCoord[0][0], _vVertices[j].TexCoord[0][1]);

					if(lightMapTexID>-1)
						glMultiTexCoord2f(GL_TEXTURE1, _vVertices[j].TexCoord[1][0], _vVertices[j].TexCoord[1][1]);
					glColor4f(_vVertices[j].Color[0], _vVertices[j].Color[1], _vVertices[j].Color[2], _vVertices[j].Color[3]);
					glVertex3f(_vVertices[j].Position[0], _vVertices[j].Position[1], _vVertices[j].Position[2]);
				}
				glEnd();
			}

			if(_vFaces[i].Type == 3)
			{
				float x, y, z;
				glBegin(GL_TRIANGLES);
				for(int j = _vFaces[i].MeshVert; j < _vFaces[i].MeshVert + _vFaces[i].NumMeshVerts; j++)
				{
					x = _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].Position[0];
					y = _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].Position[1];
					z = _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].Position[2];
					glMultiTexCoord2f(GL_TEXTURE0,_vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].TexCoord[0][0], _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].TexCoord[0][1]);
					if(lightMapTexID > 0)
					{
						glMultiTexCoord2f(GL_TEXTURE1, _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].TexCoord[1][0], _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].TexCoord[1][1]);
					}
					glVertex3f(x, y, z);
				}
				glEnd();
			}
		}
		glDisable(GL_TEXTURE_2D);
	}
}
