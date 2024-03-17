#ifndef TEXTURE_H
#define TEXTURE_H

enum aiTextureType;

namespace XEngine {
	class Texture {
	public:
		Texture();
		/// <summary>
		/// Creates new texture instance.
		/// </summary>
		/// <param name="t_path">Path to texture.</param>
		/// <param name="t_file">Name of file..</param>
		/// <param name="t_type">Texture type.</param>
		Texture(std::string t_path, std::string t_file, aiTextureType t_type) :
			path(t_path), file(t_file), type(t_type) {
			generate(); load();
		}

		/// <summary>
		/// Generate texture.
		/// </summary>
		void generate();
		/// <summary>
		/// Load texture file.
		/// </summary>
		/// <param name="t_flip">Flip texture by Y?</param>
		void load(bool t_flip = false);
		/// <summary>
		/// Set texture to current.
		/// </summary>
		void enable();
		/// <summary>
		/// Delete texture (cleanup).
		/// </summary>
		void remove();

		unsigned int id;
		aiTextureType type;
		std::string path;
		std::string file;
	};
}

#endif // TEXTURE_H