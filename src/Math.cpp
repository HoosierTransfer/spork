#include <Math.hpp>

std::vector<std::vector<double>> roll(std::vector<std::vector<double>> grid, int start_row, int start_col) {
    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<std::vector<double>> newGrid(rows, std::vector<double>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int new_row = (i - start_row + rows) % rows;
            int new_col = (j - start_col + cols) % cols;

            newGrid[new_row][new_col] = grid[i][j];
        }
    }

    return newGrid;
}

double bell(double x, double m, double s) {
    return std::exp(-std::pow((x-m) / s, 2) / 2);
}

int wrap(int x, int n) {
    return (x % n + n) % n;
}

double polynomial(double x, double m, double s, double a) {
    double r = std::abs(x - m);
    double r2 = r * r;
    double k2 = 9 * s*s;
    return r2 > k2 ? -1 : std::pow(1 - r2 / k2, a) * 2 - 1;
}

double wrapD(double x, double n) {
    return std::fmod(std::fmod(x, n) + n, n);
}

double clip(double x, double a, double b) {
    return x < a ? a : (x > b ? b : x);
}

std::vector<std::vector<double>> ogrid(int xStart, int xEnd, int yStart, int yEnd) {
    int xSize = std::abs(xEnd - xStart);
    int ySize = std::abs(yEnd - yStart);
    std::vector<std::vector<double>> grid(xSize, std::vector<double>(ySize));

    for (int i = xStart; i < xEnd; i++) {
        grid[0].push_back(i);
    }

    for (int i = yStart; i < yEnd; i++) {
        grid[1].push_back(i);
    }

    return grid;
}

std::vector<std::vector<double>> fftShift(std::vector<std::vector<double>> input) {
    std::vector<std::vector<double>> output(input.size(), std::vector<double>(input[0].size()));
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            output[i][j] = input[wrap(i + input.size() / 2, input.size())][wrap(j + input[0].size() / 2, input[0].size())];
        }
    }
    return output;
}

double* convertToDoublePointer(std::vector<std::vector<double>>& vec, int subVecIndex) {
    double* arr = new double[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i][subVecIndex];
    }
    return arr;
}