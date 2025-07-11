#ifndef FS_CUBEMAP_H
#define FS_CUBEMAP_H
#include "common.hpp"
#include "shader.hpp"
#include "utils/json.hpp"
#include "utils/utils.hpp"
#include "utils/stbi_global.hpp"

namespace Firesteel {
    struct Cubemap {
    public:
        Cubemap()
            : mInitialized(false), mID(0), mVAO(0), mVBO(0) {}
        // Create cubemap from given files.
        void load(const std::string& tDir,
                  const std::string& tR,
                  const std::string& tL,
                  const std::string& tT,
                  const std::string& tBot,
                  const std::string& tF,
                  const std::string& tBack) {
            load(tDir.c_str(), tR.c_str(), tL.c_str(), tT.c_str(), tBot.c_str(), tF.c_str(), tBack.c_str());
        }
        // Create cubemap from files that are in json.
        void load(const std::string& tCubemapJson) {
            if(!std::filesystem::exists(tCubemapJson)) {
                LOG_WARN("Cubemap at: \"" + tCubemapJson + "\" doesn't exist")
                return;
            }
            mCfgFile=tCubemapJson;
            std::ifstream ifs(tCubemapJson);
            nlohmann::json txt=nlohmann::json::parse(ifs);
            load(txt["dir"], txt["posZ"], txt["negZ"], txt["posY"], txt["negY"], txt["posX"], txt["negX"]);
        }
        // Create cubemap from given files.
        void load(const char* tDir,
                  const char* tRight="right.png",
                  const char* tLeft="left.png",
                  const char* tTop="top.png",
                  const char* tBottom="bottom.png",
                  const char* tFront="front.png",
                  const char* tBack="back.png") {
            //Setup.
            mDir=tDir;
            mInitialized=true;
            mFaces={ tFront, tBack, tTop, tBottom, tRight, tLeft };
            glActiveTexture(GL_TEXTURE11);
            glGenTextures(1, &mID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
            //Load faces.
            for (unsigned int i=0; i < 6; i++) {
                TextureData t=TextureDataFromFile(mDir + "/" + mFaces[i]);
                //Get color mode.
                GLenum color_mode=GL_RED;
                switch (t.nrComponents) {
                case 3:
                    color_mode=GL_RGB;
                    break;
                case 4:
                    color_mode=GL_RGBA;
                    break;
                }
                //Bind data.
                if(t.data) {
#ifdef FS_PRINT_DEBUG_MSGS
                    LOG_DBG("Loaded cubemap texture \"" + mDir + "/" + mFaces[i] + "\"");
#endif // FS_PRINT_DEBUG_MSGS
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, color_mode, t.width, t.height, 0, color_mode, GL_UNSIGNED_BYTE, t.data);
                } else LOG_ERRR(std::string("Failed to load texture at \"") + (const char*)mFaces[i] + "\".");
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
        // Binds this cubemap
        void enable() const {
            if(!mInitialized) return;
            glActiveTexture(GL_TEXTURE11);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
        }

        // Create mesh for cubemap.
        void initialize(const float& tSize) {
            //Set up vertices.
            mSize=tSize;
            float skybox_vert[]={
                // positions          
                -1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize, -1.0f * tSize,

                -1.0f * tSize, -1.0f * tSize,  1.0f * tSize,
                -1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                -1.0f * tSize, -1.0f * tSize,  1.0f * tSize,

                 1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize, -1.0f * tSize,

                -1.0f * tSize, -1.0f * tSize,  1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize,  1.0f * tSize,
                -1.0f * tSize, -1.0f * tSize,  1.0f * tSize,

                -1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize,  1.0f * tSize,
                -1.0f * tSize,  1.0f * tSize, -1.0f * tSize,

                -1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize, -1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize, -1.0f * tSize,
                -1.0f * tSize, -1.0f * tSize,  1.0f * tSize,
                 1.0f * tSize, -1.0f * tSize,  1.0f * tSize
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

        // Render cubemap.
        void draw(const Shader* tShader) const {
            if(!mInitialized) return;
            glDepthFunc(GL_LEQUAL);
            tShader->enable();
            //Skybox cube.
            glBindVertexArray(mVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }
        // Clear meshes.
        void clear() {
            glDeleteTextures(1, &mID);
            mFaces.clear();
        }
        // Remove meshes and textures.
        void remove() {
            if(!mInitialized) return;
            mInitialized=false;
            glDeleteVertexArrays(1, &mVAO);
            glDeleteBuffers(1, &mVBO);
            clear();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Removed cubemap");
#endif // FS_PRINT_DEBUG_MSGS
        }

        unsigned int getID() const { return mID; }
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