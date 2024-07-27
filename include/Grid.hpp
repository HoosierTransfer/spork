#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <string>

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

    int generation = 0;

public:
    Grid(unsigned int width, unsigned int height);
    virtual ~Grid();
    void draw();
    virtual void update() = 0;
    virtual void updateStats(bool shouldCalc) = 0;
    virtual void drawImGui() = 0;
    void setWidth(unsigned int width);
    void setHeight(unsigned int height);
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    virtual void parse(std::string pattern) = 0;
    virtual std::string toString() const = 0;

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

