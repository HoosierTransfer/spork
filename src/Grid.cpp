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

    grid = std::vector<float>(width * height, 0.0f);
    
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            grid[y * width + x] = rand() / (float)RAND_MAX;
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, grid.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

void Grid::draw() {
    GLuint pbo;
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * sizeof(float), nullptr, GL_STREAM_DRAW);

    void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    if (ptr) {
        memcpy(ptr, grid.data(), width * height * sizeof(float));
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
    grid = std::vector<float>(width * height, 0.0f);
}

void Grid::setHeight(unsigned int height) {
    this->height = height;
    grid = std::vector<float>(width * height, 0.0f);
}

unsigned int Grid::getWidth() const {
    return width;
}

unsigned int Grid::getHeight() const {
    return height;
}

float& Grid::operator()(unsigned int x, unsigned int y) {
    return grid[y * width + x];
}

std::vector<float>& Grid::operator[](unsigned int row) {
    rowVec.resize(width); // Resize the member variable
    for (unsigned int i = 0; i < width; i++) {
        rowVec[i] = grid[row * width + i];
    }

    return rowVec;
}

void Grid::set(unsigned int x, unsigned int y, float value) {
    grid[y * width + x] = value;
}

float Grid::get(unsigned int x, unsigned int y) const {
    return grid[y * width + x];
}

float func_smooth(float x, float a, float ea) {
    return 1.0f / (1.0f + std::exp(-(x - a) * 4.0f / ea));
}

float sigmoid_ab(float sn, float x, float a, float b) {
    return func_smooth(x, a, sn) * (1.0f - func_smooth(x, b, sn));
}

float sigmoid_mix(float sm, float x, float y, float m) {
    return x + func_smooth(m, 0.5f, sm) * (y - x);
}

void Grid::update() {
    float birthStart = 0.27f;
    float birthEnd = 0.34f;
    float deathStart = 0.52;
    float deathEnd = 0.1f;

    float alpha_n = 0.03f;
    float alpha_m = 0.15f;

    float dt = 0.116f;

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            float n = 0.0f;
            float m = 0.0f;

            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    if (i == 0 && j == 0) {
                        continue;
                    }

                    int x_ = x + i;
                    int y_ = y + j;

                    if (x_ < 0 || x_ >= width || y_ < 0 || y_ >= height) {
                        continue;
                    }

                    n += grid[y_ * width + x_];
                    m += grid[y_ * width + x_];
                }
            }

            n /= 8.0f;
            m /= 8.0f;

            float prev = grid[y * width + x];

            float s = sigmoid_ab(
                alpha_n,
                n,
                sigmoid_mix(alpha_m, birthStart, deathStart, m),
                sigmoid_mix(alpha_m, birthEnd, deathEnd, m)
            );

            float next = prev + dt * (s - prev);
            grid[y * width + x] = std::clamp(next, 0.0f, 1.0f);
        }
    }
}