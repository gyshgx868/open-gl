#pragma once
#include <glut.h>
#include "boundary.h"

namespace clip {

inline GLint clipTest(GLfloat p, GLfloat q, GLfloat* u1, GLfloat* u2) {
    GLfloat r;
    GLint returnValue = true;
    if (p < 0.0f) {
        r = q / p;
        if (r > *u2) {
            returnValue = false;
        } else if (r > *u1) {
            *u1 = r;
        }
    } else {
        if (p > 0.0f) {
            r = q / p;
            if (r < *u1) {
                returnValue = false;
            } else if (r < *u2) {
                *u2 = r;
            }
        } else {
            if (q < 0.0f) {
                returnValue = false;
            }
        }
    }
    return returnValue;
}

inline void lineClipLiangBarsk(glstd::wcPt2D winMin, glstd::wcPt2D winMax, glstd::wcPt2D p1, glstd::wcPt2D p2) {
    GLfloat u1 = 0.0, u2 = 1.0, dx = p2.getX() - p1.getX(), dy;
    if (!clipTest(-dx, p1.getX() - winMin.getX(), &u1, &u2)) {
        return;
    }
    if (!clipTest(dx, winMax.getX() - p1.getX(), &u1, &u2)) {
        return;
    }
    dy = p2.getY() - p1.getY();
    if (!clipTest(-dy, p1.getY() - winMin.getY(), &u1, &u2)) {
        return;
    }
    if (clipTest(dy, winMax.getY() - p1.getY(), &u1, &u2)) {
        if (u2 < 1.0) {
            p2.setCoords(p1.getX() + u2 * dx, p1.getY() + u2 * dy);
        }
        if (u1 > 0.0) {
            p1.setCoords(p1.getX() + u1 * dx, p1.getY() + u1 * dy);
        }
        lines::lineBresenhamImproved(glstd::round(p1.getX()), glstd::round(p1.getY()),
                                     glstd::round(p2.getX()), glstd::round(p2.getY()));
    }
}

} // namespace clip
