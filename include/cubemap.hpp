#ifndef FS_CUBEMAP_H
#define FS_CUBEMAP_H

#include "common.hpp"
#include "shader.hpp"

#include "utils/json.hpp"
#include "utils/utils.hpp"
#include "utils/stbi_global.hpp"

namespace Firesteel {
    class Cubemap {
    public:
        Cubemap()
            : mInitialized(false), mID(0), mVAO(0), mVBO(0) {}

        void load(std::string tD,
            std::string tR,
            std::string tL,
            std::string tT,
            std::string tBot,
            std::string tF,
            std::string tBack) {
            load(tD.c_str(), tR.c_str(), tL.c_str(), tT.c_str(), tBot.c_str(), tF.c_str(), tBack.c_str());
        }

        void load(std::string tCubemapJson) {
            if(!std::filesystem::exists(tCubemapJson)) return;
            mCfgFile = tCubemapJson;
            std::ifstream ifs(tCubemapJson);
            nlohmann::json txt = nlohmann::json::parse(ifs);
            load(txt["dir"], txt["posZ"], txt["negZ"], txt["posY"], txt["negY"], txt["posX"], txt["negX"]);
        }

        void load(const char* t_dir,
            const char* t_right = "right.png",
            const char* t_left = "left.png",
            const char* t_top = "top.png",
            const char* t_bottom = "bottom.png",
            const char* t_front = "front.png",
            const char* t_back = "back.png") {
            //Setup.
            mDir = t_dir;
            mInitialized = true;
            mFaces = { t_front, t_back, t_top, t_bottom, t_right, t_left };
            glActiveTexture(GL_TEXTURE11);
            glGenTextures(1, &mID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
            //Load faces.
            for (unsigned int i = 0; i < 6; i++) {
                TextureData t = TextureDataFromFile(mDir + "/" + mFaces[i]);
                //Get color mode.
                GLenum color_mode = GL_RED;
                switch (t.nrComponents) {
                case 3:
                    color_mode = GL_RGB;
                    break;
                case 4:
                    color_mode = GL_RGBA;
                    break;
                }
                //Bind data.
                if (t.data)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, color_mode, t.width, t.height, 0, color_mode, GL_UNSIGNED_BYTE, t.data);
                else
                    LOG_ERRR(std::string("Failed to load texture at \"") + (const char*)mFaces[i] + "\".");
                //Free data.
                free(t.data);
            }
            //Communicate texture properties to OpenGL.
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glActiveTexture(GL_TEXTURE0);
        }

        void bind() const {
            if(!mInitialized) return;
            glActiveTexture(GL_TEXTURE11);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
        }

        unsigned int getID() const { return mID; }

        //void load(const char* t_cb_file_path) {
        //    clear();
        //    std::vector<std::string> files = split_str(&read_from_file(t_cb_file_path), '\n');
        //    if (files.size() != 7) return;
        //    //Readress function.
        //    load_m(files[0].c_str(), files[1].c_str(), files[2].c_str(),
        //        files[3].c_str(), files[4].c_str(), files[5].c_str(), files[6].c_str());
        //}

        void initialize(const float& t_size) {
            // set up vertices
            mSize = t_size;
            float skybox_vert[] = {
                // positions          
                -1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size, -1.0f * t_size,

                -1.0f * t_size, -1.0f * t_size,  1.0f * t_size,
                -1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                -1.0f * t_size, -1.0f * t_size,  1.0f * t_size,

                 1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size, -1.0f * t_size,

                -1.0f * t_size, -1.0f * t_size,  1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size,  1.0f * t_size,
                -1.0f * t_size, -1.0f * t_size,  1.0f * t_size,

                -1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size,  1.0f * t_size,
                -1.0f * t_size,  1.0f * t_size, -1.0f * t_size,

                -1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size, -1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size, -1.0f * t_size,
                -1.0f * t_size, -1.0f * t_size,  1.0f * t_size,
                 1.0f * t_size, -1.0f * t_size,  1.0f * t_size
            };
            //Create buffers and arrays.
            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);
            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vert), &skybox_vert, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        }

        void draw(const Shader* t_shader) const {
            if(!mInitialized) return;
            glDepthFunc(GL_LEQUAL);
            t_shader->enable();
            //Skybox cube.
            glBindVertexArray(mVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }

        void clear() {
            glDeleteTextures(1, &mID);
            mFaces.clear();
        }

        void remove() {
            if(!mInitialized) return;
            glDeleteVertexArrays(1, &mVAO);
            glDeleteBuffers(1, &mVBO);
            clear();
        }

        std::string getDirectory() const { return mDir; }
        std::string getCfgFile() const { return mCfgFile; }
        float getSize() const { return mSize; }
        bool isInitialized() const { return mInitialized; }

        ~Cubemap() { remove(); }
    private:
        unsigned int mVAO, mVBO, mID;
        float mSize;
        std::string mDir, mCfgFile;
        std::vector<const char*> mFaces;
        bool mInitialized;
    };
}

#endif // !FS_CUBEMAP_H