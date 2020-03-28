
#include "stdafx.h"
#include "TrendDetector.h"
#include "CurveFitting.h"
#include "polyfit.hpp"


TrendDetector::TrendDetector(std::vector<double> const &valX, std::vector<double> const &valY) : 
  curveX(valX), 
  curveY(valY) 
{ 
  assert(!curveX.empty());
  assert(!curveY.empty());
  assert(valX.size() == valY.size());

  oCoeff = mathalgo::polyfit(valX, valY);
}

std::vector<double> const &TrendDetector::GetCurveX() const
{
  return curveX;
}

std::vector<double> const &TrendDetector::GetCurveY() const
{
  return curveY;
}

std::vector<double> const &TrendDetector::GetPolynomial3Coefficients() const
{
  return oCoeff;
}

std::pair<double,double> TrendDetector::GetExponentialCoefficients() const
{
  return ExponentialFitCoeffs(curveX, curveY);
}

std::pair<double, double> TrendDetector::GetLogarithmCoefficients() const
{
  return LogarithmFitCoeffs(curveX, curveY);
}


std::vector<double> TrendDetector::GetExponentialFitCurve() const
{
  std::vector<double> ExpFit = ExponentialFit(curveX, curveY);

  return ExpFit;
}

std::vector<double> TrendDetector::GetPolynomial3FitCurve() const
{
  std::vector<double> oFittedY = mathalgo::polyval(oCoeff, curveX);

  return oFittedY;
}

std::vector<double> TrendDetector::GetLogarithmFitCurve() const
{
  std::vector<double> LogFit = LogarithmFit(curveX, curveY);

  return LogFit;
}

std::vector<double> TrendDetector::GetPowerLawFitCurve() const
{
  std::vector<double> PowFit = PowerFit(curveX, curveY);

  return PowFit;
}

Color TrendDetector::FavorableSide() const
{
  if (oCoeff.size() == 4 && oCoeff[3] < -0.002)
  {
    return BLACK;
  }

  return WHITE;
}

void TrendDetector::ReverseSide()
{
  for( size_t i = 0; i < curveY.size(); ++i )
  {
    curveY[i] = -curveY[i];
  }

  oCoeff = mathalgo::polyfit(curveX, curveY);
}

void TrendDetector::FavorWhite()
{
  if (FavorableSide() == BLACK)
  {
    ReverseSide();
  }
}
