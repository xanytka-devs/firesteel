#define STB_IMAGE_IMPLEMENTATION

#include <fstream>
#include <sstream>
#include <iostream>

#include "XEngine/ResLoader.hpp"
#include "XEngine/Log.hpp"

void ResLoader::flipImagesVertical(bool flip) {
	stbi_set_flip_vertically_on_load(flip);
}

stbi_uc* ResLoader::loadImage(char const* file, int* width, int* height, int* comp, int req_comp) {
    try {
        return stbi_load(file, width, height, comp, req_comp);
    }
    catch (const std::exception& e) {
        LOG_ERRR("Failed to load texture '{0}'", file);
        return nullptr;
    }
}

std::string ResLoader::loadText(const char* path) {
    std::ifstream fileStream;
    //Ensure ifstream objects can throw exceptions:
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        fileStream.open(path);
        std::stringstream fileStringStream;
        // read file's buffer contents into streams
        fileStringStream << fileStream.rdbuf();
        // close file handlers
        fileStream.close();
        // convert stream into string
        return fileStringStream.str();
    }
    catch (std::ifstream::failure e) {
        LOG_ERRR("File at path '{0}' couldn't be loaded.", path);
    }
}