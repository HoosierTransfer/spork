#pragma once

#include <vector>
#include <cmath>
#include <fftw3.h>

const double EPSILON = 0.0000001;

const int PRECISION = 1000000;

double bell(double x, double m, double s);
double polynomial(double x, double m, double s, double a);
int wrap(int x, int n);
double wrapD(double x, double n);
double clip(double x, double a, double b);
std::vector<std::vector<double>> ogrid(int xStart, int xEnd, int yStart, int yEnd);
std::vector<std::vector<double>> fastConv2D(std::vector<std::vector<double>> grid);
std::vector<std::vector<double>> fftShift(std::vector<std::vector<double>> input);

std::vector<std::vector<double>> roll(std::vector<std::vector<double>> grid, int start_row, int start_col);
double* convertToDoublePointer(std::vector<std::vector<double>>& vec, int subVecIndex);

double sigmoid(double x, double a, double b);

double Round(double x);

int RandomInt(int min, int max);    

std::vector<std::vector<double>> convolve2D(const std::vector<std::vector<double>>& grid, const std::vector<std::vector<double>>& kernel);