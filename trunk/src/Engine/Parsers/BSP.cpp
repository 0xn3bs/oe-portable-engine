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
	ParseIBSP(path);
	return 0;
}

const int OE::Parsers::BSP::ParseIBSP(const char* path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");
	_IBSP_HEADER *header = (_IBSP_HEADER*)malloc(sizeof(_IBSP_HEADER));
	fread((_IBSP_HEADER*)header,sizeof(_IBSP_HEADER),1, pFile);

	_iNumVertices = header->DirEntries[10].Length / sizeof(_IBSP_VERTEX);
	_vVertices = (_OBSP_VERTEX*)malloc(sizeof(_OBSP_VERTEX)*_iNumVertices);
	_IBSP_VERTEX *vertices = (_IBSP_VERTEX*)malloc(sizeof(_IBSP_VERTEX)*_iNumVertices);
	fseek(pFile, header->DirEntries[10].Offset, SEEK_SET);
	fread((_IBSP_VERTEX*)vertices, sizeof(_IBSP_VERTEX), _iNumVertices, pFile);

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
		_vVertices[i] = tVertex;
	}

	_iNumFaces = header->DirEntries[13].Length / sizeof(_IBSP_FACE);
	_vFaces = (_OBSP_FACE*)malloc(sizeof(_OBSP_FACE)*_iNumFaces);
	_IBSP_FACE *faces = (_IBSP_FACE*)malloc(sizeof(_IBSP_FACE)*_iNumFaces);
	fseek(pFile, header->DirEntries[13].Offset, SEEK_SET);
	fread((_IBSP_FACE*)faces, sizeof(_IBSP_FACE), _iNumFaces, pFile);

	for(int i = 0; i < _iNumFaces; i++)
	{
		_OBSP_FACE tFace;
		tFace.NumVerts = faces[i].NumVertex;
		tFace.Texture = faces[i].Texture;
		tFace.Type = faces[i].Type;
		tFace.Vertex = faces[i].Vertex;
		tFace.MeshVert = faces[i].MeshVert;
		tFace.NumMeshVerts = faces[i].NumMeshVerts;
		_vFaces[i] = tFace;
	}

	_iNumTextures = header->DirEntries[1].Length / sizeof(_IBSP_TEXTURE);
	_vTextures = (GLint*)malloc(sizeof(GLint)*_iNumTextures);
	_IBSP_TEXTURE *textures = (_IBSP_TEXTURE*)malloc(sizeof(_IBSP_TEXTURE)*_iNumTextures);
	fseek(pFile, header->DirEntries[1].Offset, SEEK_SET);
	fread((_IBSP_TEXTURE*)textures, sizeof(_IBSP_TEXTURE), _iNumTextures, pFile);

	fclose(pFile);
	for(int i = 0; i < _iNumTextures; i++)
	{
		_vTextures[i] = OE::Textures::TextureManager::LoadTexture(textures[i].Name);
	}
	pFile = fopen(path, "rb");

	//	TODO: MESH VERTS
	_iNumMeshVerts = header->DirEntries[11].Length / sizeof(_IBSP_MESHVERT);
	_vMeshVerts = (_OBSP_MESHVERT*)malloc(sizeof(_OBSP_MESHVERT)*_iNumMeshVerts);
	_IBSP_MESHVERT *meshVerts = (_IBSP_MESHVERT*)malloc(sizeof(_IBSP_MESHVERT)*_iNumMeshVerts);
	fseek(pFile, header->DirEntries[11].Offset, SEEK_SET);
	fread((_IBSP_MESHVERT*)meshVerts, sizeof(_IBSP_MESHVERT), _iNumMeshVerts, pFile);

	for(int i = 0; i < _iNumMeshVerts; i++)
	{
		_OBSP_MESHVERT t;
		t.Offset = meshVerts[i].Offset;
		_vMeshVerts[i] = t;
	}

	free(meshVerts);
	free(textures);
	free(vertices);
	free(faces);
	free(header);
	return 0;
}


