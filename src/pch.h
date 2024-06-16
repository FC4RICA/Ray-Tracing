#pragma once

#include <cmath>
#include <iostream>
#include <memory>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

constexpr double PI = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

// Common Headers

#include "vec3.h"
#include "color.h"
#include "ray.h"