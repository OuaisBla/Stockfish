
#pragma once

#include<vector>


std::vector<double> Polynomial3Fit(std::vector<double> const &x, std::vector<double> const &y);

std::vector<double> PowerFit(std::vector<double> const &x, std::vector<double> const &y);

std::pair<double, double> ExponentialFitCoeffs(std::vector<double> const &x, std::vector<double> const &y);

std::vector<double> ExponentialFit(std::vector<double> const &x, std::vector<double> const &y);


std::pair<double, double> LogarithmFitCoeffs(std::vector<double> const &x, std::vector<double> const &y);

std::vector<double> LogarithmFit(std::vector<double> const &x, std::vector<double> const &y);
