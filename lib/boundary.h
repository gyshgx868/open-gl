#pragma once
#include <glut.h>
#include "basic.h"
#include <cmath>
#include <algorithm>

namespace lines {

inline void lineDDA(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    GLint dx = xEnd - x0, dy = yEnd - y0, steps, k;
    GLfloat xIncrement, yIncrement, x = x0, y = y0;
    if (fabs(dx) > fabs(dy)) {
        steps = fabs(dx);
    } else {
        steps = fabs(dy);
    }
    xIncrement = GLfloat(dx) / GLfloat(steps);
    yIncrement = GLfloat(dy) / GLfloat(steps);

    glstd::setPixel(glstd::round(x), glstd::round(y));
    for (k = 0; k < steps; k++) {
        x += xIncrement;
        y += yIncrement;
        glstd::setPixel(glstd::round(x), glstd::round(y));
    }
}

inline void lineMidpoint(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    GLint a = y0 - yEnd, b = xEnd - x0, d = 2 * a + b;
    GLint d1 = 2 * a, d2 = 2 * (a + b);
    GLint x = x0, y = y0;
    glstd::setPixel(x, y);
    while (x < xEnd) {
        if (d < 0) {
            x++;
            y++;
            d += d2;
        } else {
            x++;
            d += d1;
        }
        glstd::setPixel(x, y);
    }
}

inline void lineBresenham(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    GLint dx = fabs(xEnd - x0), dy = fabs(yEnd - y0);
    GLint p = 2 * dy - dx;
    GLint twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
    GLint x, y;

    if (x0 > xEnd) {
        x = xEnd;
        y = yEnd;
        xEnd = x0;
    } else {
        x = x0;
        y = y0;
    }
    glstd::setPixel(x, y);

    while (x < xEnd) {
        x++;
        if (p < 0) {
            p += twoDy;
        } else {
            y++;
            p += twoDyMinusDx;
        }
        glstd::setPixel(x, y);
    }
}

inline void lineBresenhamImproved(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    if (xEnd < x0) {
        xEnd ^= x0 ^= xEnd ^= x0;
        yEnd ^= y0 ^= yEnd ^= y0;
        //swap(xEnd, x0);
        //swap(yEnd, y0);
    }
    // 画第一个点
    GLint x = x0, y = y0;
    glstd::setPixel(x, y);
    // 直线平行坐标轴单独绘制
    if (y0 == yEnd) {
        // 平行x轴
        x = std::min(x0, xEnd);
        while (x < std::max(x0, xEnd)) {
            x++;
            glstd::setPixel(x, y);
        }
        return;
    }
    if (x0 == xEnd) {
        // 平行y轴
        y = std::min(y0, yEnd);
        while (y < std::max(y0, yEnd)) {
            y++;
            glstd::setPixel(x, y);
        }
        return;
    }

    GLint dx = xEnd - x0, dy = yEnd - y0, p;
    GLint twoDy = dy << 1, twoDyMinusDx = (dy - dx) << 1;
    GLint twoDx = dx << 1, twoDxMinusDy = (dx - dy) << 1;
    GLint twoDxAddDy = (dx + dy) << 1;
    GLfloat k = GLfloat(dy) / GLfloat(dx);

    // 0 < k < 1
    if (k < 1.0f && k > 0.0f) {
        p = (dy << 1) - dx;
        while (x < xEnd) {
            x++;
            if (p < 0) {
                p += twoDy;
            } else {
                y++;
                p += twoDyMinusDx;
            }
            glstd::setPixel(x, y);
        }
    }

    // k >= 1
    if (k >= 1.0f) {
        p = dy;
        while (y < yEnd) {
            y++;
            if (p < 0) {
                p += twoDx;
            } else {
                x++;
                p += twoDxMinusDy;
            }
            glstd::setPixel(x, y);
        }
    }
    // 0 > k > -1
    if (k > -1.0f && k < 0.0f) {
        p = (dy << 1) + dx;
        while (x < xEnd) {
            x++;
            if (p >= 0) {
                p += twoDy;
            } else {
                y--;
                p += twoDxAddDy;
            }
            glstd::setPixel(x, y);
        }
    }
    // k < -1
    if (k <= -1.0f) {
        p = (dx << 1) - dy;
        while (y > yEnd) {
            y--;
            if (p >= 0) {
                p -= twoDx;
            } else {
                x++;
                p -= twoDxAddDy;
            }
            glstd::setPixel(x, y);
        }
    }
}

} // namespace llines

namespace circles {

inline void __circlePlotPoints(GLint xc, GLint yc, glstd::screenPt circPt) {
    glstd::setPixel(xc + circPt.getX(), yc + circPt.getY());
    glstd::setPixel(xc - circPt.getX(), yc + circPt.getY());
    glstd::setPixel(xc + circPt.getX(), yc - circPt.getY());
    glstd::setPixel(xc - circPt.getX(), yc - circPt.getY());

    glstd::setPixel(xc + circPt.getY(), yc + circPt.getX());
    glstd::setPixel(xc - circPt.getY(), yc + circPt.getX());
    glstd::setPixel(xc + circPt.getY(), yc - circPt.getX());
    glstd::setPixel(xc - circPt.getY(), yc - circPt.getX());
}

inline void circleMidPoint(GLint xc, GLint yc, GLint radius) {
    glstd::screenPt circPt;
    GLint p = 1 - radius;
    circPt.setCoords(0, radius);
    __circlePlotPoints(xc, yc, circPt);

    while (circPt.getX() < circPt.getY()) {
        circPt.incrementX();
        if (p < 0) {
            p += 2 * circPt.getX() + 1;
        } else {
            circPt.decrementY();
            p += 2 * (circPt.getX() - circPt.getY()) + 1;
        }
        __circlePlotPoints(xc, yc, circPt);
    }
}

} // namespace circles
