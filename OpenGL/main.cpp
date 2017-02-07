#include <glut.h>
#include <iostream>
#include "boundary.h"
using namespace std;
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

float xs, ys, xe, ye;

void displayEvent() {
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    //glBegin(GL_POINTS);
    // lineMidpoint(xs, ys, xe, ye);
    //lineBresenham(xs, ys, xe, ye);
    //lineBresenham2(xs, ys, xe, ye);

    // 五角星
    /*
    lineBresenham2(400, 550, 195, 50);
    lineBresenham2(400, 550, 605, 50);
    lineBresenham2(110, 365, 690, 365);
    lineBresenham2(110, 365, 605, 50);
    lineBresenham2(690, 365, 195, 50);
     */

    circles::circleMidPoint(xs, ys, sqrt((xs - xe) * (xs - xe) + (ys - ye) * (ys - ye)));
    //glEnd();
    glFlush();
}

// 鼠标事件函数 
void mouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        xs = x;
        ys = 600 - y;
        glutPostRedisplay();
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        xe = x;
        ye = 600 - y;
        glutPostRedisplay();
        displayEvent();
    }
}

int main(int argc, char* argv[]) {
    // 初始化glut
    glutInit(&argc, argv);
    // 设置OpenGL显示模式
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    cout << "Please input the start position and the end position (x, y):\n";
    cin >> xs >> ys >> xe >> ye;
    // 设置窗口的初始位置
    glutInitWindowPosition(50, 100);
    // 设置窗口大小
    glutInitWindowSize(800, 600);
    // 设置窗口标题
    glutCreateWindow("画线算法实习");
    // 设置清除颜色
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800, 0.0, 600.0);

    glClear(GL_COLOR_BUFFER_BIT);

    // 设置显示回调函数
    glutDisplayFunc(displayEvent);
    // 设置鼠标事件回调函数
    glutMouseFunc(mouseEvent);

    glutMainLoop();
    return 0;
}
