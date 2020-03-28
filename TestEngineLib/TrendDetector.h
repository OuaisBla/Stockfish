#pragma once


#include <math.h>
#include "EvalStacker.h"


class TrendDetector
{
public:
  

  TrendDetector() = default;

  TrendDetector(std::vector<double> const &valX, std::vector<double> const &valY);
  

  std::vector<double> const &GetCurveX() const;
  std::vector<double> const &GetCurveY() const;

  std::vector<double> const &GetPolynomial3Coefficients() const;
  std::pair<double, double> GetExponentialCoefficients() const;
  std::pair<double, double> GetLogarithmCoefficients() const;


  std::vector<double> GetExponentialFitCurve() const;
  std::vector<double> GetPolynomial3FitCurve() const;
  std::vector<double> GetLogarithmFitCurve() const;
  std::vector<double> GetPowerLawFitCurve() const;

  Color FavorableSide() const;

  void ReverseSide();

  void FavorWhite();

private:

  std::vector<double> curveX;
  std::vector<double> curveY;

  std::vector<double> oCoeff;

};

