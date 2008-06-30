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
#include "Engine/GLFW/glfw.h"

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

	if( extensions.find_first_of("GL_EXT_texture_filter_anisotropic") != -1 )
	{
		std::cout << "Anisotropic supported on your video card!" << std::endl;
	}

	if( extensions.find_first_of("GL_ARB_multitexture") != -1 )
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
		tVertex.Normal[1] = vertices[i].Normal[2];
		tVertex.Normal[2] = -vertices[i].Normal[1];

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
	_vRenderedFaces = (bool*)malloc(sizeof(bool)*_iNumFaces);
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

void OE::Parsers::BSP::_IBSP_ParseVisData(FILE *file, _IBSP_HEADER *header)
{
	_iNumPlanes = header->DirEntries[2].Length / sizeof(_IBSP_PLANE);
	_vPlanes = (_OBSP_PLANE*)malloc(sizeof(_OBSP_PLANE)*_iNumPlanes);
	_IBSP_PLANE *planes = (_IBSP_PLANE*)malloc(sizeof(_IBSP_PLANE)*_iNumPlanes);
	fseek(file, header->DirEntries[2].Offset, SEEK_SET);
	fread((_IBSP_PLANE*)planes, sizeof(_IBSP_PLANE), _iNumPlanes, file);

	for(int i = 0; i < _iNumPlanes; i++)
	{
		_vPlanes[i].Distance = planes[i].Distance;
		//_vPlanes[i].Normal.x = planes[i].Normal.x;
		//_vPlanes[i].Normal.y = planes[i].Normal.y;
		//_vPlanes[i].Normal.z = planes[i].Normal.z;
		_vPlanes[i].Normal.x = planes[i].Normal.x;
		_vPlanes[i].Normal.y = planes[i].Normal.z;
		_vPlanes[i].Normal.z = -planes[i].Normal.y;
	}

	free(planes);

	_iNumNodes = header->DirEntries[3].Length / sizeof(_IBSP_NODE);
	_vNodes = (_OBSP_NODE*)malloc(sizeof(_OBSP_NODE)*_iNumNodes);
	_IBSP_NODE *nodes = (_IBSP_NODE*)malloc(sizeof(_IBSP_NODE)*_iNumNodes);
	fseek(file, header->DirEntries[3].Offset, SEEK_SET);
	fread((_IBSP_NODE*)nodes, sizeof(_IBSP_NODE), _iNumNodes, file);

	for(int i = 0; i < _iNumNodes; i++)
	{
		_vNodes[i].Children[0] = nodes[i].Children[0];
		_vNodes[i].Children[1] = nodes[i].Children[1];
		_vNodes[i].Mins[0] = nodes[i].Mins[0];
		_vNodes[i].Mins[1] = nodes[i].Mins[1];
		_vNodes[i].Mins[2] = nodes[i].Mins[2];
		_vNodes[i].Maxs[0] = nodes[i].Maxs[0];
		_vNodes[i].Maxs[1] = nodes[i].Maxs[1];
		_vNodes[i].Maxs[2] = nodes[i].Maxs[2];
		_vNodes[i].Plane = nodes[i].Plane;
	}

	free(nodes);

	_iNumLeafs = header->DirEntries[4].Length / sizeof(_IBSP_LEAF);
	_vLeafs = (_OBSP_LEAF*)malloc(sizeof(_OBSP_LEAF)*_iNumLeafs);
	_IBSP_LEAF *leafs = (_IBSP_LEAF*)malloc(sizeof(_IBSP_LEAF)*_iNumLeafs);
	fseek(file, header->DirEntries[4].Offset, SEEK_SET);
	fread((_IBSP_LEAF*)leafs, sizeof(_IBSP_LEAF), _iNumLeafs, file);

	for(int i = 0; i < _iNumLeafs; i++)
	{
		_vLeafs[i].Cluster = leafs[i].Cluster;
		_vLeafs[i].Area = leafs[i].Area;
		_vLeafs[i].Mins[0] = leafs[i].Mins[0];
		_vLeafs[i].Mins[1] = leafs[i].Mins[1];
		_vLeafs[i].Mins[2] = leafs[i].Mins[2];
		_vLeafs[i].Maxs[0] = leafs[i].Maxs[0];
		_vLeafs[i].Maxs[1] = leafs[i].Maxs[1];
		_vLeafs[i].Maxs[2] = leafs[i].Maxs[2];
		_vLeafs[i].LeafFace = leafs[i].LeafFace;
		_vLeafs[i].NumLeafFaces = leafs[i].NumLeafFaces;
		_vLeafs[i].LeafBrush = leafs[i].LeafBrush;
		_vLeafs[i].NumLeafBrushes = leafs[i].NumLeafBrushes;
	}

	free(leafs);

	_iNumLeafFaces = header->DirEntries[5].Length / sizeof(_IBSP_LEAFFACE);
	_vLeafFaces = (_OBSP_LEAFFACE*)malloc(sizeof(_OBSP_LEAFFACE)*_iNumLeafFaces);
	_IBSP_LEAFFACE *leafFaces = (_IBSP_LEAFFACE*)malloc(sizeof(_IBSP_LEAFFACE)*_iNumLeafFaces);
	fseek(file, header->DirEntries[5].Offset, SEEK_SET);
	fread((_IBSP_LEAFFACE*)leafFaces, sizeof(_IBSP_LEAFFACE), _iNumLeafFaces, file);

	for(int i = 0; i < _iNumLeafFaces; i++)
	{
		_vLeafFaces[i].Face = leafFaces[i].Face;
	}

	free(leafFaces);

	_iNumLeafBrushes = header->DirEntries[6].Length / sizeof(_IBSP_LEAFBRUSH);
	_vLeafBrushes = (_OBSP_LEAFBRUSH*)malloc(sizeof(_IBSP_LEAFBRUSH)*_iNumLeafBrushes);
	_IBSP_LEAFBRUSH *leafBrushes = (_IBSP_LEAFBRUSH*)malloc(sizeof(_IBSP_LEAFBRUSH)*_iNumLeafBrushes);
	fseek(file, header->DirEntries[6].Offset, SEEK_SET);
	fread((_IBSP_LEAFBRUSH*)leafBrushes, sizeof(_IBSP_LEAFBRUSH), _iNumLeafBrushes, file);

	for(int i = 0; i < _iNumLeafBrushes; i++)
	{
		_vLeafBrushes[i].Brush = leafBrushes[i].Brush;
	}

	free(leafBrushes);

	if(header->DirEntries[16].Length > 0)
	{
		fseek(file, header->DirEntries[16].Offset, SEEK_SET);
		fread(&_iNumVecs, sizeof(int), 1, file);
		fread(&_iSizeOfVecs, sizeof(int), 1, file);
		_vVecs = (unsigned char*)malloc(sizeof(unsigned char) * _iNumVecs * _iSizeOfVecs);
		fread((unsigned char*)_vVecs, sizeof(unsigned char), _iNumVecs * _iSizeOfVecs,file);
	}

	_iNumBrushes = header->DirEntries[8].Length / sizeof(_IBSP_BRUSH);
	_vBrushes = (_OBSP_BRUSH*)malloc(sizeof(_IBSP_BRUSH)*_iNumBrushes);
	_IBSP_BRUSH* brushes = (_IBSP_BRUSH*)malloc(sizeof(_IBSP_BRUSH)*_iNumBrushes);
	fseek(file, header->DirEntries[8].Offset, SEEK_SET);
	fread((_IBSP_BRUSH*)brushes, sizeof(_IBSP_BRUSH), _iNumBrushes, file);

	for(int i = 0; i < _iNumBrushes; i++)
	{
		_vBrushes[i].BrushSide = brushes[i].BrushSide;
		_vBrushes[i].NumBrushSides = brushes[i].NumBrushSides;
		_vBrushes[i].Texture = brushes[i].Texture;
	}

	free(brushes);

	_iNumBrushSides = header->DirEntries[9].Length / sizeof(_IBSP_BRUSHSIDE);
	_vBrushSides = (_OBSP_BRUSHSIDE*)malloc(sizeof(_IBSP_BRUSHSIDE)*_iNumBrushSides);
	_IBSP_BRUSHSIDE* brushSides = (_IBSP_BRUSHSIDE*)malloc(sizeof(_IBSP_BRUSHSIDE)*_iNumBrushSides);
	fseek(file, header->DirEntries[9].Offset, SEEK_SET);
	fread((_IBSP_BRUSHSIDE*)brushSides, sizeof(_IBSP_BRUSHSIDE), _iNumBrushSides, file);

	for(int i = 0; i < _iNumBrushSides; i++)
	{
		_vBrushSides[i].Plane = brushSides[i].Plane;
		_vBrushSides[i].Texture = brushSides[i].Texture;
	}
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
	_IBSP_ParseVisData(pFile, pHeader);
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

void OE::Parsers::BSP::RenderFace(int index)
{
	if(_vRenderedFaces[index])
		return;

	if(_vFaces[index].Type != 1 && _vFaces[index].Type != 3)
	{
		return;
	}

	int textureMapNumLevels = 1;
	int lightMapNumLevels = 1;
	GLint textureMapTexID = -1;
	GLint lightMapTexID = -1;

	int contentFlags = _vTextures[_vFaces[index].Texture].Contents;
	int surfaceFlags = _vTextures[_vFaces[index].Texture].Flags;

	if(surfaceFlags & 0x80)
		return;

	if(surfaceFlags & 0x200)
		return;

	if(surfaceFlags & 0x4)
		return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);
	glEnable(GL_TEXTURE_2D);

	//	Bind Light Map
	if(_vFaces[index].LMIndex > -1)
	{
		lightMapTexID = OE::Textures::TextureManager::GetTexturesID(_vLightMaps[_vFaces[index].LMIndex].TextureIndex);
		lightMapNumLevels = OE::Textures::TextureManager::GetTexturesNumLevels(_vLightMaps[_vFaces[index].LMIndex].TextureIndex);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lightMapTexID);
		glEnable(GL_TEXTURE_2D);
	}

	//	Bind Texture Map
	if(_vTextures[_vFaces[index].Texture].TextureIndex > -1)
	{
		textureMapTexID = OE::Textures::TextureManager::GetTexturesID(_vTextures[_vFaces[index].Texture].TextureIndex);
		textureMapNumLevels = OE::Textures::TextureManager::GetTexturesNumLevels(_vTextures[_vFaces[index].Texture].TextureIndex);

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

	if(_vFaces[index].Type == 1)
	{
		glBegin(GL_POLYGON);
		for(int j = _vFaces[index].Vertex; j < _vFaces[index].Vertex + _vFaces[index].NumVerts; j++)
		{
			glMultiTexCoord2f(GL_TEXTURE0, _vVertices[j].TexCoord[0][0], _vVertices[j].TexCoord[0][1]);

			if(lightMapTexID>-1)
				glMultiTexCoord2f(GL_TEXTURE1, _vVertices[j].TexCoord[1][0], _vVertices[j].TexCoord[1][1]);
			glColor4f(_vVertices[j].Color[0], _vVertices[j].Color[1], _vVertices[j].Color[2], _vVertices[j].Color[3]);
			glVertex3f(_vVertices[j].Position[0], _vVertices[j].Position[1], _vVertices[j].Position[2]);
		}
		glEnd();
	}

	if(_vFaces[index].Type == 3)
	{
		float x, y, z;
		glBegin(GL_TRIANGLES);
		for(int j = _vFaces[index].MeshVert; j < _vFaces[index].MeshVert + _vFaces[index].NumMeshVerts; j++)
		{
			x = _vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].Position[0];
			y = _vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].Position[1];
			z = _vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].Position[2];
			glMultiTexCoord2f(GL_TEXTURE0,_vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].TexCoord[0][0], _vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].TexCoord[0][1]);
			if(lightMapTexID > 0)
			{
				glMultiTexCoord2f(GL_TEXTURE1, _vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].TexCoord[1][0], _vVertices[_vFaces[index].Vertex + _vMeshVerts[j].Offset].TexCoord[1][1]);
			}
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	_vRenderedFaces[index] = true;
}

