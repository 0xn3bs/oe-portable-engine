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
#include "Engine/Textures/TextureManager.h"

#define	V_HEADER_LUMPS	64

#define BSP_TYPE_UNKNOWN -1
#define BSP_TYPE_IBSP 0
#define BSP_TYPE_VBSP 1

namespace Odorless
{
	namespace Engine
	{
		namespace Parsers
		{
			class BSP
			{
			public:
				struct _ILUMPDIRECTORY
				{
					int offset;
					int length;
				};

				struct _VLUMPDIRECTORY
				{
					int offset;
					int length;
				};

				struct _IHEADER
				{
					char magic[4];
					int version;
					_ILUMPDIRECTORY direntries[17];
				};

				struct _VLUMP
				{
					int		fileofs, filelen;
					int		version;
					char	fourCC[4];
				};

				struct _VEDGE
				{
					unsigned short v[2];
				};

				struct _VVECTOR
				{
					float x, y, z;
				};

				struct _VHEADER
				{
					int ident;
					int version;
					_VLUMP lumps[V_HEADER_LUMPS];
					int mapRevision;
				};

				struct _IVERTEX
				{
					float position[3];
					float texcoord[2][2];
					float normal[3];
					unsigned char color[4];
				};
				
				struct _IMESHVERT
				{
					int offset;
				};

				struct _VVERTEX
				{
					_VVECTOR point;
				};

				struct _ITEXTURE
				{
					char name[64];
					int flags;
					int contents;
				};

				struct _IFACE
				{
					int texture;
					int effect;
					int type;
					int vertex;
					int n_vertexes;
					int meshvert;
					int n_meshverts;
					int lm_index;
					int lm_start[2];
					int lm_size[2];
					float lm_origin[3];
					float lm_vecs[2][3];
					float normal[3];
					int size[2];
				};

				struct _VFACE
				{
					unsigned short planenum;		// the plane number
					char side;				// faces opposite to the node's plane direction
					char onNode; 			// 1 of on node, 0 if in leaf
					int firstedge;			// index into surfedges	
					short numedges;			// number of surfedges
					short texinfo;			// texture info
					short dispinfo;			// displacement info
					short surfaceFogVolumeID;		// ?	
					char styles[4];			// switchable lighting info
					int lightofs;			// offset into lightmap lump
					float area;				// face area in units^2
					int LightmapTextureMinsInLuxels[2];   // texture lighting info
					int LightmapTextureSizeInLuxels[2];   // texture lighting info
					int origFace;			// original face this was split from
					unsigned short    numPrims;		// primitives
					unsigned short    firstPrimID; 
					unsigned int      smoothingGroups;	// lightmap smoothing group
				};

				struct _VTEXINFO
				{
					float		textureVecsTexelsPerWorldUnits[2][4];			// [s/t][xyz offset]
					float		lightmapVecsLuxelsPerWorldUnits[2][4];			// [s/t][xyz offset] - length is in units of texels/area
					int			flags;				// miptex flags + overrides
					int			texdata;			// Pointer to texture name, size, etc.
				};

				//	All that follows deals with the OBSP that all other BSP formats are converted to
				struct _OFACE
				{
					int texture;
					int type;
					int vertex;
					int n_vertexes;
					int meshvert;
					int n_meshverts;
				};

				struct _OMESHVERT
				{
					int offset;
				};

				struct _OVERTEX
				{
					float position[3];
					float texcoord[2][2];
					float normal[3];
					unsigned char color[4];
				};

				struct _OEDGE
				{
					unsigned short v[2];
				};

				BSP()
				{
				}

				~BSP()
				{
				}

				const int ParseBSP(const char *path);
				const int ParseIBSP(FILE* pFile);
				const int ParseVBSP(FILE* pFile);

				void DebugRender();

			private:
				std::vector<_OVERTEX> _vVertices;
				std::vector<_OEDGE> _vEdges;
				std::vector<_OFACE> _vFaces;
				std::vector<GLint> _vTextures;
				std::vector<_OMESHVERT> _vMeshVerts;
				int _iBSPType;
			};
		}
	}
}
#endif
