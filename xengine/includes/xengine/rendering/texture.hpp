#ifndef TEXTURE_H
#define TEXTURE_H

enum aiTextureType;

namespace XEngine {
	enum TextureType {
		TT_NONE		= 0x0000,
		TT_2D		= 0x0DE1,
		TT_3D		= 0x806F,
		TT_CUBEMAP	= 0x8513
	};

	class Texture {
	public:
		Texture();
		/// <summary>
		/// Creates new texture instance.
		/// </summary>
		/// <param name="t_path">Path to texture.</param>
		/// <param name="t_file">Name of file.</param>
		/// <param name="t_type">Texture type.</param>
		Texture(const char* t_path, const char* t_file, aiTextureType t_type) :
			path(t_path), file(t_file), type(t_type) {
			generate(); load();
		}

		Texture(const char* t_path, const char* t_file, bool t_load = false)
			: path(t_path), file(t_file), type(1), id(0) {
			if(!t_load) return;
			generate(); load();
		}
		unsigned int get_id() const { return id; }

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
		void remove() const;

		int type;
		const char* path = "";
		const char* file = "";
	private:
		unsigned int id;
	};
}

#endif // TEXTURE_H