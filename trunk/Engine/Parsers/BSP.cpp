#include "BSP.h"
#include <GL/glfw.h>
#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'V')
const int Odorless::Engine::Parsers::BSP::ParseBSP(const char *path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");

	/*
	_IHEADER *header = (_IHEADER*)malloc(sizeof(_IHEADER));
	fread((_IHEADER*)header,sizeof(_IHEADER),1, pFile);
	fseek(pFile, 0, SEEK_SET);
	if(strcmp(header->magic, "IBSP.") == 0)
	ParseIBSP(pFile);
	else
	if(strcmp(header->magic, "VBSP") == 0)
	ParseVBSP(pFile);
	*/

	//	Read the first 4 bytes for the magic number.
	char* magic = (char*)malloc(sizeof(char)*5);
	fread((char*)magic,sizeof(magic),1, pFile);
	magic[4] = '\0';
	fseek(pFile, 0, SEEK_SET);

	if(strcmp(magic, "IBSP") == 0)
	{
		_iBSPType = BSP_TYPE_IBSP;
		ParseIBSP(pFile);
	}
	else
		if(strcmp(magic, "VBSP") == 0)
		{
			_iBSPType = BSP_TYPE_VBSP;
			ParseVBSP(pFile);
		}

		free(magic);
		fclose(pFile);
		return 0;
}

const int Odorless::Engine::Parsers::BSP::ParseIBSP(FILE* pFile)
{
	_IHEADER *header = (_IHEADER*)malloc(sizeof(_IHEADER));
	fread((_IHEADER*)header,sizeof(_IHEADER),1, pFile);

	int nVertices = header->direntries[10].length / sizeof(_IVERTEX);
	_IVERTEX *vertices = (_IVERTEX*)malloc(sizeof(_IVERTEX)*nVertices);
	fseek(pFile, header->direntries[10].offset, SEEK_SET);
	fread((_IVERTEX*)vertices, sizeof(_IVERTEX), nVertices, pFile);

	for(int i = 0; i < nVertices; i++)
	{
		int y = vertices[i].position[2];
		int z = -vertices[i].position[1];
		_OVERTEX tVertex;
		tVertex.position[0] = vertices[i].position[0];
		tVertex.position[1] = y;
		tVertex.position[2] = z;

		tVertex.texcoord[0][0] = vertices[i].texcoord[0][0];
		tVertex.texcoord[0][1] = vertices[i].texcoord[0][1];

		tVertex.texcoord[1][0] = vertices[i].texcoord[1][0];
		tVertex.texcoord[1][1] = vertices[i].texcoord[1][1];

		tVertex.normal[0] = vertices[i].normal[0];
		tVertex.normal[1] = vertices[i].normal[1];
		tVertex.normal[2] = vertices[i].normal[2];
		_vVertices.push_back(tVertex);
	}

	int nFaces = header->direntries[13].length / sizeof(_IFACE);
	_IFACE *faces = (_IFACE*)malloc(sizeof(_IFACE)*nFaces);
	fseek(pFile, header->direntries[13].offset, SEEK_SET);
	fread((_IFACE*)faces, sizeof(_IFACE), nFaces, pFile);

	for(int i = 0; i < nFaces; i++)
	{
		_OFACE tFace;
		tFace.n_vertexes = faces[i].n_vertexes;
		tFace.texture = faces[i].texture;
		tFace.type = faces[i].type;
		tFace.vertex = faces[i].vertex;
		tFace.meshvert = faces[i].meshvert;
		tFace.n_meshverts = faces[i].n_meshverts;
		_vFaces.push_back(tFace);
	}

	int nTextures = header->direntries[1].length / sizeof(_ITEXTURE);
	_ITEXTURE *textures = (_ITEXTURE*)malloc(sizeof(_ITEXTURE)*nTextures);
	fseek(pFile, header->direntries[1].offset, SEEK_SET);
	fread((_ITEXTURE*)textures, sizeof(_ITEXTURE), nTextures, pFile);

	for(int i = 0; i < nTextures; i++)
	{
		char name[100];

		strcpy (name,"base/");
		strcat (name,textures[i].name);
		strcat (name,".tga");
		GLint texIndex = -1;
		if(texIndex = Odorless::Engine::Textures::TextureManager::LoadTexture(name))
		{
			_vTextures.push_back(texIndex);
			continue;
		}
		strcpy (name,"base/");
		strcat (name,textures[i].name);
		strcat (name,".jpg");
		if(texIndex = Odorless::Engine::Textures::TextureManager::LoadTexture(name))
		{
			_vTextures.push_back(texIndex);
			continue;
		}
	}

	//	TODO: MESH VERTS
	int nMeshVerts = header->direntries[11].length / sizeof(_IMESHVERT);
	_IMESHVERT *meshVerts = (_IMESHVERT*)malloc(sizeof(_IMESHVERT)*nMeshVerts);
	fseek(pFile, header->direntries[11].offset, SEEK_SET);
	fread((_IMESHVERT*)meshVerts, sizeof(_IMESHVERT), nMeshVerts, pFile);

	for(int i = 0; i < nMeshVerts; i++)
	{
		_OMESHVERT t;
		t.offset = meshVerts[i].offset;
		_vMeshVerts.push_back(t);
	}

	free(meshVerts);
	free(textures);
	free(vertices);
	free(faces);
	free(header);
	return 0;
}


