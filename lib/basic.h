#pragma once
#include <glut.h>

namespace glstd {

class wcPt2D {
private:
    GLfloat x, y;

public:
    explicit wcPt2D(GLfloat x = 0.0f, GLfloat y = 0.0f) {
        this->x = x;
        this->y = y;
    }

    void setCoords(GLfloat xCoord, GLfloat yCoord) {
        x = xCoord;
        y = yCoord;
    }

    GLfloat getX() const {
        return x;
    }

    GLfloat getY() const {
        return y;
    }
};

class screenPt {
private:
    GLint x, y;

public:
    explicit screenPt(GLint x = 0, GLint y = 0) {
        this->x = x;
        this->y = y;
    }

    void setCoords(GLint xCoord, GLint yCoord) {
        x = xCoord;
        y = yCoord;
    }

    GLint getX() const {
        return x;
    }

    GLint getY() const {
        return y;
    }

    void incrementX() {
        ++x;
    }

    void decrementY() {
        --y;
    }
};

inline GLint round(const GLfloat& x) {
    return GLint(x + 0.5);
}

inline void setPixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

template <class T>
T sqr(T x) {
    return x * x;
}

} // namespace glstd
