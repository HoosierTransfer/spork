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

double sigmoid(double x, double a, double b) {
    return 1.0 / (1.0 + exp(-4.0 * (x - 0.5)));
}

double Round(double x) {
    return std::round(x * PRECISION) / PRECISION;
}

int RandomInt(int min, int max) {
    return min + (rand() % (max - min + 1));
}

std::vector<std::vector<double>> convolve2D(const std::vector<std::vector<double>>& grid, const std::vector<std::vector<double>>& kernel) {
    int rows = grid.size();
    int cols = grid[0].size();
    int krows = kernel.size();
    int kcols = kernel[0].size();
    
    // Determine the size of the FFT (padded to prevent wrap-around effects)
    int fftRows = rows;
    int fftCols = cols;
    
    // Allocate memory for input, kernel, and output
    fftw_complex *inGrid = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftRows * fftCols);
    fftw_complex *inKernel = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftRows * fftCols);
    fftw_complex *outGrid = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftRows * fftCols);
    fftw_complex *outKernel = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftRows * fftCols);
    
    // Create plans
    fftw_plan planForwardGrid = fftw_plan_dft_2d(fftRows, fftCols, inGrid, outGrid, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan planForwardKernel = fftw_plan_dft_2d(fftRows, fftCols, inKernel, outKernel, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan planBackward = fftw_plan_dft_2d(fftRows, fftCols, outGrid, inGrid, FFTW_BACKWARD, FFTW_ESTIMATE);
    
    // Initialize input arrays with zero padding
    for (int i = 0; i < fftRows; ++i) {
        for (int j = 0; j < fftCols; ++j) {
            if (i < rows && j < cols) {
                inGrid[i * fftCols + j][0] = grid[i][j];
                inGrid[i * fftCols + j][1] = 0.0;
            } else {
                inGrid[i * fftCols + j][0] = 0.0;
                inGrid[i * fftCols + j][1] = 0.0;
            }
            if (i < krows && j < kcols) {
                inKernel[i * fftCols + j][0] = kernel[i][j];
                inKernel[i * fftCols + j][1] = 0.0;
            } else {
                inKernel[i * fftCols + j][0] = 0.0;
                inKernel[i * fftCols + j][1] = 0.0;
            }
        }
    }
    
    // Execute forward FFTs
    fftw_execute(planForwardGrid);
    fftw_execute(planForwardKernel);
    
    // Perform element-wise multiplication in the frequency domain
    for (int i = 0; i < fftRows * fftCols; ++i) {
        double real = outGrid[i][0] * outKernel[i][0] - outGrid[i][1] * outKernel[i][1];
        double imag = outGrid[i][0] * outKernel[i][1] + outGrid[i][1] * outKernel[i][0];
        outGrid[i][0] = real;
        outGrid[i][1] = imag;
    }
    
    // Execute inverse FFT
    fftw_execute(planBackward);
    
    // Normalize the result and copy to output vector
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = inGrid[i * fftCols + j][0] / (fftRows * fftCols);
        }
    }
    
    // Free allocated memory
    fftw_destroy_plan(planForwardGrid);
    fftw_destroy_plan(planForwardKernel);
    fftw_destroy_plan(planBackward);
    fftw_free(inGrid);
    fftw_free(inKernel);
    fftw_free(outGrid);
    fftw_free(outKernel);
    
    return roll(result, -1, -1);
}

bool IsZero(double x) { return (x < EPSILON && x > -EPSILON); }
bool IsPos(double x) { return (x > EPSILON); }
bool IsNeg(double x) { return (x < -EPSILON); }

TwoPointsData TwoPoints(double x1, double y1, double x2, double y2, double olda) {
	double dX = x1 - x2;
	double dY = y1 - y2;
	double d = sqrt(dX*dX + dY*dY);
	double a = IsZero(d) ? 0 : atan2(dY, dX) / M_PI * 180;
	double da = !olda ? 0 : a - olda;
    da = wrapD(da + 540, 360) - 180;

    return {d, a, da};
}