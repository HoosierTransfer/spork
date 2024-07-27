#include <Grid.hpp>

#include <algorithm>
#include <cmath>
#include <cstring>

#include <map>
#include <algorithm>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <Framebuffer.hpp>

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

void Grid::exportToVideo(const std::string& filename, int frameCount, int fps) {
    // Create VideoWriter object
    cv::VideoWriter videoWriter(filename, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, cv::Size(512, 512), true);

    if (!videoWriter.isOpened()) {
        std::cerr << "Error opening video file for write." << std::endl;
        return;
    }

    Framebuffer framebuffer(512, 512);

    // Ensure framebuffer is bound before capturing
    framebuffer.bind();
    framebuffer.resize(512, 512);  
    glViewport(0, 0, 512, 512);

    // Draw and capture each frame
    for (int frame = 0; frame < frameCount; ++frame) {
        framebuffer.clear();

        // Draw the grid
        draw();

        // Read pixels from the framebuffer
        std::vector<unsigned char> pixels(512 * 512 * 3); // RGB
        glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

        // Convert to OpenCV matrix
        cv::Mat image(512, 512, CV_8UC3, pixels.data());
        cv::Mat flippedImage;
        cv::flip(image, flippedImage, 0); // Flip the image vertically

        cv::Mat bgrImage;
        cv::cvtColor(flippedImage, bgrImage, cv::COLOR_RGBA2BGR);


        // Write the frame to the video
        videoWriter.write(bgrImage);

        // Update the grid
        update();
    }

    // Release the framebuffer
    framebuffer.unbind();

    videoWriter.release();
}


