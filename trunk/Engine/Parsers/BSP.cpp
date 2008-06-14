#include "BSP.h"
#include <GL/glfw.h>

const int Odorless::Engine::Parsers::BSP::ParseBSP(const char *path)
{
	FILE *pFile;
	pFile = fopen(path, "rb");

	_HEADER *header = (_HEADER*)malloc(sizeof(_HEADER));
	fread((_HEADER*)header,sizeof(_HEADER),1, pFile);

	int numVertices = header->direntries[10].length / sizeof(_VERTEX);
	_VERTEX *vertices = (_VERTEX*)malloc(sizeof(_VERTEX)*numVertices);
	fseek(pFile, header->direntries[10].offset, SEEK_SET);
	fread((_VERTEX*)vertices, sizeof(_VERTEX), numVertices, pFile);

	for(int i = 0; i < numVertices; i++)
	{
		int y = vertices[i].position[2];
		int z = -vertices[i].position[1];
		vertices[i].position[1] = y;
		vertices[i].position[2] = z;
		vVertices.push_back(vertices[i]);
	}

	int numFaces = header->direntries[13].length / sizeof(_FACE);
	_FACE *faces = (_FACE*)malloc(sizeof(_FACE)*numFaces);
	fseek(pFile, header->direntries[13].offset, SEEK_SET);
	fread((_FACE*)faces, sizeof(_FACE), numFaces, pFile);

	for(int i = 0; i < numFaces; i++)
	{
		vFaces.push_back(faces[i]);
	}

	fclose(pFile);
	return 0;
}

void Odorless::Engine::Parsers::BSP::DebugRender()
{
	glColor4f(0.0f, 1.0f, 0.0f,0.75f);
	for(int i = 0; i < vFaces.size(); i++)
	{
		glBegin(GL_LINES);
		for(int j = vFaces[i].vertex; j < vFaces[i].vertex + vFaces[i].n_vertexes; j++)
		{
			glVertex3f(vVertices[j].position[0], vVertices[j].position[1], vVertices[j].position[2]);
		}
		glEnd();
	}
}