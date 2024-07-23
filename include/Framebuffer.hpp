#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Framebuffer {
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    void bind();
    void unbind();

    void clear();

    void resize(int width, int height);

    int getWidth() const;
    int getHeight() const;

    GLuint getTexture() const;

private:
    GLuint m_fbo;
    GLuint m_texture;
    GLuint m_rbo;
    int m_width;
    int m_height;
};
