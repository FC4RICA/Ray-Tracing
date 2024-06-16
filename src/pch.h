#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


// C++ Std Usings

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

// Common Headers

#include "vec3.h"
#include "color.h"
#include "interval.h"
#include "ray.h"