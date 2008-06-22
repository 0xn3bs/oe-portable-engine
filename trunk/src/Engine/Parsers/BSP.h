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
#ifndef BSP_H_
#define BSP_H_

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Engine/Base.h"
#include "Engine/Textures/TextureManager.h"

#define	V_HEADER_LUMPS	64

#define BSP_TYPE_UNKNOWN -1
#define BSP_TYPE_IBSP 0
#define BSP_TYPE_VBSP 1

namespace OE
{
	namespace Parsers
	{
		class BSP
		{
		public:

			//	ID BSP
			struct _IBSP_LUMPDIRECTORY
			{
				int Offset;
				int Length;
			};

			struct _IBSP_HEADER
			{
				char Magic[4];
				int Version;
				_IBSP_LUMPDIRECTORY DirEntries[17];
			};

			struct _IBSP_TEXTURE
			{
				char Name[64];
				int Flags;
				int Contents;
			};

			struct _IBSP_VERTEX
			{
				float Position[3];
				float TexCoord[2][2];
				float Normal[3];
				unsigned char Color[4];
			};

			struct _IBSP_MESHVERT
			{
				int Offset;
			};

			struct _IBSP_FACE
			{
				int Texture;
				int Effect;
				int Type;
				int Vertex;
				int NumVertex;
				int MeshVert;
				int NumMeshVerts;
				int LMIndex;
				int LMStart[2];
				int LMSize[2];
				float LMOrigin[3];
				float LMVecs[2][3];
				float Normal[3];
				int Size[2];
			};

			struct _IBSP_LIGHTMAP
			{
				unsigned char map[128][128][3];
			};

			//	Valve BSP
			struct _VBSP_LUMP
			{
				int		FileOfs, FileLen;
				int		Version;
				char	FourCC[4];
			};

			struct _VBSP_HEADER
			{
				int Ident;
				int Version;
				_VBSP_LUMP Lumps[V_HEADER_LUMPS];
				int MapRevision;
			};

			struct _VBSP_LUMPDIRECTORY
			{
				int Offset;
				int Length;
			};

			struct _VBSP_VECTOR
			{
				float x, y, z;
			};

			struct _VBSP_VERTEX
			{
				_VBSP_VECTOR Point;
			};

			struct _VBSP_FACE
			{
				unsigned short PlaneNum;		// the plane number
				char Side;				// faces opposite to the node's plane direction
				char OnNode; 			// 1 of on node, 0 if in leaf
				int FirstEdge;			// index into surfedges	
				short NumEdges;			// number of surfedges
				short TexInfo;			// texture info
				short DispInfo;			// displacement info
				short SurfaceFogVolumeID;		// ?	
				char Styles[4];			// switchable lighting info
				int LMOfs;			// offset into lightmap lump
				float Area;				// face area in units^2
				int LightmapTextureMinsInLuxels[2];   // texture lighting info
				int LightmapTextureSizeInLuxels[2];   // texture lighting info
				int OriginalFace;			// original face this was split from
				unsigned short    NumPrims;		// primitives
				unsigned short    FirstPrimID; 
				unsigned int      SmoothingGroups;	// lightmap smoothing group
			};

			struct _VBSP_EDGE
			{
				unsigned short v[2];
			};

			struct _VBSP_TEXINFO
			{
				float		textureVecsTexelsPerWorldUnits[2][4];			// [s/t][xyz offset]
				float		lightmapVecsLuxelsPerWorldUnits[2][4];			// [s/t][xyz offset] - length is in units of texels/area
				int			flags;				// miptex flags + overrides
				int			texdata;			// Pointer to texture name, size, etc.
			};

			//	Odorless Entertainment BSP
			//	All other BSP formats will (hopefully) be converted to this format.
			struct _OBSP_FACE
			{
				int Texture;
				int Type;
				int Vertex;
				int NumVerts;
				int MeshVert;
				int NumMeshVerts;
				int Size[2];
				int LMIndex;
				int LMStart[2];
				int LMSize[2];
			};

			struct _OBSP_MESHVERT
			{
				int Offset;
			};

			struct _OBSP_VERTEX
			{
				float Position[3];
				float TexCoord[2][2];
				float Normal[3];
				unsigned char Color[4];
			};

			struct _OBSP_EDGE
			{
				unsigned short v[2];
			};

			struct _OBSP_LIGHTMAP
			{
				unsigned char map[128][128][3];
				int textureIndex;
			};

			BSP()
			{
			}

			~BSP()
			{
				free(_vVertices);
				free(_vEdges);
				free(_vFaces);
				free(_vTextures);
				free(_vMeshVerts);
				free(_vLightMaps);
			}

			const int ParseBSP(const char *path);
			const int ParseIBSP(const char* path);
			const int ParseVBSP(const char* path);

			void DebugRender();

		private:
			void _IBSP_ParseVertices(FILE *file, _IBSP_HEADER* header);
			void _IBSP_ParseFaces(FILE *file, _IBSP_HEADER* header);
			void _IBSP_ParseTextures(FILE *file, _IBSP_HEADER* header);
			void _IBSP_ParseMeshVerts(FILE* file, _IBSP_HEADER* header);
			void _IBSP_ParseLightmaps(FILE* file, _IBSP_HEADER* header);

			_OBSP_VERTEX* _vVertices;
			int _iNumVertices;

			_OBSP_EDGE* _vEdges;
			int _iNumEdges;

			_OBSP_FACE* _vFaces;
			int _iNumFaces;

			int* _vTextures;
			int _iNumTextures;

			_OBSP_MESHVERT* _vMeshVerts;
			int _iNumMeshVerts;

			_OBSP_LIGHTMAP* _vLightMaps;
			int _iNumLightMaps;

			int _iBSPType;
		};
	}
}
#endif
