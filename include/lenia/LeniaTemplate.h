#pragma once

#include <vector>

typedef struct {
    int kernelRadius;
    int updateFrequency;
    double mu;
    double sigma;
    std::vector<double> kernel_B;
    std::vector<double> kernel_E; // unused
    int coreId;
    int layerId;
    int deltaId;
    double alpha;
    bool limitValue; // unused
} LeniaTemplate;