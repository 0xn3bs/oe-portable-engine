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
#include "Engine/Input/Input.h"
#include "Engine/Textures/TextureManager.h"
#include "Engine/Maths/Vector.h"
#include "Engine/Cameras/FPSCamera.h"

#define	V_HEADER_LUMPS	64

#define BSP_TYPE_UNKNOWN -1
#define BSP_TYPE_IBSP 0
#define BSP_TYPE_VBSP 1

namespace OE
{
	namespace Parsers
	{
		class RenderPathManager
		{

		};

		class BSP : public OE::Input::InputListener
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

			struct _IBSP_PLANE
			{
				OE::Maths::Vec3<float> Normal;
				float Distance;
			};

			struct _IBSP_NODE
			{
				int Plane;
				int Children[2]; //	Negative numbers are leaf indices: -(leaf+1)
				int Mins[3];
				int Maxs[3];
			};

			struct _IBSP_LEAF
			{
				int Cluster;
				int Area;
				int Mins[3];
				int Maxs[3];
				int LeafFace;
				int NumLeafFaces;
				int LeafBrush;
				int NumLeafBrushes;
			};

			struct _IBSP_LEAFFACE
			{
				int Face;
			};

			struct _IBSP_LEAFBRUSH
			{
				int Brush;
			};
			
			struct _IBSP_BRUSH
			{
				int BrushSide;
				int NumBrushSides;
				int Texture;
			};

			struct _IBSP_BRUSHSIDE
			{
				int Plane;
				int Texture;
			};

			struct _IBSP_TEXTURE
			{
				char Name[64];
				int Flags;
				int Contents;
			};

			struct _IBSP_VERTEX
			{
				OE::Maths::Vec3<float> Position;
				float TexCoord[2][2];
				OE::Maths::Vec3<float> Normal;
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
				OE::Maths::Vec3<float> Position;
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

#define	_VBSP_VECTOR OE::Maths::Vec3<float>

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
			struct _OBSP_PLANE
			{
				OE::Maths::Vec3<double> Normal;
				float Distance;
			};

			struct _OBSP_NODE
			{
				int Plane;
				int Children[2]; //	Negative numbers are leaf indices: -(leaf+1)
				int Mins[3];
				int Maxs[3];
			};

			struct _OBSP_LEAF
			{
				int Cluster;
				int Area;
				int Mins[3];
				int Maxs[3];
				int LeafFace;
				int NumLeafFaces;
				int LeafBrush;
				int NumLeafBrushes;
			};

			struct _OBSP_LEAFFACE
			{
				int Face;
			};

			struct _OBSP_LEAFBRUSH
			{
				int Brush;
			};

			struct _OBSP_BRUSH
			{
				int BrushSide;
				int NumBrushSides;
				int Texture;
			};

			struct _OBSP_BRUSHSIDE
			{
				int Plane;
				int Texture;
			};

			struct _OBSP_TEXTURE
			{
				char Name[64];
				int Flags;
				int Contents;
				int TextureIndex;
			};

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
				OE::Maths::Vec3<float> Position;
				float TexCoord[2][2];
				OE::Maths::Vec3<float> Normal;
				unsigned char Color[4];
			};

			struct _OBSP_EDGE
			{
				unsigned short v[2];
			};

			struct _OBSP_LIGHTMAP
			{
				unsigned char map[128][128][3];
				int TextureIndex;
			};

			BSP()
			{
				_iNumVecs = 0;
				_iSizeOfVecs = 0;
				_iNumFacesToRender = 0;
				if(OE::Input::InputManager::IsInitialized()==false)
					std::cerr << "Warning: BSP - The Input Manager has not yet been initialized!" << std::endl;
				else
					OE::Input::InputManager::AddInputListener(this);
			}
			~BSP()
			{
				free(_vPlanes);
				free(_vNodes);
				free(_vLeafs);
				free(_vLeafFaces);
				free(_vLeafBrushes);
				free(_vVertices);
				free(_vEdges);
				free(_vFaces);
				free(_vTextures);
				free(_vMeshVerts);
				free(_vLightMaps);
				free(_vVecs);
				free(_vRenderedFaces);
			}

			const int ParseBSP(const char *path);
			const int ParseIBSP(const char* path);
			const int ParseVBSP(const char* path);

			void DebugRender(float dt, OE::Cameras::FPSCamera* fpsCamera);

		private:
			void _IBSP_ParseVertices(FILE *file, _IBSP_HEADER* header);
			void _IBSP_ParseFaces(FILE *file, _IBSP_HEADER* header);
			void _IBSP_ParseTextures(FILE *file, _IBSP_HEADER* header);
			void _IBSP_ParseMeshVerts(FILE* file, _IBSP_HEADER* header);
			void _IBSP_ParseLightmaps(FILE* file, _IBSP_HEADER* header);
			void _IBSP_ParseVisData(FILE* file, _IBSP_HEADER* header);

			void TraverseBSPTree(OE::Cameras::FPSCamera* fpsCamera, int nodeIndex);
			void RenderLeaf(int index);
			void RenderFace(int index);

			bool* _vRenderedFaces;

			_OBSP_PLANE* _vPlanes;
			int _iNumPlanes;

			_OBSP_NODE* _vNodes;
			int _iNumNodes;

			_OBSP_LEAF* _vLeafs;
			int _iNumLeafs;

			_OBSP_LEAFFACE* _vLeafFaces;
			int _iNumLeafFaces;

			_OBSP_LEAFBRUSH* _vLeafBrushes;
			int _iNumLeafBrushes;

			_OBSP_BRUSH* _vBrushes;
			int _iNumBrushes;

			_OBSP_BRUSHSIDE* _vBrushSides;
			int _iNumBrushSides;

			_OBSP_VERTEX* _vVertices;
			int _iNumVertices;

			_OBSP_EDGE* _vEdges;
			int _iNumEdges;

			_OBSP_FACE* _vFaces;
			int _iNumFaces;

			_OBSP_TEXTURE* _vTextures;
			int _iNumTextures;

			_OBSP_MESHVERT* _vMeshVerts;
			int _iNumMeshVerts;

			_OBSP_LIGHTMAP* _vLightMaps;
			int _iNumLightMaps;

			int _iNumVecs;
			int _iSizeOfVecs;
			unsigned char* _vVecs;

			int _iBSPType;

			int _iNumFacesToRender;
		};
	}
}

#endif /*BSP_H_*/
