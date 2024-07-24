#include <lenia/Lenia.hpp>

#include <Math.hpp>
#include <Kernel.hpp>
#include <defines.h>

#include <iostream>
#ifdef OPENMP
#include <omp.h>
#endif

using namespace std;

Lenia::Lenia(unsigned int width, unsigned int height, LeniaTemplate rule) : Grid(width, height) {
    conv2d = new FastConv2D(buildKernel(width, rule.kernelRadius, rule.coreId, rule.layerId, rule.kernel_B, rule.alpha), width, height);
    updateFrequency = rule.updateFrequency;
    NS = rule.kernelRadius;
    mu = rule.mu;
    sigma = rule.sigma;
    alpha = rule.alpha;
    delta = vector<vector<double>>(height, vector<double>(width, 0.0));
    std::cout << "Lenia::Lenia" << std::endl;
}

#ifdef OPENMP
void Lenia::update() {
    vector<vector<double>> newData = data;
    newData = conv2d->apply(newData);
  
    #pragma omp parallel for
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            double d = deltaFunc(newData[y][x]);
            delta[y][x] = d;
            newData[y][x] = clip(data[y][x] + 1.0 / updateFrequency * d, 0.0, 1.0);
        }
    }

    data = newData;
}
#else
void Lenia::update() {
    vector<vector<double>> newData = data;
    newData = conv2d->apply(newData);
  
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            double d = deltaFunc(newData[y][x]);
            delta[y][x] = d;
            newData[y][x] = clip(data[y][x] + 1.0 / updateFrequency * d, 0.0, 1.0);
        }
    }

    data = newData;
}
#endif

double Lenia::deltaFunc(double x) {
    double r = abs(x - mu);
    double r2 = r * r;
    double k, k2, p, q; // Declare variables outside the switch
    switch (deltaId) {
        case 0:
            k = 2.0 * sigma * sigma;
            return exp(-r2 / k) * 2.0 - 1.0;
        case 1:
            k2 = 9.0 * sigma * sigma;
            return r2 > k2 ? -1.0 : pow(1.0 - r2 / k2, alpha) * 2.0 - 1.0;
        case 2:
            p = sigma / 2.0;
            q = sigma * 2.0;
            return r <= p ? 1.0 : r <= q ? 2.0 * (q - r) / (q - p) - 1.0 : -1.0;
        case 3:
            return r <= sigma ? 1.0 : -1.0;
    }
    return 0.0;
}
