#pragma once
#include <glut.h>
#include <math.h>

namespace splines {
#define eps 1e-6

inline int sgn(const GLdouble& x) {
    return x < -eps ? -1 : x > eps;
}

struct Point {
    GLdouble x, y;

    Point(const GLdouble& x = 0, const GLdouble& y = 0) : x(x), y(y) {
    }

    friend Point operator +(const Point& a, const Point& b) {
        return Point(a.x + b.x, a.y + b.y);
    }

    friend Point operator -(const Point& a, const Point& b) {
        return Point(a.x - b.x, a.y - b.y);
    }

    friend Point operator *(const Point& a, const double b) {
        return Point(a.x * b, a.y * b);
    }

    friend Point operator *(const double a, const Point& b) {
        return b * a;
    }

    GLdouble arg() const {
        return atan2(y, x);
    }

    // ÄæÊ±ÕëÐý×ªangle»¡¶È
    Point rotate(const GLdouble& angle) const {
        return rotate(cos(angle), sin(angle));
    }

    Point rotate(const Point& p, const GLdouble& angle) const {
        return (*this - p).rotate(angle) + p;
    }

    Point rotate(const GLdouble& cosa, const GLdouble& sina) const {
        return Point(x * cosa - y * sina, x * sina + y * cosa);
    }
};

inline void binomialCoeffs(GLint n, GLint* c) {
    for (GLint k = 0; k <= n; k++) {
        c[k] = 1;
        for (GLint j = n; j >= k + 1; j--) {
            c[k] *= j;
        }
        for (GLint j = n - k; j >= 2; j--) {
            c[k] /= j;
        }
    }
}

inline void plotPoint(Point bezCurvePt) {
    glBegin(GL_POINTS);
    glVertex2f(bezCurvePt.x, bezCurvePt.y);
    glEnd();
}

inline void computeBezPt(GLfloat u, Point* bezPt, GLint nCtrlPts, Point* CtrlPts, GLint* c) {
    GLint n = nCtrlPts - 1;
    GLfloat bezBlendFcn;
    bezPt->x = bezPt->y = 0.0f;
    for (GLint k = 0; k < nCtrlPts; k++) {
        bezBlendFcn = c[k] * pow(u, k) * pow(1 - u, n - k);
        bezPt->x += CtrlPts[k].x * bezBlendFcn;
        bezPt->y += CtrlPts[k].y * bezBlendFcn;
    }
}

inline void bezier(Point* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts) {
    Point bezCurvePt;
    GLfloat u;
    GLint* c = new GLint[nCtrlPts];
    binomialCoeffs(nCtrlPts - 1, c);
    for (GLint k = 0; k <= nBezCurvePts; k++) {
        u = GLfloat(k) / GLfloat(nBezCurvePts);
        computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, c);
        plotPoint(bezCurvePt);
    }
    delete[] c;
}
} // namespace splines
