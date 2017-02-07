#include <glut.h>
#include <math.h>
#include <iostream>
#include "bezier.h"
#include "b_spline.h"
using namespace std;

#if 0
class Point
{
public:
    Point() : x(0.0), y(0.0), w(1.0) {};
    Point(float x, float y, float w) : x(x), y(y), w(w) {};
    float x;
    float y;
    float w;
};

/**
*
* class Decasteljau. class that contains points, lines and the curve
*
**/
class Deboor {

public:
    Deboor() {
        knots[0] = 0;
        knots[1] = 0;
        knots[2] = 0;
        knots[3] = 0;
        knots[4] = 1;
        knots[5] = 2;
        knots[6] = 3;
        knots[7] = 3;
        knots[8] = 3;
        knots[9] = 3;
    }

    void reshape(int w, int h) {
        _height = h;
        _width = w;
    }

    void MouseClick(int x, int y) {
        // convert to our y coordinate
        float vy = -1 * (y - _height);

        printf("adding point x y = %d %f \n", x, vy);
        _points[iter] = new Point(x, vy, 0);
        iter++;
    }

    // reset all the points
    void RightMouseClick() {
        for (int i = 0; i <= iter; i++) {
            if (_points[i] != NULL)
                // unset all point
                _points[i] = NULL;
        }
        // reset point counter
        iter = 0;
    }

    // methods to call all draw colors
    void Draw() {
        drawPoints();
        drawStraightLines();
        drawBSplines();
        glFlush();
    }

private:
    int _height = 480;
    int _width = 600;

    int iter = 0;
    Point* _points[100];
    const float _interpolateStep = 0.01;
    const int degree = 3;

    float knots[10];

    ///
    /// draw line from color 1 to color 2
    ///
    void drawStraightLines() {
        glColor3f(1, 0, 0);

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= iter; i++) {
            Point* point2 = _points[i];
            if (point2 != NULL) {
                glVertex2f(point2->x, point2->y);
            }
        }
        glEnd();
    }

    // 
    float deBoor(int n, int i, float t, float* knots)
    {
        if (n == 0)
        {
            if ((knots[i] <= t) && (t < knots[i + 1]))
                return 1;

            return 0;
        }
        else
        {
            float denominator1 = knots[i + n] - knots[i];
            float denominator2 = knots[i + n + 1] - knots[i + 1];
            float quotient1 = (denominator1 == 0) ? 0 : (t - knots[i]) / denominator1;
            float quotient2 = (denominator2 == 0) ? 0 : (knots[i + n + 1] - t) / denominator2;

            return quotient1 * deBoor(n - 1, i, t, knots) + quotient2 * deBoor(n - 1, i + 1, t, knots);
        }
    }

    void drawBSplines()
    {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);
        for (float t = 0; t <= degree; t += _interpolateStep)
        {
            Point p;
            for (int i = 0; i < iter; ++i)
            {
                float deBoorCoefficient = deBoor(degree, i, t, knots);
                p.x += _points[i]->x * deBoorCoefficient;
                p.y += _points[i]->y * deBoorCoefficient;
            }
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }


    ///
    ///
    ///
    void drawPoints() {
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(5);

        glBegin(GL_POINTS);
        for (int i = 0; i <= iter; i++) {
            Point* point = _points[i];
            if (point != NULL) {
                glVertex2f(point->x, point->y);
            }
        }
        glEnd();
    }

};

int width = 640;
int height = 480;

Deboor deboor;

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glEnable(GL_POINT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0f, 1.0f);
}

void reshape(int w, int h) {
    width = w;
    height = h;

    deboor.reshape(w, h);

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 0.0f, 1.0f);
}

// method to be passed to glutdisplayfunc to draw the triangle lines and colors
void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    deboor.Draw();

    glFlush();
}

void mouseMap(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
            deboor.MouseClick(x, y);
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
            deboor.RightMouseClick();
    }

    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(width, height);
    glutCreateWindow("Cox-deBoor");

    init();

    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseMap);

    glutMainLoop();
    return 0;
}
#endif

#if 1
splines::Point points[1000];
GLint cur = 0;

void b_splines(int n) {
    GLfloat dt = 1.0 / n;
    for (int j = 0; j < cur - 3; j++) {
        for (GLfloat t = 0.0; t < 1.0; t += dt) {
            GLfloat f1 = (-t * t * t + 3 * t * t - 3 * t + 1) / 6.0;
            GLfloat f2 = (3 * t * t * t - 6 * t * t + 4) / 6.0;
            GLfloat f3 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6.0;
            GLfloat f4 = t * t * t / 6.0;
            plotPoint(f1 * points[j] + f2 * points[j + 1] + f3 * points[j + 2] + f4 * points[j + 3]);
        }
    }
}

