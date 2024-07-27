#include <Grid.hpp>

#include <algorithm>
#include <cmath>
#include <cstring>

Grid::Grid(unsigned int width, unsigned int height) : width(width), height(height), shader("grid") {
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader.compile();

    data = std::vector<std::vector<double>>(height, std::vector<double>(width, 0.0));

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

Grid::~Grid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
}

float* Grid::getData() {
    float* data = new float[width * height];
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            data[y * width + x] = this->data[y][x];
        }
    }

    return data;
}

void Grid::draw() {
    GLuint pbo;
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * sizeof(float), nullptr, GL_STREAM_DRAW);

    void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    if (ptr) {
        memcpy(ptr, getData(), width * height * sizeof(float));
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_FLOAT, nullptr);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glDeleteBuffers(1, &pbo);

    shader.use();
    shader.setInt("grid", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Grid::setWidth(unsigned int width) {
    this->width = width;
    data = std::vector<std::vector<double>>(height, std::vector<double>(width, 0.0));
}

void Grid::setHeight(unsigned int height) {
    this->height = height;
    data = std::vector<std::vector<double>>(height, std::vector<double>(width, 0.0));
}

unsigned int Grid::getWidth() const {
    return width;
}

unsigned int Grid::getHeight() const {
    return height;
}

std::vector<double>& Grid::operator[](unsigned int row) {
    return data[row];
}

void Grid::set(unsigned int x, unsigned int y, double value) {
    data[y][x] = value;
}

double Grid::get(unsigned int x, unsigned int y) const {
    return data[y][x];
}

void Grid::reset() {
    generation = 0;
    std::fill(data.begin(), data.end(), std::vector<double>(width, 0.0));
}

void Grid::randomize() {
    generation = 0;
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            data[y][x] = rand() / (double)RAND_MAX;
        }
    }
}

void Grid::randomizePatches() {
    for (int i = 0; i < randomPatches; i++) {
        int x = rand() % width;
        int y = rand() % height;
        for (int j = 0; j < randomPatchSize; j++) {
            for (int k = 0; k < randomPatchSize; k++) {
                data[(y + j) % height][(x + k) % width] = rand() / (double)RAND_MAX;
            }
        }
    }
}
