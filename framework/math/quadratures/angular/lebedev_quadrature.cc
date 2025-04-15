// SPDX-FileCopyrightText: 2024 The OpenSn Authors <https://open-sn.github.io/opensn/>
// SPDX-License-Identifier: MIT

#include "framework/math/quadratures/angular/lebedev_quadrature.h"
#include "framework/logging/log.h"
#include "framework/runtime.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace opensn
{

LebedevQuadrature::LebedevQuadrature(int order, const std::string& base_path, bool verbose)
  : AngularQuadrature(AngularQuadratureType::LebedevQuadrature, 3)
{
  LoadFromFile(order, base_path, verbose);
}

void
LebedevQuadrature::LoadFromFile(int order, const std::string& base_path, bool verbose)
{
  // Construct the filename
  std::ostringstream filename;
  filename << base_path << "/lebedev_order" << std::setfill('0') << std::setw(3) << order << ".csv";
  std::ifstream file(filename.str());
  if (!file.is_open())
    throw std::invalid_argument("LebedevQuadrature: Failed to open file: " + filename.str());

  abscissae.clear();
  weights.clear();
  omegas.clear();

  std::string line;

  // Read all points from the file
  std::stringstream ostr;
  double weight_sum = 0.0;
  int point_count = 0;

  while (std::getline(file, line))
  {
    if (line.empty())
      continue;

    std::istringstream ss(line);
    double x, y, z, w;
    char comma;

    // Format: X, Y, Z, Weight
    ss >> x >> comma >> y >> comma >> z >> comma >> w;

    // Calculate phi and theta from x, y, z
    double r = sqrt(x * x + y * y + z * z);
    double theta = acos(z / r);
    double phi = atan2(y, x);
    if (phi < 0.0)
      phi += 2.0 * M_PI;

    // Create the point
    QuadraturePointPhiTheta qpoint(phi, theta);
    abscissae.push_back(qpoint);

    // Create the direction vector
    Vector3 omega{x, y, z};
    omegas.push_back(omega);

    // Store the weight
    weights.push_back(w);
    weight_sum += w;

    if (verbose)
    {
      char buf[200];
      snprintf(buf,
               200,
               "Varphi=%.2f Theta=%.2f Weight=%.3e\n",
               qpoint.phi * 180.0 / M_PI,
               qpoint.theta * 180.0 / M_PI,
               w);
      ostr << buf;
    }

    point_count++;
  }

  // If no points found
  if (abscissae.empty())
    throw std::invalid_argument("LebedevQuadrature: No quadrature points found in file: " +
                                filename.str());

  if (verbose)
  {
    log.Log() << "Loaded " << point_count << " Lebedev quadrature points from order " << order
              << " file";
    log.Log() << ostr.str() << "\n"
              << "Weight sum=" << weight_sum;
  }

  // Check weight sum (should be 1 for 3D)
  const double expected_sum = 1.0;
  if (fabs(weight_sum - expected_sum) > 1.0e-10)
  {
    if (verbose)
    {
      log.Log() << "Warning: Sum of weights differs from expected value 1.";
      log.Log() << "Expected: " << expected_sum << ", Actual: " << weight_sum;
    }

    // Normalize weights
    const double scale_factor = expected_sum / weight_sum;
    for (auto& w : weights)
      w *= scale_factor;

    if (verbose)
      log.Log() << "Weights have been normalized to sum to 1.";
  }
}

// Method to get octant optimized quadrature
void
LebedevQuadrature::OptimizeForOctant(const double normalization, bool verbose)
{
  std::vector<QuadraturePointPhiTheta> new_abscissae;
  std::vector<double> new_weights;
  std::vector<Vector3> new_omegas;

  // First octant: x ≥ 0, y ≥ 0, z ≥ 0
  double weight_sum = 0.0;

  for (size_t i = 0; i < abscissae.size(); ++i)
  {
    const auto& omega = omegas[i];

    if (omega.x >= 0.0 && omega.y >= 0.0 && omega.z >= 0.0)
    {
      new_abscissae.push_back(abscissae[i]);
      new_omegas.push_back(omega);
      new_weights.push_back(weights[i]);
      weight_sum += weights[i];
    }
  }

  // Normalize weights if requested
  if (normalization > 0.0)
  {
    double scale_factor = normalization / weight_sum;
    for (auto& w : new_weights)
      w *= scale_factor;
  }

  // Replace original data with octant data
  abscissae = new_abscissae;
  omegas = new_omegas;
  weights = new_weights;

  if (verbose)
  {
    log.Log() << "Optimized for first octant. Remaining points: " << abscissae.size();
    log.Log() << "Weight sum after optimization: " << weight_sum;
    if (normalization > 0.0)
      log.Log() << "Weights normalized to sum to " << normalization;
  }
}

} // namespace opensn