void OE::Parsers::BSP::RenderLeaf(int index)
{
	int x = _vLeafs[index].Cluster;

	for(int i = 0; i < _iNumLeafs; i++)
	{
		if(i==index)
			continue;
		if(x>0)
		{
			bool visible = false;
			if(i==index)
				continue;
			int y = _vLeafs[i].Cluster;
			if(_iNumVecs > 0)
			{
				visible = _vVecs[x * _iSizeOfVecs + y / 8] & (1 << y % 8);
				if(visible)
				{
					for(int j = _vLeafs[i].LeafFace; j < _vLeafs[i].LeafFace + _vLeafs[i].NumLeafFaces; j++)
					{
						RenderFace(_vLeafFaces[j].Face);
					}
				}
			}
			else
			{
				for(int j = _vLeafs[i].LeafFace; j < _vLeafs[i].LeafFace + _vLeafs[i].NumLeafFaces; j++)
				{
					RenderFace(_vLeafFaces[j].Face);
				}
			}
		}
		else
		{
			for(int j = _vLeafs[i].LeafFace; j < _vLeafs[i].LeafFace + _vLeafs[i].NumLeafFaces; j++)
			{
				RenderFace(_vLeafFaces[j].Face);
			}
		}
	}	

	for(int i = _vLeafs[index].LeafFace; i < _vLeafs[index].LeafFace + _vLeafs[index].NumLeafFaces; i++)
	{
		RenderFace(_vLeafFaces[i].Face);
	}
}

