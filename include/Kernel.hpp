#pragma once

#include <vector>

double _kernelCoreFunction(double r, double alpha, int coreId);

double _kernelFunction(double r, int layerId, std::vector<double> b, int bDiv, double alpha , int coreId);

std::vector<std::vector<double>> buildKernel(int size, int nSize, int coreId, int layerId, std::vector<double> b, double alpha);