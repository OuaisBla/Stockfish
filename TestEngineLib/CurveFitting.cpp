
#include "stdafx.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include "CurveFitting.h"
#include "polyfit.hpp"


std::vector<double> Polynomial3Fit(std::vector<double> const &x, std::vector<double> const &y)
{
  std::vector<double> oCoeff = mathalgo::polyfit(x, y);
  std::vector<double> oFittedY = mathalgo::polyval(oCoeff, x);

  return oFittedY;
}

std::vector<double> PowerFit(std::vector<double> const &x, std::vector<double> const &y)
{
  size_t const n = x.size();

  double dmin = *min_element(y.begin(), y.end());

  std::vector<double> lny(n, 0.0);

  for (size_t i = 0; i < n; i++)                        //Calculate the values of ln(yi)
  {
    lny[i] = log((y[i] - dmin) + 1.0);
  }

  double xsum = 0, x2sum = 0, ysum = 0, xysum = 0;                //variables for sums/sigma of xi,yi,xi^2,xiyi etc

  for (size_t i = 0; i < n; i++)
  {
    double const lnxi = log(x[i]);

    xsum += lnxi;                        //calculate sigma(xi)
    ysum += lny[i];                        //calculate sigma(yi)
    x2sum += pow(lnxi, 2);                //calculate sigma(x^2i)
    xysum += lnxi * lny[i];                    //calculate sigma(xi*yi)
  }

  double b = (n * xysum - xsum * ysum) / (n * x2sum - xsum * xsum);            //calculate intercept

  double a = (ysum - b * xsum) / n;            //calculate slope(or the the power of exp)

  double c = exp(a);                        //since b=ln(c)

  std::vector<double> y_fit(n);                        //an array to store the new fitted values of y    

  for (size_t i = 0; i < n; i++)
  {
    y_fit[i] = c * pow(x[i], b);                    //to calculate y(fitted) at given x points
  }

  return y_fit;
}

std::pair<double, double> ExponentialFitCoeffs(std::vector<double> const &x, std::vector<double> const &y)
{
  size_t const n = x.size();

  double dmin = *min_element(y.begin(), y.end());

  std::vector<double> lny(n, 0.0);

  for (size_t i = 0; i < n; i++)                        //Calculate the values of ln(yi)
  {
    lny[i] = log((y[i] - dmin) + 1.0);
  }

  double x2_y_sum = 0, y_lny_sum = 0, x_y_sum = 0, x_y_lny_sum = 0, y_sum = 0;

  for (size_t i = 0; i < n; i++)
  {
    y_sum += y[i];
    x2_y_sum += x[i] * x[i] * y[i];
    y_lny_sum += y[i] * lny[i];
    x_y_sum += x[i] * y[i];
    x_y_lny_sum += x[i] * y[i] * lny[i];
  }

  double a = (x2_y_sum * y_lny_sum - x_y_sum * x_y_lny_sum) / (y_sum * x2_y_sum - x_y_sum * x_y_sum);            //calculate slope

  double b = (y_sum * x_y_lny_sum - x_y_sum * y_lny_sum) / (y_sum * x2_y_sum - x_y_sum * x_y_sum);            //calculate intercept

  return std::pair<double, double>( a, b);
}

std::vector<double> ExponentialFit(std::vector<double> const &x, double a, double b )
{
  size_t const n = x.size();

  double c = exp(a);                        //since b=ln(c)

  std::vector<double> y_fit(n);                        //an array to store the new fitted values of y    

  for (size_t i = 0; i < n; i++)
  {
    y_fit[i] = c * exp(b * x[i]);                    //to calculate y(fitted) at given x points
  }

  return y_fit;
}

std::vector<double> ExponentialFit(std::vector<double> const &x, std::vector<double> const &y)
{
  std::pair<double, double> coeffs = ExponentialFitCoeffs(x, y);

  return ExponentialFit(x, coeffs.first, coeffs.second);
}


std::pair<double, double> LogarithmFitCoeffs(std::vector<double> const &x, std::vector<double> const &y)
{
  size_t const n = x.size();

  std::vector<double> lnx(n);

  for (size_t i = 0; i < n; i++)                        //Calculate the values of ln(yi)
  {
    lnx[i] = log(x[i]);
  }

  double ysum = 0, lnXsum = 0, ylnXsum = 0, lnX2sum = 0;

  for (size_t i = 0; i < n; i++)
  {
    ysum += y[i];
    lnXsum += lnx[i];
    ylnXsum += y[i] * lnx[i];
    lnX2sum += lnx[i] * lnx[i];
  }

  double b = (n * ylnXsum - ysum * lnXsum) / (n * lnX2sum - lnXsum * lnXsum);

  double a = (ysum - b * lnXsum) / n;

  return std::pair<double, double>(a, b);
}

std::vector<double> LogarithmFit(std::vector<double> const &x, double a, double b)
{
  size_t const n = x.size();

  std::vector<double> y_fit(n);                        //an array to store the new fitted values of y    

  for (size_t i = 0; i < n; i++)
  {
    y_fit[i] = a + b * log(x[i]);
  }

  return y_fit;
}

std::vector<double> LogarithmFit(std::vector<double> const &x, std::vector<double> const &y)
{
  std::pair<double, double> coeffs = LogarithmFitCoeffs(x, y);

  return LogarithmFit(x, coeffs.first, coeffs.second);
}
