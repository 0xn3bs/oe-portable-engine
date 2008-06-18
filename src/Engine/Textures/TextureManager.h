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
#ifndef TEXTURE_H_
#define TEXTURE_H_

#define TEXTURE_PATH "base/"

#include <GL/glfw.h>
#include <vector>
#include <string>
#include "FreeImage.h"

#ifndef GL_TEXTURE_2D_BINDING_EXT
#define GL_TEXTURE_2D_BINDING_EXT         0x8069
#endif

namespace OE
{
	namespace Textures
	{
		class TextureManager
		{
		public:
			static GLint LoadTexture(const char* name);
			static void DeleteTexture(const GLuint index);
			static void Dispose();

		private:
			static std::vector<GLint> _vLoadedTextures;
			static bool _LoadImage(const char* path, GLuint Texture);
			static bool _DoesFileExist(const char* path);
			static std::string _GetTexturePath(const char* name);
			static GLint _LoadTextureFromPath(const char* path);

			static void _SwapRedAndBlueComponents(FIBITMAP* image, int width, int height)
			{
				for( int x = 0; x < width; x++ )
				{
					for( int y = 0; y < height; y++ )
					{
						RGBQUAD p;
						FreeImage_GetPixelColor( image, x, y, &p );

						BYTE tmp = p.rgbRed;
						p.rgbRed = p.rgbBlue;
						p.rgbBlue = tmp;

						FreeImage_SetPixelColor( image, x, y, &p );
					}
				}
			}

			static void _GenerateMipmaps(FIBITMAP* image, GLint &width, GLint &height, unsigned int &bpp)
			{
				bool canGenerateMipmaps = (width % 2 == 0 && height % 2 == 0 && width/height == 1) ? true : false;

				const int tBpp = (bpp == 24) ? GL_RGB : GL_RGBA;

				if(canGenerateMipmaps)
				{
					int i = 0;
					for(int j = width; j >= 1; j/=2)
					{
						image = FreeImage_Rescale(image, j, j, FREE_IMAGE_FILTER::FILTER_BILINEAR);
						BYTE* bits = FreeImage_GetBits(image);
						glTexImage2D(GL_TEXTURE_2D, i, tBpp, j, j, 0, tBpp, GL_UNSIGNED_BYTE, bits);
						i++;
					}
				}
				else
				{
					BYTE* bits = FreeImage_GetBits(image);
					glTexImage2D(GL_TEXTURE_2D, 0, tBpp, width, height, 0, tBpp, GL_UNSIGNED_BYTE, bits);
				}
			}
		};
	}
}

#endif /*TEXTURE_H_*/
