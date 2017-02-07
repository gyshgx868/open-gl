#include <glut.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include "boundary.h"

#define PI acos(-1.0)
#define ANGLE_PER_HOUR PI / 6
#define ANGLE_PER_MINUTE PI / 30
#define ANGLE_PER_SECOND PI / 30

template <class T>
T sqr(T x) { return x * x; }

inline int inCircle(int x, int y, int x0, int y0, int radius) {
    return sqr(x - x0) + sqr(y - y0) - sqr(radius);
}

void pointFill(int rowFrom, int colFrom, int rowTo, int colTo, int x, int y, int radius) {
    for (int i = rowFrom; i <= rowTo; i++) {
        for (int j = colFrom; j <= colTo; j++) {
            if (inCircle(i, j, x, y, radius) < 0) {
                glstd::setPixel(i, j);
            }
        }
    }
}

double hourAngle(tm *now) {
    double hours = now->tm_hour + now->tm_min / 60.0 + now->tm_sec / 3600.0;
    return hours * ANGLE_PER_HOUR;
}

double minuteAngle(tm *now) {
    double minutes = now->tm_min + now->tm_sec / 60.0;
    return minutes * ANGLE_PER_MINUTE;
}

double secondAngle(tm *now) {
    double second = now->tm_sec;
    return second * ANGLE_PER_SECOND;
}

void displayEvent() {
    // ��ȡϵͳʱ��
    time_t t = time(nullptr);
    tm *now = localtime(&t);

    // ʹ�������䷨���Ʊ���
    glPointSize(1.0f);
    glColor3ub(160, 82, 45);
    pointFill(0, 0, 800, 600, 400, 300, 270);
    glColor3ub(0, 0, 0);
    circles::circleMidPoint(400, 300, 270);

    glColor3ub(0, 255, 255);
    pointFill(0, 0, 800, 600, 400, 300, 250);
    glColor3f(0.0, 0.0, 0.0);
    circles::circleMidPoint(400, 300, 250);

    // ���ƿ̶�
    glPointSize(6.0f);
    double r = 250 - 20;
    for (int i = 0; i < 12; i++) {
        char text[3];
        int index = 0;
        int value = (12 + i) % 12;
        if (value == 0) {
            value = 12;
        }
        if (value / 10) {
            text[index++] = '0' + value / 10;
        }
        text[index++] = '0' + value % 10;
        text[index] = '\0';
        // ��λ���λ��
        int x = 400 + r * sin(i * ANGLE_PER_HOUR);
        int y = 300 + r * cos(i * ANGLE_PER_HOUR);
        glRasterPos2d(x - 5, y - 7);
        for (int j = 0; text[j] != '\0'; j++) {
            // ����ַ�
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[j]);
        }
        // setPixel(400 + r * sin(i * ANGLE_PER_HOUR), 300 + r * cos(i * ANGLE_PER_HOUR));
    }

    // ����Բ��
    circles::circleMidPoint(400, 300, 3);

    // ʱ��
    lines::lineBresenhamImproved(400, 300, 400 + 100 * sin(hourAngle(now)), 300 + 100 * cos(hourAngle(now)));
    
    // ����
    glPointSize(3.0f);
    lines::lineBresenhamImproved(400, 300, 400 + 150 * sin(minuteAngle(now)), 300 + 150 * cos(minuteAngle(now)));
    
    // ����
    glPointSize(2.0f);
    lines::lineBresenhamImproved(400, 300, 400 + 200 * sin(secondAngle(now)), 300 + 200 * cos(secondAngle(now)));

    glFlush();
}

int main(int argc, char* argv[]) {
    // ��ʼ��glut
    glutInit(&argc, argv);
    // ����OpenGL��ʾģʽ
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    // ���ô��ڵĳ�ʼλ��
    glutInitWindowPosition(50, 100);
    // ���ô��ڴ�С
    glutInitWindowSize(800, 600);
    // ���ô��ڱ���
    glutCreateWindow("ʱ��");
    // ���������ɫ
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800, 0.0, 600.0);

    glClear(GL_COLOR_BUFFER_BIT);

    // ������ʾ�ص�����
    glutDisplayFunc(displayEvent);

    glutIdleFunc(&displayEvent);

    glutMainLoop();
    return 0;
}
