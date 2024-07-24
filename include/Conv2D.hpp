#pragma once

#include <vector>
#include <cmath>

class Conv2D {
public:
    Conv2D() = default;
    Conv2D(std::vector<std::vector<double>> kernel, int inputWidth, int inputHeight);

    virtual std::vector<std::vector<double>> apply(std::vector<std::vector<double>> input);
private:
    std::vector<std::vector<double>> kernel;
};