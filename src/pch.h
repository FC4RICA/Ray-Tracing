#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>


// C++ Std Usings

using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

constexpr double Infinity = std::numeric_limits<double>::infinity();
constexpr double Pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * Pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + ((max - min) * random_double());
}

// Common Headers

#include "vec3.h"
#include "color.h"
#include "interval.h"
#include "ray.h"