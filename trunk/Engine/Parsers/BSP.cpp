#include "BSP.h"
#include <GL/glfw.h>

#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'V')

const int Odorless::Engine::Parsers::BSP::ParseBSP(const char *path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");

	if(pFile==NULL)
	{
		std::cerr << "Error - Failed to load map \"" << path << "\", no such map!" << std::endl;
		return -1;
	}

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

	_iNumVertices = header->direntries[10].length / sizeof(_IVERTEX);
	_vVertices = (_OVERTEX*)malloc(sizeof(_OVERTEX)*_iNumVertices);
	_IVERTEX *vertices = (_IVERTEX*)malloc(sizeof(_IVERTEX)*_iNumVertices);
	fseek(pFile, header->direntries[10].offset, SEEK_SET);
	fread((_IVERTEX*)vertices, sizeof(_IVERTEX), _iNumVertices, pFile);

	for(int i = 0; i < _iNumVertices; i++)
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
		_vVertices[i] = tVertex;
	}

	_iNumFaces = header->direntries[13].length / sizeof(_IFACE);
	_vFaces = (_OFACE*)malloc(sizeof(_OFACE)*_iNumFaces);
	_IFACE *faces = (_IFACE*)malloc(sizeof(_IFACE)*_iNumFaces);
	fseek(pFile, header->direntries[13].offset, SEEK_SET);
	fread((_IFACE*)faces, sizeof(_IFACE), _iNumFaces, pFile);

	for(int i = 0; i < _iNumFaces; i++)
	{
		_OFACE tFace;
		tFace.n_vertexes = faces[i].n_vertexes;
		tFace.texture = faces[i].texture;
		tFace.type = faces[i].type;
		tFace.vertex = faces[i].vertex;
		tFace.meshvert = faces[i].meshvert;
		tFace.n_meshverts = faces[i].n_meshverts;
		_vFaces[i] = tFace;
	}

	_iNumTextures = header->direntries[1].length / sizeof(_ITEXTURE);
	_vTextures = (GLint*)malloc(sizeof(GLint)*_iNumTextures);
	_ITEXTURE *textures = (_ITEXTURE*)malloc(sizeof(_ITEXTURE)*_iNumTextures);
	fseek(pFile, header->direntries[1].offset, SEEK_SET);
	fread((_ITEXTURE*)textures, sizeof(_ITEXTURE), _iNumTextures, pFile);

	for(int i = 0; i < _iNumTextures; i++)
	{
		char name[100];

		strcpy (name,"base/");
		strcat (name,textures[i].name);
		strcat (name,".tga");
		GLint texIndex = -1;
		if(texIndex = Odorless::Engine::Textures::TextureManager::LoadTexture(name))
		{
			_vTextures[i] = texIndex;
			continue;
		}
		strcpy (name,"base/");
		strcat (name,textures[i].name);
		strcat (name,".jpg");
		if(texIndex = Odorless::Engine::Textures::TextureManager::LoadTexture(name))
		{
			_vTextures[i] = texIndex;
			continue;
		}
	}

	//	TODO: MESH VERTS
	_iNumMeshVerts = header->direntries[11].length / sizeof(_IMESHVERT);
	_vMeshVerts = (_OMESHVERT*)malloc(sizeof(_OMESHVERT)*_iNumMeshVerts);
	_IMESHVERT *meshVerts = (_IMESHVERT*)malloc(sizeof(_IMESHVERT)*_iNumMeshVerts);
	fseek(pFile, header->direntries[11].offset, SEEK_SET);
	fread((_IMESHVERT*)meshVerts, sizeof(_IMESHVERT), _iNumMeshVerts, pFile);

	for(int i = 0; i < _iNumMeshVerts; i++)
	{
		_OMESHVERT t;
		t.offset = meshVerts[i].offset;
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
const int Odorless::Engine::Parsers::BSP::ParseVBSP(FILE* pFile)
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

void Odorless::Engine::Parsers::BSP::DebugRender()
{
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if(_iBSPType == BSP_TYPE_IBSP)
	{
		for(int i = 0; i < _iNumFaces; i++)
		{
			if(_vFaces[i].type != 1 && _vFaces[i].type != 3)
			{
				continue;
			}

			if(_vTextures[_vFaces[i].texture]>0)
			{
				glBindTexture(GL_TEXTURE_2D, _vTextures[_vFaces[i].texture]);
			}
			else
				glBindTexture(GL_TEXTURE_2D, 1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if(_vFaces[i].type == 1)
			{
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
				if(_vFaces[i].type == 3)
				{
					float x, y, z;

					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBegin(GL_TRIANGLES);
					for(int j = _vFaces[i].meshvert; j < _vFaces[i].meshvert + _vFaces[i].n_meshverts; j++)
					{
						x = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[0];
						y = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[1];
						z = _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].position[2];
						glTexCoord2f(_vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].texcoord[0][0], _vVertices[_vFaces[i].vertex + _vMeshVerts[j].offset].texcoord[0][1]);
						glVertex3f(x, y, z);
					}
					glEnd();
				}
		}
		glDisable(GL_BLEND);
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