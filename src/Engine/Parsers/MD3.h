/***************************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
***************************************************************************************************/
#ifndef MD3_H_
#define MD3_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "Engine/Maths/Vector.h"
#include "Engine/Textures/TextureManager.h"
#include <GLFW\glfw3.h>

#define MAX_QPATH 64
#define MD3_MAX_FRAMES 1024
#define MD3_MAX_TAGS 16
#define MD3_MAX_SURFACES 32
#define MD3_MAX_SHADERS 256
#define MD3_MAX_VERTS 4096
#define MD3_MAX_TRIANGLES 8192
#define MD3_XYZ_SCALE 0.015625

namespace OE
{
	namespace Parsers
	{
		class MD3
		{
		public:
			struct _MD3_HEADER
			{
				int Ident;
				int Version;
				char Name[MAX_QPATH];
				int Flags;
				int NumFrames;
				int NumTags;
				int NumSurfaces;
				int NumSkins;	//	Not really used.
				int OffsetFrames;
				int OffsetTags;
				int OffsetSurfaces;
				int OffsetEOF;
			};

			struct _MD3_FRAME
			{
				OE::Maths::Vec3<float> MinBounds;
				OE::Maths::Vec3<float> MaxBounds;
				OE::Maths::Vec3<float> LocalOrigin;
				float Radius;
				char Name[16];
			};

			struct _MD3_TAG
			{
				char Name[MAX_QPATH];
				OE::Maths::Vec3<float> Origin;
				OE::Maths::Vec3<float> Axis[3];
			};

			struct _MD3_SURFACE
			{
				int Ident;
				char Name[MAX_QPATH];
				int Flags;
				int NumFrames;
				int NumShaders;
				int NumVerts;
				int NumTriangles;
				int OffsetTriangles;
				int OffsetShaders;
				int OffsetSt;
				int OffsetVertices;
				int OffsetEnd;
			};

			struct _MD3_SHADER
			{
				char Name[MAX_QPATH];
				int ShaderIndex;
			};

			struct _MD3_TRIANGLE
			{
				int Indexes[3];
			};

			struct _MD3_TEXCOORD
			{
				float ST[2];	//	UV?
			};

			struct _MD3_VERTEX
			{
				OE::Maths::Vec3<short> Position;
				short Normal;
			};

			struct _OE_SHADER
			{
				_MD3_SHADER Shader;
				int textureIndex;
			};

			struct _OE_SURFACE
			{
				_MD3_SURFACE Surface;
				std::vector<_OE_SHADER> _vShaders;
				std::vector<_MD3_TRIANGLE> _vTriangles;
				std::vector<_MD3_TEXCOORD> _vTexCoords;
				std::vector<_MD3_VERTEX> _vVertices;
			};

			std::vector<_MD3_FRAME> _vFrames;
			std::vector<_MD3_TAG> _vTags;
			std::vector<_OE_SURFACE> _vSurfaces;

			MD3()
			{
			}

			~MD3()
			{
			}

