#include "BSP.h"
#include <GL/glfw.h>

const int Odorless::Engine::Parsers::BSP::ParseBSP(const char *path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");

	_IHEADER *header = (_IHEADER*)malloc(sizeof(_IHEADER));
	fread((_IHEADER*)header,sizeof(_IHEADER),1, pFile);
	fseek(pFile, 0, SEEK_SET);
	if(strcmp(header->magic, "IBSP.") == 0)
		ParseIBSP(pFile);
	else
	if(strcmp(header->magic, "VBSP.") == 0)
		ParseVBSP(pFile);

	free(header);
	fclose(pFile);
	return 0;
}

const int Odorless::Engine::Parsers::BSP::ParseIBSP(FILE* pFile)
{
	_IHEADER *header = (_IHEADER*)malloc(sizeof(_IHEADER));
	fread((_IHEADER*)header,sizeof(_IHEADER),1, pFile);

	int numVertices = header->direntries[10].length / sizeof(_IVERTEX);
	_IVERTEX *vertices = (_IVERTEX*)malloc(sizeof(_IVERTEX)*numVertices);
	fseek(pFile, header->direntries[10].offset, SEEK_SET);
	fread((_IVERTEX*)vertices, sizeof(_IVERTEX), numVertices, pFile);

	for(int i = 0; i < numVertices; i++)
	{
		int y = vertices[i].position[2];
		int z = -vertices[i].position[1];
		vertices[i].position[1] = y;
		vertices[i].position[2] = z;
		vVertices.push_back(vertices[i]);
	}

	int numFaces = header->direntries[13].length / sizeof(_IFACE);
	_IFACE *faces = (_IFACE*)malloc(sizeof(_IFACE)*numFaces);
	fseek(pFile, header->direntries[13].offset, SEEK_SET);
	fread((_IFACE*)faces, sizeof(_IFACE), numFaces, pFile);

	for(int i = 0; i < numFaces; i++)
	{
		vFaces.push_back(faces[i]);
	}
	return 0;
}

const int Odorless::Engine::Parsers::BSP::ParseVBSP(FILE* pFile)
{
	_IHEADER *header = (_IHEADER*)malloc(sizeof(_IHEADER));
	fread((_IHEADER*)header,sizeof(_IHEADER),1, pFile);

	int numVertices = header->direntries[10].length / sizeof(_IVERTEX);
	_IVERTEX *vertices = (_IVERTEX*)malloc(sizeof(_IVERTEX)*numVertices);
	fseek(pFile, header->direntries[10].offset, SEEK_SET);
	fread((_IVERTEX*)vertices, sizeof(_IVERTEX), numVertices, pFile);

	for(int i = 0; i < numVertices; i++)
	{
		int y = vertices[i].position[2];
		int z = -vertices[i].position[1];
		vertices[i].position[1] = y;
		vertices[i].position[2] = z;
		vVertices.push_back(vertices[i]);
	}

	int numFaces = header->direntries[13].length / sizeof(_IFACE);
	_IFACE *faces = (_IFACE*)malloc(sizeof(_IFACE)*numFaces);
	fseek(pFile, header->direntries[13].offset, SEEK_SET);
	fread((_IFACE*)faces, sizeof(_IFACE), numFaces, pFile);

	for(int i = 0; i < numFaces; i++)
	{
		vFaces.push_back(faces[i]);
	}
	return 0;
}

void Odorless::Engine::Parsers::BSP::DebugRender()
{
	glBindTexture(GL_TEXTURE_2D, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	for(int i = 0; i < vFaces.size(); i++)
	{
		if(vFaces[i].type != 1)
		{
			continue;
		}

		glBegin(GL_POLYGON);
		for(int j = vFaces[i].vertex; j < vFaces[i].vertex + vFaces[i].n_vertexes; j++)
		{
			glTexCoord2f(vVertices[j].texcoord[0][0], vVertices[j].texcoord[0][1]);
			glVertex3f(vVertices[j].position[0], vVertices[j].position[1], vVertices[j].position[2]);
		}
		glEnd();
	}
}