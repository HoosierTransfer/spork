#include <Conv2D.hpp>
#include <defines.h>

#ifdef OPENMP
#include <omp.h>
#endif

Conv2D::Conv2D(std::vector<std::vector<double>> kernel, int inputWidth, int inputHeight) {
    this->kernel = kernel;
}

#ifdef OPENMP
std::vector<std::vector<double>> Conv2D::apply(std::vector<std::vector<double>> input) {
    std::vector<std::vector<double>> output(input.size(), std::vector<double>(input[0].size(), 0.0f));

    #pragma omp parallel for
    for (int y = 0; y < static_cast<int>(input.size()); y++) {
        for (unsigned int x = 0; x < input[0].size(); x++) {
            for (unsigned int ky = 0; ky < kernel.size(); ky++) {
                for (unsigned int kx = 0; kx < kernel[0].size(); kx++) {
                    int dx = x + kx - kernel[0].size() / 2;
                    int dy = y + ky - kernel.size() / 2;

                    if (dx >= 0 && dx < static_cast<int>(input[0].size()) && dy >= 0 && dy < static_cast<int>(input.size())) {
                        output[y][x] += input[dy][dx] * kernel[ky][kx];
                    }
                }
            }
        }
    }

    return output;
}
#else
std::vector<std::vector<double>> Conv2D::apply(std::vector<std::vector<double>> input) {
    std::vector<std::vector<double>> output(input.size(), std::vector<double>(input[0].size(), 0.0f));

    for (unsigned int y = 0; y < input.size(); y++) {
        for (unsigned int x = 0; x < input[0].size(); x++) {
            for (unsigned int ky = 0; ky < kernel.size(); ky++) {
                for (unsigned int kx = 0; kx < kernel[0].size(); kx++) {
                    int dx = x + kx - kernel[0].size() / 2;
                    int dy = y + ky - kernel.size() / 2;

                    if (dx >= 0 && dx < input[0].size() && dy >= 0 && dy < input.size()) {
                        output[y][x] += input[dy][dx] * kernel[ky][kx];
                    }
                }
            }
        }
    }

    return output;
}
#endif