			void ParseMD3(const char* path)
			{
				FILE *pFile;
				pFile = fopen(path, "rb");

				_MD3_HEADER* pHeader = (_MD3_HEADER*)malloc(sizeof(_MD3_HEADER));
				fread((_MD3_HEADER*)pHeader,sizeof(_MD3_HEADER),1, pFile);

				char ident[5] = 
				{
					(char)pHeader->Ident, 
					(char)(pHeader->Ident>>4), 
					(char)(pHeader->Ident>>16), 
					(char)(pHeader->Ident>>24), 
					0
				};

				if(strcmp("IDP3", ident) == 0)
				{
					_MD3_FRAME* pFrames = (_MD3_FRAME*)malloc(sizeof(_MD3_FRAME)*pHeader->NumFrames);
					fseek(pFile, pHeader->OffsetFrames, SEEK_SET);
					fread((_MD3_FRAME*)pFrames, sizeof(_MD3_FRAME), pHeader->NumFrames, pFile);

					for(int i = 0; i < pHeader->NumFrames; ++i)
					{
						_vFrames.push_back(pFrames[i]);
					}

					free(pFrames);

					_MD3_SURFACE* pSurfaces = (_MD3_SURFACE*)malloc(sizeof(_MD3_SURFACE)*pHeader->NumSurfaces);
					fseek(pFile, pHeader->OffsetSurfaces, SEEK_SET);
					fread((_MD3_SURFACE*)pSurfaces, sizeof(_MD3_SURFACE), pHeader->NumSurfaces, pFile);

					for(int i = 0; i < pHeader->NumSurfaces; ++i)
					{
						_OE_SURFACE tempSurf;
						tempSurf.Surface = pSurfaces[i];

						_MD3_SHADER* pShaders = (_MD3_SHADER*)malloc(sizeof(_MD3_SHADER)*pSurfaces[i].NumShaders);
						_MD3_TRIANGLE* pTriangles = (_MD3_TRIANGLE*)malloc(sizeof(_MD3_TRIANGLE)*pSurfaces[i].NumTriangles);
						_MD3_TEXCOORD* pTexCoords = (_MD3_TEXCOORD*)malloc(sizeof(_MD3_TEXCOORD)*pSurfaces[i].NumVerts);
						_MD3_VERTEX* pVertices = (_MD3_VERTEX*)malloc(sizeof(_MD3_VERTEX)*pSurfaces[i].NumVerts);

						fseek(pFile, pHeader->OffsetSurfaces + pSurfaces[i].OffsetShaders, SEEK_SET);
						fread((_MD3_SHADER*)pShaders, sizeof(_MD3_SHADER), pSurfaces[i].NumShaders, pFile);
						for(int j = 0; j < pSurfaces[i].NumShaders; ++j)
						{
							_OE_SHADER tShader;
							tShader.Shader = pShaders[j];
							std::string tName = "base/";
							tName += std::string(pShaders[j].Name);
							tShader.textureIndex = OE::Textures::TextureManager::LoadTextureFromPath(tName.c_str());
							tempSurf._vShaders.push_back(tShader);
						}

						fseek(pFile, pHeader->OffsetSurfaces + pSurfaces[i].OffsetTriangles, SEEK_SET);
						fread((_MD3_TRIANGLE*)pTriangles, sizeof(_MD3_TRIANGLE), pSurfaces[i].NumTriangles, pFile);
						for(int j = 0; j < pSurfaces[i].NumTriangles; ++j)
						{
							tempSurf._vTriangles.push_back(pTriangles[j]);
						}

						fseek(pFile, pHeader->OffsetSurfaces + pSurfaces[i].OffsetSt, SEEK_SET);
						fread((_MD3_TEXCOORD*)pTexCoords, sizeof(_MD3_TEXCOORD), pSurfaces[i].NumVerts, pFile);
						for(int j = 0; j < pSurfaces[i].NumVerts; ++j)
						{
							tempSurf._vTexCoords.push_back(pTexCoords[j]);
						}

						fseek(pFile, pHeader->OffsetSurfaces + pSurfaces[i].OffsetVertices, SEEK_SET);
						fread((_MD3_VERTEX*)pVertices, sizeof(_MD3_VERTEX), pSurfaces[i].NumVerts, pFile);
						for(int j = 0; j < pSurfaces[i].NumVerts; ++j)
						{
							float y = static_cast<float>(pVertices[j].Position[2] * MD3_XYZ_SCALE);
							float z = static_cast<float>(-pVertices[j].Position[1] * MD3_XYZ_SCALE);
							pVertices[j].Position[0] = static_cast<short>(pVertices[j].Position[0] * MD3_XYZ_SCALE);
							pVertices[j].Position[1] = static_cast<short>(y);
							pVertices[j].Position[2] = static_cast<short>(z);
							tempSurf._vVertices.push_back(pVertices[j]);
						}

						_vSurfaces.push_back(tempSurf);

						free(pShaders);
						free(pTriangles);
						free(pTexCoords);
						free(pVertices);
					}

					free(pSurfaces);
				}

				fclose(pFile);
				free(pHeader);
			}

			void DebugRender()
			{
				for( unsigned int i = 0; i < _vSurfaces.size(); ++i )
				{
					if(_vSurfaces[i]._vShaders[0].textureIndex==-1)
						continue;

					glBindTexture(GL_TEXTURE_2D, OE::Textures::TextureManager::GetTexturesID(_vSurfaces[i]._vShaders[0].textureIndex));
					glEnable(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glBegin(GL_TRIANGLES);
					for( unsigned int j = 0; j < _vSurfaces[i]._vTriangles.size(); ++j )
					{
						for(int k = 0; k < 3; ++k)
						{
							int index = _vSurfaces[i]._vTriangles[j].Indexes[k];
							_MD3_VERTEX vert = _vSurfaces[i]._vVertices[index];
							glTexCoord2f(_vSurfaces[i]._vTexCoords[index].ST[0], _vSurfaces[i]._vTexCoords[index].ST[1]);
							glVertex3f(vert.Position.x, vert.Position.y, vert.Position.z);
						}
					}
					glEnd();
				}
			}
		};
	}
}

#endif /*MD3_H_*/
