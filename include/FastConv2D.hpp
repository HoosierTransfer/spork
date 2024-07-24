#pragma once

#include <Conv2D.hpp>
#include <fftw3.h>
#include <complex>

class FastConv2D : public Conv2D {
public:
    FastConv2D() = default;

    FastConv2D(std::vector<std::vector<double>> kernel, int inputWidth, int inputHeight);
    ~FastConv2D();

    std::vector<std::vector<double>> apply(std::vector<std::vector<double>> input) override;
private:
    fftw_complex* _kernel_fft;
    fftw_complex* _grid_fft;
    fftw_complex* _input_fft;
    fftw_plan _gridPlan;
    fftw_plan _inversePlan;
};