// SPDX-FileCopyrightText: 2024 The OpenSn Authors <https://open-sn.github.io/opensn/>
// SPDX-License-Identifier: MIT

#ifndef LEBEDEV_QUADRATURE_H
#define LEBEDEV_QUADRATURE_H

#include "framework/math/quadratures/angular/angular_quadrature.h"
#include <vector>
#include <string>
#include <map>

namespace opensn
{

/**
 * @brief Implementation of Lebedev quadrature for angular integration on the surface of a sphere.
 *
 * Lebedev quadrature provides a set of points on the surface of a sphere that allows for
 * symmetric and efficient angular integration. This implementation reads point data from
 * files and constructs the quadrature set.
 */
class LebedevQuadrature : public AngularQuadrature
{
public:
  /**
   * @brief Constructor for Lebedev quadrature.
   *
   * @param order The order of the Lebedev quadrature set to load
   * @param base_path Base path to the directory containing order files
   * @param verbose Flag to enable verbose output
   */
  LebedevQuadrature(int order,
                    const std::string& base_path = "lebedev_orders",
                    bool verbose = false);

  /**
   * @brief Loads quadrature points from a file for the specified order.
   *
   * File is expected to be named: base_path/lebedev_order##.csv where ## is the order number.
   * lebedev_order##.csv is formatted to contain X, Y, Z, W pairs in that order for a given order.
   *
   * @param order The order to load
   * @param base_path Base path to the directory containing order files
   * @param verbose Flag to enable verbose output
   */
  void
  LoadFromFile(int order, const std::string& base_path = "lebedev_orders", bool verbose = false);

  /**
   * @brief Optimizes the quadrature set for the first octant (x ≥ 0, y ≥ 0, z ≥ 0).
   *
   * This method reduces the quadrature set to only points in the first octant,
   * which is useful for problems with symmetry across the coordinate planes.
   *
   * @param normalization The value to normalize the weight sum to (default 0 = no normalization)
   * @param verbose Flag to enable verbose output
   */
  void OptimizeForOctant(const double normalization = 0.0, bool verbose = false);
};

} // namespace opensn

#endif // LEBEDEV_QUADRATURE_H