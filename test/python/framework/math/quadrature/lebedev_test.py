#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
This script demonstrates usage of the LebedevQuadrature class by creating
quadratures of different orders, examining their weights, and testing
the octant optimization functionality.
"""

import os


def split_string(input_str, separator):
    """
    Splits the input_str using the specified separator.
    """
    return input_str.split(separator)


def get_weight_sum(quad_points_file):
    """
    Opens a quadrature points file, reads each line,
    splits the line to extract the 4th value (index 3) as a float,
    and sums these values.
    """
    weight_sum = 0.0
    try:
        with open(quad_points_file, "r") as file:
            file.readline()  # Skip header
            for line in file:
                values = split_string(line.strip(), ",")
                weight_sum += float(values[3])
    except Exception as e:
        print(f"Error: Could not open file {quad_points_file}: {e}")
    return weight_sum


def write_quadrature_to_file(quadrature, filename):
    """
    Writes the quadrature points and weights to a CSV file.
    """
    with open(f"{filename}_points.csv", "w") as file:
        # Write header
        file.write("x,y,z,weight\n")
        # Write each point
        for i in range(len(quadrature.weights)):
            omega = quadrature.omegas[i]
            weight = quadrature.weights[i]
            file.write(f"{omega.x},{omega.y},{omega.z},{weight}\n")


def find_opensn_root(current_path=None):
    """Find the opensn root directory by traversing up from the current path."""
    if current_path is None:
        current_path = os.path.dirname(os.path.abspath(__file__))

    # Start from the current directory and traverse up
    path = current_path
    while True:
        # Check if we've reached the root of the filesystem
        if path == os.path.dirname(path):
            raise RuntimeError("Could not find 'opensn' directory in path hierarchy")

        # Check if this directory or its parent is named 'opensn'
        if os.path.basename(path) == "opensn":
            return path

        # Move up one directory
        path = os.path.dirname(path)


root_path = find_opensn_root()
quad_path = os.path.join(root_path, "framework/math/quadratures/angular/lebedev_orders")

# --- Lebedev Quadrature Test 1 ---
# Create a Lebedev quadrature of order 7
print("\n--- Testing Lebedev Quadrature, Order 7 ---")
leb_quad1 = LebedevQuadrature(7, quad_path)

# Write quadrature to file for inspection
write_quadrature_to_file(leb_quad1, "LebedevTest1")
quad1_sum = get_weight_sum("LebedevTest1_points.csv")
print(f"Weight-Sum-1={quad1_sum:.3e}\n\n")

# --- Lebedev Quadrature Test 2 ---
# Create a higher order Lebedev quadrature
print("\n--- Testing Lebedev Quadrature, Order 15 ---")
leb_quad2 = LebedevQuadrature(15, quad_path)

# Write optimized quadrature to file
write_quadrature_to_file(leb_quad2, "LebedevTest2")
quad2_sum = get_weight_sum("LebedevTest2_points.csv")
print(f"Weight-Sum-2={quad2_sum:.3e}\n\n")

# Clean up: remove generated files.
os.system("rm LebedevTest1* LebedevTest2*")
