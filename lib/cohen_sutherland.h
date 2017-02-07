#pragma once
#include <glut.h>
#include "basic.h"
#include "boundary.h"

namespace clip {

const GLint winLeftBitCode = 0x1;
const GLint winRightBitCode = 0x2;
const GLint winBottomBitCode = 0x4;
const GLint winTopBitCode = 0x8;

inline GLint inside(GLint code) {
    return GLint(!code);
}

inline GLint reject(GLint code1, GLint code2) {
    return GLint(code1 & code2);
}

inline GLint accept(GLint code1, GLint code2) {
    return GLint(!(code1 | code2));
}

inline GLubyte encode(glstd::wcPt2D pt, glstd::wcPt2D winMin, glstd::wcPt2D winMax) {
    GLubyte code = 0x00;
    if (pt.getX() < winMin.getX()) {
        code |= winLeftBitCode;
    }
    if (pt.getX() > winMax.getX()) {
        code |= winRightBitCode;
    }
    if (pt.getY() < winMin.getY()) {
        code |= winBottomBitCode;
    }
    if (pt.getY() > winMax.getY()) {
        code |= winTopBitCode;
    }
    return code;
}

inline void swapPts(glstd::wcPt2D* p1, glstd::wcPt2D* p2) {
    glstd::wcPt2D tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

inline void swapCodes(GLubyte* c1, GLubyte* c2) {
    GLubyte tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

inline void lineClipCohSuth(glstd::wcPt2D winMin, glstd::wcPt2D winMax, glstd::wcPt2D p1, glstd::wcPt2D p2) {
    GLubyte code1, code2;
    GLint done = false, plotLine = false;
    GLfloat m = 0;
    while (!done) {
        code1 = encode(p1, winMin, winMax);
        code2 = encode(p2, winMin, winMax);
        if (accept(code1, code2)) {
            done = true;
            plotLine = true;
        } else if (reject(code1, code2)) {
            done = true;
        } else {
            if (inside(code1)) {
                swapPts(&p1, &p2);
                swapCodes(&code1, &code2);
            }
            if (p2.getX() != p1.getX()) {
                m = (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
            }
            if (code1 & winLeftBitCode) {
                p1.setCoords(winMin.getX(), p1.getY() + (winMin.getX() - p1.getX()) * m);
            } else if (code1 & winRightBitCode) {
                p1.setCoords(winMax.getX(), p1.getY() + (winMax.getX() - p1.getX()) * m);
            } else if (code1 & winBottomBitCode) {
                if (p2.getX() != p1.getX()) {
                    p1.setCoords(p1.getX() + (winMin.getY() - p1.getY()) / m, p1.getY());
                }
                p1.setCoords(p1.getX(), winMin.getY());
            } else if (code1 & winTopBitCode) {
                if (p2.getX() != p1.getX()) {
                    p1.setCoords(p1.getX() + (winMax.getY() - p1.getY()) / m, p1.getY());
                }
                p1.setCoords(p1.getX(), winMax.getY());
            }
        }
    }
    if (plotLine) {
        lines::lineBresenhamImproved(glstd::round(p1.getX()), glstd::round(p1.getY()),
                                     glstd::round(p2.getX()), glstd::round(p2.getY()));
    }
}

} // namespace clip
