#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.hpp>

class Grid {
private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Shader shader;
    unsigned int texture;

    void normalize();

    float* getData();

protected:
    unsigned int width, height;
    std::vector<std::vector<double>> data;

public:
    Grid(unsigned int width, unsigned int height);
    virtual ~Grid();
    void draw();
    virtual void update() {}
    void setWidth(unsigned int width);
    void setHeight(unsigned int height);
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void reset();
    void randomize();

    void randomizePatches();

    std::vector<double>& operator[](unsigned int row);
    const std::vector<double>& operator[](unsigned int row) const;

    void set(unsigned int x, unsigned int y, double value);
    double get(unsigned int x, unsigned int y) const;

    int randomPatches;
    int randomPatchSize;
};