//	Ignore VBSP implementation for now.
const int Odorless::Engine::Parsers::BSP::ParseVBSP(FILE* pFile)
{
	/*
	_VHEADER *header = (_VHEADER*)malloc(sizeof(_VHEADER));
	fread((_VHEADER*)header,sizeof(_VHEADER),1, pFile);

	int nVertices = header->lumps[3].filelen / sizeof(_VVERTEX);
	_VVERTEX *vertices = (_VVERTEX*)malloc(sizeof(_VVERTEX)*nVertices);
	fseek(pFile, header->lumps[3].fileofs, SEEK_SET);
	fread((_VVERTEX*)vertices, sizeof(_VVERTEX), nVertices, pFile);

	int nFaces = header->lumps[7].filelen / sizeof(_VFACE);
	_VFACE *faces = (_VFACE*)malloc(sizeof(_VFACE)*nFaces);
	fseek(pFile, header->lumps[7].fileofs, SEEK_SET);
	fread((_VFACE*)faces, sizeof(_VFACE), nFaces, pFile);

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

	for(int i = 0; i < nVertices; i++)
	{
	_OVERTEX tVertex;

	tVertex.position[0] = vertices[i].point.x;
	tVertex.position[1] = vertices[i].point.z;
	tVertex.position[2] = -vertices[i].point.y;

	_vVertices.push_back(tVertex);
	}

	for(int i = 0; i < nFaces; i++)
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

void Odorless::Engine::Parsers::BSP::DebugRender()
{
	//glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if(_iBSPType == BSP_TYPE_IBSP)
	{
		for(int i = 0; i < _vFaces.size(); i++)
		{
			if(_vFaces[i].type != 1 && _vFaces[i].type != 3)
			{
				continue;
			}

			//std::cout << _vTextures[_vFaces[i].texture] << std::endl;
			/*if(_vTextures[_vFaces[i].texture]>0)
			{
			glBindTexture(GL_TEXTURE_2D, _vTextures[_vFaces[i].texture]);
			}
			else
			glBindTexture(GL_TEXTURE_2D, 1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

			if(_vFaces[i].type == 1)
			{
				if(_vTextures[_vFaces[i].texture]>0)
				{
					glBindTexture(GL_TEXTURE_2D, _vTextures[_vFaces[i].texture]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBegin(GL_POLYGON);
					for(int j = _vFaces[i].vertex; j < _vFaces[i].vertex + _vFaces[i].n_vertexes; j++)
					{
						glTexCoord2f(_vVertices[j].texcoord[0][0], _vVertices[j].texcoord[0][1]);
						glVertex3f(_vVertices[j].position[0], _vVertices[j].position[1], _vVertices[j].position[2]);
					}
					glEnd();
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, 1);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
					glBegin(GL_LINES);
					for(int j = _vFaces[i].vertex; j < _vFaces[i].vertex + _vFaces[i].n_vertexes; j++)
					{
						glVertex3f(_vVertices[j].position[0], _vVertices[j].position[1], _vVertices[j].position[2]);
					}
					glEnd();
				}
			}
			else
				if(_vFaces[i].type == 3)
				{
					float x, y, z;

					/*
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBegin(GL_TRIANGLES);
					for(int j = _vFaces[i].meshvert; j < _vFaces[i].meshvert + _vFaces[i].n_meshverts; j++)
					{
						x = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[0];
						y = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[1];
						z = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[2];
						//x = _vVertices[_vMeshVerts[_vFaces[i].meshvert].offset].position[0];
						//y = _vVertices[_vMeshVerts[_vFaces[i].meshvert].offset].position[1];
						//z = _vVertices[_vMeshVerts[_vFaces[i].meshvert].offset].position[2];
						glTexCoord2f(_vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].texcoord[0][0], _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].texcoord[0][1]);
						glVertex3f(x, y, z);
					}
					glEnd();
					*/

					glBindTexture(GL_TEXTURE_2D, 0);
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBegin(GL_LINES);
					for(int j = _vFaces[i].meshvert; j < _vFaces[i].meshvert + _vFaces[i].n_meshverts; j++)
					{
						x = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[0];
						y = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[1];
						z = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[2];
						//x = _vVertices[_vMeshVerts[_vFaces[i].meshvert].offset].position[0];
						//y = _vVertices[_vMeshVerts[_vFaces[i].meshvert].offset].position[1];
						//z = _vVertices[_vMeshVerts[_vFaces[i].meshvert].offset].position[2];
						glVertex3f(x, y, z);
					}
					glEnd();
				}
		}
	}



	/*

	if(_iBSPType == BSP_TYPE_IBSP)
	{
	for(int i = 0; i < _vFaces.size(); i++)
	{
	if(_vFaces[i].type != 1)
	{
	continue;
	}

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//glBindTexture(_vTextures[_vFaces[i].texture], 1);
	glBegin(GL_LINES);
	for(int j = _vFaces[i].vertex; j < _vFaces[i].vertex + _vFaces[i].n_vertexes; j++)
	{
	glTexCoord2f(_vVertices[j].texcoord[0][0], _vVertices[j].texcoord[0][1]);
	glVertex3f(_vVertices[j].position[0], _vVertices[j].position[1], _vVertices[j].position[2]);
	}
	glEnd();
	}
	}*/

	/*
	if(_iBSPType == BSP_TYPE_VBSP)
	{
	glBegin(GL_LINES);
	for(int i = 0; i < _vEdges.size(); i++)
	{
	_OVERTEX tVertA = _vVertices[_vEdges[i].v[0]];
	_OVERTEX tVertB = _vVertices[_vEdges[i].v[1]];

	glVertex3f(tVertA.position[0], tVertA.position[1], tVertA.position[2]);
	glVertex3f(tVertB.position[0], tVertB.position[1], tVertB.position[2]);
	}
	glEnd();
	}
	*/
}