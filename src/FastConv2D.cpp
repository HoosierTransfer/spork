#include <FastConv2D.hpp>

#include <Math.hpp>

#include <iostream>
#include <stdexcept>

FastConv2D::FastConv2D(std::vector<std::vector<double>> kernel, int inputWidth, int inputHeight) : Conv2D(kernel, inputWidth, inputHeight) {
    int kernelWidth = kernel.size();
    int kernelHeight = kernel[0].size();

    fftw_complex* kernelIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * inputWidth * inputHeight);
    _kernel_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * inputWidth * inputHeight);
    fftw_plan kernelPlan = fftw_plan_dft_2d(inputWidth, inputHeight, kernelIn, _kernel_fft, FFTW_FORWARD, FFTW_PATIENT);
    if (kernelWidth != inputWidth || kernelHeight != inputHeight) {
        throw std::invalid_argument("Kernel size must match input size for FastConv2D");
    }
    for (int x = 0; x < inputWidth; x++) {
        for (int y = 0; y < inputHeight; y++) {
            kernelIn[x * inputHeight + y][0] = kernel[x][y];
            kernelIn[x * inputHeight + y][1] = 0;
        }
    }

    fftw_execute(kernelPlan);
    fftw_destroy_plan(kernelPlan);
    fftw_free(kernelIn);

    _grid_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * inputWidth * inputHeight);
    _input_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * inputWidth * inputHeight);    

    _gridPlan = fftw_plan_dft_2d(inputWidth, inputHeight, _input_fft, _grid_fft, FFTW_FORWARD, FFTW_PATIENT);
    _inversePlan = fftw_plan_dft_2d(inputWidth, inputHeight, _grid_fft, _input_fft, FFTW_BACKWARD, FFTW_PATIENT);
}

FastConv2D::~FastConv2D() {
    fftw_destroy_plan(_gridPlan);
    fftw_destroy_plan(_inversePlan);
    fftw_free(_grid_fft);
    fftw_free(_input_fft);
    fftw_free(_kernel_fft);
}

std::vector<std::vector<double>> FastConv2D::apply(std::vector<std::vector<double>> input) {
    int inputWidth = input.size();
    int inputHeight = input[0].size();

    for (int x = 0; x < inputWidth; x++) {
        for (int y = 0; y < inputHeight; y++) {
            _input_fft[x * inputHeight + y][0] = input[x][y];
            _input_fft[x * inputHeight + y][1] = 0;
        }
    }

    fftw_execute(_gridPlan);

    for (int x = 0; x < inputWidth; x++) {
        for (int y = 0; y < inputHeight; y++) {
            double real = _grid_fft[x * inputHeight + y][0] * _kernel_fft[x * inputHeight + y][0] - _grid_fft[x * inputHeight + y][1] * _kernel_fft[x * inputHeight + y][1];
            double imag = _grid_fft[x * inputHeight + y][0] * _kernel_fft[x * inputHeight + y][1] + _grid_fft[x * inputHeight + y][1] * _kernel_fft[x * inputHeight + y][0];
            _grid_fft[x * inputHeight + y][0] = real;
            _grid_fft[x * inputHeight + y][1] = imag;
        }
    }

    fftw_execute(_inversePlan);

    std::vector<std::vector<double>> output(inputWidth, std::vector<double>(inputHeight));
    for (int x = 0; x < inputWidth; x++) {
        for (int y = 0; y < inputHeight; y++) {
            output[x][y] = _input_fft[x * inputHeight + y][0] / (inputWidth * inputHeight);
        }
    }

    return roll(output, -1, -1);
}
