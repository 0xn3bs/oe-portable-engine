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

				struct _IHEADER
				{
					char magic[4];
					int version;
					_ILUMPDIRECTORY direntries[17];
				};

				struct _INODE
				{
					int plane;
					int children[2];
					int mins[3];
					int maxs[3];
				};

				struct _IVERTEX
				{
					float position[3];
					float texcoord[2][2];
					float normal[3];
					unsigned char color[4];
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
				std::vector<_IVERTEX> vVertices;
				std::vector<_IFACE> vFaces;
			};
		}
	}
}
#endif
