#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.hpp>

class Grid {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int width, height;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Shader shader;
    unsigned int texture;

    std::vector<float> grid;

    std::vector<float> rowVec;

    void normalize();

public:
    Grid(unsigned int width, unsigned int height);
    void draw();
    void update();
    void setWidth(unsigned int width);
    void setHeight(unsigned int height);
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    float& operator()(unsigned int x, unsigned int y);

    std::vector<float>& operator[](unsigned int row);

    void set(unsigned int x, unsigned int y, float value);
    float get(unsigned int x, unsigned int y) const;
};

