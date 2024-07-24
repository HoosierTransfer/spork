#pragma once

#include <Grid.hpp>
#include <FastConv2D.hpp>
#include <lenia/LeniaTemplate.h>

#include <Math.hpp>

class Lenia : public Grid {
public:
    Lenia(unsigned int width, unsigned int height, LeniaTemplate rule);
    void update() override;

    double mu = 0.15;

    double sigma = 0.017;

    double alpha = 4.0;

    int deltaId = 0;

    int updateFrequency = 10;

private:
    std::vector<std::vector<double>> delta;

    FastConv2D* conv2d;

    int NS;

    double deltaFunc(double x);
};