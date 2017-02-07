#pragma once
#include <glut.h>
#include "basic.h"

namespace clip {

typedef enum { Left, Right, Bottom, Top } Boundary;

const GLint nClip = 4;

struct Node {
    glstd::wcPt2D point;
    bool bound;

    Node() {
        bound = false;
    }
};

inline GLint inside(glstd::wcPt2D p, Boundary b, glstd::wcPt2D winMin, glstd::wcPt2D winMax) {
    switch (b) {
        case Left:
            if (p.getX() < winMin.getX()) {
                return false;
            }
            break;
        case Right:
            if (p.getX() > winMax.getX()) {
                return false;
            }
            break;
        case Bottom:
            if (p.getY() < winMin.getY()) {
                return false;
            }
            break;
        case Top:
            if (p.getY() > winMax.getY()) {
                return false;
            }
            break;
        default:
            break;
    }
    return true;
}

inline GLint cross(glstd::wcPt2D p1, glstd::wcPt2D p2, Boundary winEdge, glstd::wcPt2D winMin, glstd::wcPt2D winMax) {
    if (inside(p1, winEdge, winMin, winMax) == inside(p2, winEdge, winMin, winMax)) {
        return false;
    }
    return true;
}

inline glstd::wcPt2D intersect(glstd::wcPt2D p1, glstd::wcPt2D p2, Boundary winEdge, 
                               glstd::wcPt2D winMin, glstd::wcPt2D winMax) {
    glstd::wcPt2D iPt;
    GLfloat m = 0;
    if (p1.getX() != p2.getX()) {
        m = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
    }
    switch (winEdge) {
        case Left:
            iPt.setCoords(winMin.getX(), p2.getY() + (winMin.getX() - p2.getX()) * m);
            break;
        case Right:
            iPt.setCoords(winMax.getX(), p2.getY() + (winMax.getX() - p2.getX()) * m);
            break;
        case Bottom:
            if (p1.getX() != p2.getX()) {
                iPt.setCoords(p2.getX() + (winMin.getY() - p2.getY()) / m, winMin.getY());
            } else {
                iPt.setCoords(p2.getX(), winMin.getY());
            }
            break;
        case Top:
            if (p1.getX() != p2.getX()) {
                iPt.setCoords(p2.getX() + (winMax.getY() - p2.getY()) / m, winMax.getY());
            } else {
                iPt.setCoords(p2.getX(), winMax.getY());
            }
            break;
        default: break;
    }
    return iPt;
}

inline void clipPoint(glstd::wcPt2D p, Boundary winEdge, glstd::wcPt2D winMin, glstd::wcPt2D winMax,
                      glstd::wcPt2D* pOut, int* cnt, Node first[], glstd::wcPt2D* s) {
    glstd::wcPt2D iPt;
    if (!first[winEdge].bound) {
        first[winEdge].point = p;
        first[winEdge].bound = true;
    } else {
        if (cross(p, s[winEdge], winEdge, winMin, winMax)) {
            iPt = intersect(p, s[winEdge], winEdge, winMin, winMax);
            if (winEdge < Top) {
                clipPoint(iPt, Boundary(winEdge + 1), winMin, winMax, pOut, cnt, first, s);
            } else {
                pOut[*cnt] = iPt;
                (*cnt)++;
            }
        }
    }

    s[winEdge] = p;
    if (inside(p, winEdge, winMin, winMax)) {
        if (winEdge < Top) {
            clipPoint(p, Boundary(winEdge + 1), winMin, winMax, pOut, cnt, first, s);
        } else {
            pOut[*cnt] = p;
            (*cnt)++;
        }
    }
}

inline void closeClip(glstd::wcPt2D winMin, glstd::wcPt2D winMax, glstd::wcPt2D* pOut, GLint* cnt,
                      Node first[], glstd::wcPt2D* s) {
    glstd::wcPt2D pt;
    for (int winEdge = Left; winEdge <= Top; winEdge++) {
        if (cross(s[winEdge], first[winEdge].point, Boundary(winEdge), winMin, winMax)) {
            pt = intersect(s[winEdge], first[winEdge].point, Boundary(winEdge), winMin, winMax);
            if (winEdge < Top) {
                clipPoint(pt, Boundary(winEdge + 1), winMin, winMax, pOut, cnt, first, s);
            } else {
                pOut[*cnt] = pt;
                (*cnt)++;
            }
        }
    }
}

inline GLint polygonClipSuthHodg(glstd::wcPt2D winMin, glstd::wcPt2D winMax, GLint n,
                                 glstd::wcPt2D* pIn, glstd::wcPt2D* pOut) {
    Node first[nClip];
    glstd::wcPt2D s[nClip];
    GLint cnt = 0;
    for (int k = 0; k < n; k++) {
        clipPoint(pIn[k], Left, winMin, winMax, pOut, &cnt, first, s);
    }
    closeClip(winMin, winMax, pOut, &cnt, first, s);
    return cnt;
}

} // namespace clip
