#pragma once

#include <Grid.hpp>
#include <FastConv2D.hpp>
#include <lenia/LeniaTemplate.h>

#include <lenia/LeniaTaxonomyTree.hpp>

#include <string>

#include <Math.hpp>

#include <lenia/Growth.hpp>

class Lenia : public Grid {
public:
    Lenia(unsigned int width, unsigned int height, LeniaTemplate rule);
    void update() override;

    double mu = 0.15;

    double sigma = 0.017;

    double alpha = 4.0;

    int deltaId = 0;

    int updateFrequency = 10;

    bool softClip = false;

    void drawImGui() override;

    void parse(std::string pattern) override;

    std::string toString() const override;


private:
    std::vector<std::vector<double>> delta;

    std::vector<std::vector<double>> kernel;

    FastConv2D* conv2d;

    int NS;

    LeniaTaxonomyTree tree;

    double deltaFunc(double x);

    void setRule(std::string rule);

    void addCellArrray(std::vector<std::vector<std::string>> cells, int x1, int y1, int x2, int y2, double scale, double angle, int flip, int w, int h);

    void drawTree(TreeNode* node, int level);
};