//	Ignore VBSP implementation for now.
const int OE::Parsers::BSP::ParseVBSP(const char* path)
{
	/*
	_VHEADER *header = (_VHEADER*)malloc(sizeof(_VHEADER));
	fread((_VHEADER*)header,sizeof(_VHEADER),1, pFile);

	int _iNumVertices = header->lumps[3].filelen / sizeof(_VVERTEX);
	_VVERTEX *vertices = (_VVERTEX*)malloc(sizeof(_VVERTEX)*_iNumVertices);
	fseek(pFile, header->lumps[3].fileofs, SEEK_SET);
	fread((_VVERTEX*)vertices, sizeof(_VVERTEX), _iNumVertices, pFile);

	int _iNumFaces = header->lumps[7].filelen / sizeof(_VFACE);
	_VFACE *faces = (_VFACE*)malloc(sizeof(_VFACE)*_iNumFaces);
	fseek(pFile, header->lumps[7].fileofs, SEEK_SET);
	fread((_VFACE*)faces, sizeof(_VFACE), _iNumFaces, pFile);

	int nEdges = header->lumps[12].filelen / sizeof(_VEDGE);
	_VEDGE *edges = (_VEDGE*)malloc(sizeof(_VEDGE)*nEdges);
	fseek(pFile, header->lumps[12].fileofs, SEEK_SET);
	fread((_VEDGE*)edges, sizeof(_VEDGE), nEdges, pFile);

	int nTexInfos = header->lumps[6].filelen / sizeof(_VTEXINFO);
	_VEDGE *texinfos = (_VEDGE*)malloc(sizeof(_VTEXINFO)*nTexInfos);
	fseek(pFile, header->lumps[6].fileofs, SEEK_SET);
	fread((_VTEXINFO*)texinfos, sizeof(_VTEXINFO), nTexInfos, pFile);

	for(int i = 0; i < nEdges; i++)
	{
	_OEDGE tEdge;
	tEdge.v[0] = edges[i].v[0];
	tEdge.v[1] = edges[i].v[1];
	_vEdges.push_back(tEdge);
	}

	for(int i = 0; i < _iNumVertices; i++)
	{
	_OVERTEX tVertex;

	tVertex.position[0] = vertices[i].point.x;
	tVertex.position[1] = vertices[i].point.z;
	tVertex.position[2] = -vertices[i].point.y;

	_vVertices.push_back(tVertex);
	}

	for(int i = 0; i < _iNumFaces; i++)
	{
	for(int j = faces[i].firstedge; j < faces[i].firstedge + faces[i].numedges; j++)
	{
	_OVERTEX tVertA = _vVertices[_vEdges[j].v[0]];
	_OVERTEX tVertB = _vVertices[_vEdges[j].v[1]];
	std::cout << tVertA.position[0] << " " << tVertA.position[1] << " " << tVertA.position[2] << std::endl;
	std::cout << tVertB.position[0] << " " << tVertB.position[1] << " " << tVertB.position[2] << std::endl;
	std::cout << j << " " << faces[i].firstedge + faces[i].numedges << std::endl;
	}
	//
	//_OVERTEX tVertA = _vVertices[_vEdges[i].v[0]];
	//_OVERTEX tVertB = _vVertices[_vEdges[i].v[1]];
	//std::cout << tVertA.position[0] << " " << tVertA.position[1] << " " << tVertA.position[2] << std::endl;
	//std::cout << tVertB.position[0] << " " << tVertB.position[1] << " " << tVertB.position[2] << std::endl;
	//_OFACE tFace;
	}

	free(header);
	free(vertices);
	free(faces);
	free(edges);
	free(texinfos);
	*/
	return 0;
}

void OE::Parsers::BSP::DebugRender()
{
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if(_iBSPType == BSP_TYPE_IBSP)
	{
		for(int i = 0; i < _iNumFaces; i++)
		{
			if(_vFaces[i].Type != 1 && _vFaces[i].Type != 3)
			{
				continue;
			}

			if(_vTextures[_vFaces[i].Texture]>0)
			{
				glBindTexture(GL_TEXTURE_2D, _vTextures[_vFaces[i].Texture]);
			}
			else
				glBindTexture(GL_TEXTURE_2D, 1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if(_vFaces[i].Type == 1)
			{
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glBegin(GL_POLYGON);
				for(int j = _vFaces[i].Vertex; j < _vFaces[i].Vertex + _vFaces[i].NumVerts; j++)
				{
					glTexCoord2f(_vVertices[j].TexCoord[0][0], _vVertices[j].TexCoord[0][1]);
					glVertex3f(_vVertices[j].Position[0], _vVertices[j].Position[1], _vVertices[j].Position[2]);
				}
				glEnd();
			}

			if(_vFaces[i].Type == 3)
			{
				float x, y, z;

				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glBegin(GL_TRIANGLES);
				for(int j = _vFaces[i].MeshVert; j < _vFaces[i].MeshVert + _vFaces[i].NumMeshVerts; j++)
				{
					x = _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].Position[0];
					y = _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].Position[1];
					z = _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].Position[2];
					glTexCoord2f(_vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].TexCoord[0][0], _vVertices[_vFaces[i].Vertex + _vMeshVerts[j].Offset].TexCoord[0][1]);
					glVertex3f(x, y, z);
				}
				glEnd();
			}
		}
		glDisable(GL_BLEND);
	}
}