void displayEvent() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制对齐线
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    for (int i = 0; i < cur; i++) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();

    glColor3f(1.0, 0, 0);
    if (cur >= 4) {
        b_splines(1000);
    }
    glFlush();
}

void mouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        points[cur].x = x;
        points[cur].y = y;
        cur++;
        
        //if (cur == 1) {
        //    points[cur].x = x;
        //    points[cur].y = y;
        //    cur++;
        //}
        glutPostRedisplay();
    }
}

void reshapeEvent(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("B-Splines");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glutDisplayFunc(displayEvent);
    glutReshapeFunc(reshapeEvent);
    glutMouseFunc(mouseEvent);
    glutMainLoop();
    return 0;
}
#endif

#if 0
GLsizei winWidth = 600;
GLsizei winHeight = 600;

typedef splines::Point Vector;

splines::Point first[4], second[4];
Vector first_vec[3], second_vec[3];
GLint cur = 0;

#if 0
// Bezier 曲面
void bezierSurface(GLint nBezCurvePts) {
    for (GLfloat u = 0; u < 1; u += 1.0 / nBezCurvePts) {
        for (GLfloat v = 0; v < 1; v += 1.0 / nBezCurvePts) {
            wcPt3D pt;
            pt.x = (1 - u) * (1 - v) * ctrlPts[0].x + (1 - u) * v * ctrlPts[1].x
                + (1 - v) * u * ctrlPts[2].x + u * v * ctrlPts[3].x;
            pt.y = (1 - u) * (1 - v) * ctrlPts[0].y + (1 - u) * v * ctrlPts[1].y
                + (1 - v) * u * ctrlPts[2].y + u * v * ctrlPts[3].y;
            pt.z = 0.0f;
            plotPoint(pt);
        }
    }
}
#endif

void displayEvent() {
    GLint nCtrlPts = 4, nBezCurvePts = 1000;
    glPointSize(4);
    glColor3f(1.0f, 0.0f, 0.0f);
//bezier(ctrlPts, nCtrlPts, nBezCurvePts);
//bezierSurface(nBezCurvePts);
    //splines::b_splines(first);
    glFlush();
}

void reshapeEvent(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winWidth, winHeight, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void mouseEvent(GLint button, GLint state, GLint x, GLint y) {
#if 0
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        first[cur].x = x;
        first[cur].y = y;
        cout << x << ", " << y << endl;
        cur++;
        if (cur == 4) {
            cur = 0;
            glutPostRedisplay();
        }
    }
#else
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (cur == 0) {
            glClear(GL_COLOR_BUFFER_BIT);
        }
        if (cur < 4) {
            first[cur].x = x;
            first[cur++].y = y;
// printf("first: %d %d\n", x, y);
        } else {
            second[cur - 4].x = x;
            second[cur - 4].y = y;
            cur++;
// printf("second: %d %d\n", x, y);
        }
        if (cur == 4) {
            splines::bezier(first, 4, 1000);
            glFlush();
        }
        if (cur == 8) {
            splines::bezier(second, 4, 1000);
            glFlush();
            cur = 0;
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
// move
        double dx = second[0].x - first[3].x;
        double dy = second[0].y - first[3].y;
        for (int i = 0; i < 4; i++) {
            second[i].x -= dx;
            second[i].y -= dy;
        }
        for (int i = 1; i < 4; i++) {
            first_vec[i - 1] = first[i] - first[i - 1];
        }
        for (int i = 1; i < 4; i++) {
            second_vec[i - 1] = second[i] - second[i - 1];
        }

// before rotate
        double angle_first = first_vec[2].arg();
        double angle_second = second_vec[0].arg();

// debug info
/*
printf("First angle: %.2f\n", angle_first);
printf("Second angle: %.2f\n", angle_second);
*/

// after rotate
        for (int i = 0; i < 3; i++) {
            second_vec[i] = second_vec[i].rotate(angle_first - angle_second);
        }
        

        second[1] = second_vec[0] + second[0];
        second[2] = second_vec[1] + second[1];
        second[3] = second_vec[2] + second[2];

// debug info
/*
angle_first = first_vec[2].arg();
angle_second = second_vec[0].arg();
printf("First angle: %.2f\n", angle_first);
printf("Second angle: %.2f\n", angle_second);
*/

        glClear(GL_COLOR_BUFFER_BIT);
        bezier(first, 4, 1000);
        bezier(second, 4, 1000);
        glFlush();
    }
#endif
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Bezier Curve");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(displayEvent);
    glutReshapeFunc(reshapeEvent);
    glutMouseFunc(mouseEvent);
    glutMainLoop();
    return 0;
}
#endif