void OE::Parsers::BSP::TraverseBSPTree(OE::Cameras::FPSCamera* fpsCamera, int nodeIndex)
{
	double dotProd = fpsCamera->GetPos().DotProduct(_vPlanes[_vNodes[nodeIndex].Plane].Normal);
	double distance = _vPlanes[_vNodes[nodeIndex].Plane].Distance;
	dotProd -= distance;
	if(dotProd > 0)
	{
		int index = _vNodes[nodeIndex].Children[0];

		//	Is it a leaf?
		if(index < 0)
		{
			index*=-1;
			index--;

			RenderLeaf(index);
		}
		else
		{
			TraverseBSPTree(fpsCamera, index);
		}
	}
	else
	{
		int index = _vNodes[nodeIndex].Children[1];

		//	Is it a leaf?
		if(index < 0)
		{
			index*=-1;
			index--;

			RenderLeaf(index);
		}
		else
		{
			TraverseBSPTree(fpsCamera, index);
		}
	}
}

void OE::Parsers::BSP::DebugRender(float dt, OE::Cameras::FPSCamera* fpsCamera)
{
	if(_iBSPType == BSP_TYPE_IBSP)
	{
		TraverseBSPTree(fpsCamera, 0);
		memset(_vRenderedFaces, 0, sizeof(bool) * _iNumFaces);
		fpsCamera->Update(dt);
	}
}
