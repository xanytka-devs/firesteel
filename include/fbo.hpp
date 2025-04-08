#ifndef FS_FBO
#define FS_FBO

#include "common.hpp"
#include "shader.hpp"

namespace Firesteel {
    class Framebuffer {
    public:
        Framebuffer() { }

        Framebuffer(const int& tWidth, const int& tHeight, const size_t& tFBOTextures = 1) {
            mSize = glm::vec2(tWidth, tHeight);
            createBuffers(tFBOTextures);
        }
        Framebuffer(const glm::vec2& tSize, const size_t& tFBOTextures = 1) {
            mSize = tSize;
            createBuffers(tFBOTextures);
        }

        void scale(const int& tWidth, const int& tHeight) {
            mSize = glm::vec2(tWidth, tHeight);
            scaleBuffers();
        }

        void scale(const glm::vec2& tSize) {
            mSize = tSize;
            scaleBuffers();
        }

        bool isComplete() const {
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            bool result = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return result;
        }
        void bind() const {
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            glEnable(GL_DEPTH_TEST);
        }
        void unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
        }

        void quad() {
            float quadVertices[] = {
                // positions       UVs
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,

                -1.0f,  1.0f,  0.0f, 1.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f, 1.0f
            };
            //Screen quad VAO.
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        }
        void bindTexture() const {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, FBOtextures[0]);
        }
        void drawQuad(const Shader* tShader) const {
            tShader->enable();
            glBindVertexArray(quadVAO);
            bindTexture();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        unsigned int getID(const size_t& tID = 0) const { return FBOtextures[tID]; }
        glm::vec2 getSize() const { return mSize; }
        float getWidth() const { return mSize.x; }
        float getHeight() const { return mSize.y; }
        float aspect() const { return (mSize.x / mSize.y); }
    private:
        glm::vec2 mSize = glm::vec2(0);
        unsigned int quadVAO = 0, quadVBO = 0;
        unsigned int FBO = 0, RBO = 0;
        unsigned int FBOtextures[11]{};
        size_t FBOsSize = 1;

        void createBuffers(const size_t& tFBOTexs) {
            FBOsSize = tFBOTexs;
            glActiveTexture(GL_TEXTURE0);
            //FBO creation.
            glGenFramebuffers(1, &FBO);
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            //Framebuffer's textures.
            glGenTextures(static_cast<GLsizei>(FBOsSize), FBOtextures);
            unsigned int attachments[11]{};
            for (size_t i = 0; i < FBOsSize; i++) {
                glBindTexture(GL_TEXTURE_2D, FBOtextures[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(mSize.x), static_cast<GLsizei>(mSize.y), 0, GL_RGB, GL_FLOAT, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                attachments[i] = GL_COLOR_ATTACHMENT0 + static_cast<unsigned int>(i);
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, FBOtextures[i], 0);
            }
            //Framebuffer's render buffer.
            glGenRenderbuffers(1, &RBO);
            glBindRenderbuffer(GL_RENDERBUFFER, RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(mSize.x), static_cast<GLsizei>(mSize.y));
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
            // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
            glDrawBuffers(static_cast<GLsizei>(FBOsSize), attachments);
            glActiveTexture(GL_TEXTURE0);
        }
        void scaleBuffers() const {
            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            //Framebuffer's textures.
            for(size_t i = 0; i < FBOsSize; i++) {
                glBindTexture(GL_TEXTURE_2D, FBOtextures[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(mSize.x), static_cast<GLsizei>(mSize.y), 0, GL_RGB, GL_FLOAT, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLsizei>(i), GL_TEXTURE_2D, FBOtextures[i], 0);
            }
            //Framebuffer's render buffer.
            glBindRenderbuffer(GL_RENDERBUFFER, RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(mSize.x), static_cast<GLsizei>(mSize.y));
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
            //Unbind framebuffer.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glActiveTexture(GL_TEXTURE0);
        }
    };
}

#endif // !FS_FBO