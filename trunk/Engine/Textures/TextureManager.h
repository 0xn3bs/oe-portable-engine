#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glfw.h>
#include <vector>

#ifndef GL_TEXTURE_2D_BINDING_EXT
#define GL_TEXTURE_2D_BINDING_EXT         0x8069
#endif

namespace Odorless
{
	namespace Engine
	{
		namespace Textures
		{
			class TextureManager
			{
				public:
					static GLint LoadTexture(const char *path);
					static void DeleteTexture(const GLuint index);
					static void Dispose();

				private:
					static std::vector<GLint> _vLoadedTextures;
					static bool LoadTGA(const char *path, GLuint Texture);
					static bool LoadJPG(const char *path, GLuint Texture);
			};
		}
	}
}

#endif /*TEXTURE_H_*/
