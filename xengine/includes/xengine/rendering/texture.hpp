#ifndef TEXTURE_H
#define TEXTURE_H

enum aiTextureType;

namespace XEngine {
	class Texture {
	public:
		Texture();
		Texture(std::string t_path, std::string t_file, aiTextureType t_type);

		void generate();
		void load(bool t_flip = false);
		void enable();
		void remove();

		unsigned int id;
		aiTextureType type;
		std::string path;
		std::string file;
	};
}

#endif // TEXTURE_H