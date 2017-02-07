#pragma once
#include <glut.h>
#include "cohen_sutherland.h"
#include "basic.h"
#include "boundary.h"

namespace clip {

inline glstd::wcPt2D clip(glstd::wcPt2D winMin, glstd::wcPt2D winMax, glstd::wcPt2D p1, glstd::wcPt2D p2) {
    glstd::wcPt2D pt;
    while (true) {
        GLint code1 = encode(p1, winMin, winMax);
        GLint code2 = encode(p2, winMin, winMax);
        GLfloat tempX = (p1.getX() + p2.getX()) / 2;
        GLfloat tempY = (p1.getY() + p2.getY()) / 2;
        GLfloat d1 = glstd::sqr(tempX - p1.getX());
        GLfloat d2 = glstd::sqr(tempY - p1.getY());
        glstd::wcPt2D mid(tempX, tempY);
        GLint code = encode(mid, winMin, winMax);
        if (code1 & code2) {
            break;
        }
        if (code2 == 0) {
            tempX = p2.getX();
            tempY = p2.getY();
            pt.setCoords(tempX, tempY);
            break;
        }
        GLfloat d = sqrt(d1 + d2);
        if (d <= 1) {
            pt.setCoords(tempX, tempY);
            break;
        }
        if (code2 & code) {
            p2.setCoords(tempX, tempY);
        } else {
            p1.setCoords(tempX, tempY);
        }
    }
    return pt;
}

inline void lineMidClip(glstd::wcPt2D winMin, glstd::wcPt2D winMax, glstd::wcPt2D p1, glstd::wcPt2D p2) {
    glstd::wcPt2D src = clip(winMin, winMax, p1, p2);
    glstd::wcPt2D dest = clip(winMin, winMax, p2, p1);
    lines::lineBresenhamImproved(src.getX(), src.getY(), dest.getX(), dest.getY());
}

}
