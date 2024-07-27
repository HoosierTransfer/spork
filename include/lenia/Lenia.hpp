#pragma once

#include <Grid.hpp>
#include <FastConv2D.hpp>
#include <lenia/LeniaTemplate.h>

#include <lenia/LeniaTaxonomyTree.hpp>

#include <string>

#include <Math.hpp>

#include <lenia/Growth.hpp>

struct Stat {
    double max;
    std::string name;
    std::vector<double> data;
    double value;
};

class Lenia : public Grid {
public:
    Lenia(unsigned int width, unsigned int height, LeniaTemplate rule);

    void update() override;
    void updateStats(bool shouldCalc) override;

    double mu = 0.15;
    double sigma = 0.017;
    double alpha = 4.0;

    int deltaId = 0;

    int statLength = 300;

    int updateFrequency = 10;

    bool softClip = false;

    void drawImGui() override;

    void parse(std::string pattern) override;
    std::string toString() const override;

    void reset();
private:
    void updateStat(std::string name, double value, bool log, double max);
    void updateStat(std::string name, double value, bool logScale, double max, bool forceMax, bool add);

    std::vector<std::vector<double>> delta;
    std::vector<std::vector<double>> kernel;

    std::vector<Stat> stats;

    FastConv2D* conv2d;

    int NS;

    LeniaTaxonomyTree tree;

    double deltaFunc(double x);

    void setRule(std::string rule);
    void addCellArrray(std::vector<std::vector<std::string>> cells, int x1, int y1, int x2, int y2, double scale, double angle, int flip, int w, int h);

    void drawTree(TreeNode* node, int level);

    double mX, mY; // Centroid of the pattern
    double gX, gY; // Growth centroid
    double axisA, axisB, th, ec, cp; // semi-major & semi-minor axes, orientation, eccentricity
    double vol, volg, moi, rog; // volume, growth, mass moment of inertia, radius of gyration
    double m00, m10, m01, m11, m20, m02, m30, m03, m21, m12, m22, m31, m13, m40, m04; // raw moments, mass = m00
    double g00, g10, g01; // raw moments for growth
    double centralMoment11, centralMoment20, centralMoment02, centralMoment30, centralMoment03, centralMoment21, centralMoment12, centralMoment22, centralMoment31, centralMoment13, centralMoment40, centralMoment04; // central moments (translation invariant)
    double normalizedMoment11, normalizedMoment20, normalizedMoment02, normalizedMoment30, normalizedMoment03, normalizedMoment21, normalizedMoment12, normalizedMoment22, normalizedMoment31, normalizedMoment13, normalizedMoment40, normalizedMoment04; // normalized central moments (translation & scale invariant)
    double huMomentInvariant1, huMomentInvariant2, huMomentInvariant3, huMomentInvariant4, huMomentInvariant5, huMomentInvariant6, huMomentInvariant7, huMomentInvariant8, huMomentInvariant9, huMomentInvariant10, huMomentInvariant11, huMomentInvariant12, huMomentInvariant13; // Hu's & Flusser's moment invariants (translation, scale & rotation invariant; huMomentInvariant2,huMomentInvariant3 not useful, huMomentInvariant7 skew invariant)

    double oldth=0, dth=0, th180=0;
    double shiftX, shiftY;
    double arrowX, arrowY;
    double oldmX, oldmY, dM, aM, oldaM, daM, dMG, aMG, oldaMG, daMG;


};
