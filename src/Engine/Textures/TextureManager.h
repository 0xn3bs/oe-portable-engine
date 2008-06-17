#ifndef TEXTURE_H_
#define TEXTURE_H_

#define TEXTURE_PATH "base/"

#include "Engine/Parsers/JPEG/tinyjpeg.h"
#include <GL/glfw.h>
#include <vector>
#include <string>

#ifndef GL_TEXTURE_2D_BINDING_EXT
#define GL_TEXTURE_2D_BINDING_EXT         0x8069
#endif

namespace OEngine
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
			static bool LoadTGA(const char* path, GLuint Texture);
			static bool LoadJPG(const char* path, GLuint Texture);
			static bool DoesFileExist(const char* path);
			static std::string GetTexturePath(const char* name);
			static GLint _LoadTextureFromPath(const char* path);
		};
	}
}

#endif /*TEXTURE_H_*/
