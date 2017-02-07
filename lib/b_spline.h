#pragma once
#include <glut.h>
#include "bezier.h"

namespace splines {

inline GLdouble f(GLint i, GLdouble t) {
    if (i == 0) {
        return (-t * t * t + 3 * t * t - 3 * t + 1) / 6.0;
    }
    if (i == 1) {
        return (3 * t * t * t - 6 * t * t + 4) / 6.0;
    }
    if (i == 2) {
        return (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6.0;
    }
    return t * t * t / 6.0;
}

inline void b_splines(Point *points) {
    Point temp_points[4];
    for (GLdouble t = 0; t < 1; t += 0.001) {
        for (int i = 0; i < 4; i++) {
            temp_points[i] = f(i, t) * points[3 - i];
        }
        for (int i = 0; i < 4; i++) {
            splines::plotPoint(temp_points[i]);
        }
    }
}

} // namespace b_spline