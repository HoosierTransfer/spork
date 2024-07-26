#pragma once

#include <vector>
#include <cmath>

class Growth {
public:
    int id;
    double mu;
    double sigma;
    double alpha;
    Growth(double mu, double sigma, double alpha, int id) : mu(mu), sigma(sigma), alpha(alpha), id(id) {}
    double apply(double x) {
        double r = abs(x - mu);
        double r2 = r * r;
        double k, k2, p, q; // Declare variables outside the switch
        switch (id) {
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
};