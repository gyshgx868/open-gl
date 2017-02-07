#include <glut.h>
#include "liang_barsky.h"
#include "cohen_sutherland.h"
#include "sutherland_hodgman.h"
#include "middle_clip.h"

void displayEvent() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glstd::wcPt2D p1(0, 0), p2(800, 600), winMin(100, 100), winMax(700, 500);
    // �ü��������
    lines::lineBresenhamImproved(100, 100, 700, 100);
    lines::lineBresenhamImproved(100, 100, 100, 500);
    lines::lineBresenhamImproved(100, 500, 700, 500);
    lines::lineBresenhamImproved(700, 500, 700, 100);
    //clip::lineClipCohSuth(winMin, winMax, p1, p2);
    //clip::lineClipLiangBarsk(winMin, winMax, p1, p2);
    //clip::lineMidClip(winMin, winMax, p1, p2);

    glstd::wcPt2D in[6];
    glstd::wcPt2D out[500];
    in[0].setCoords(120, 140);
    in[1].setCoords(120, 490);
    in[2].setCoords(300, 350);
    in[3].setCoords(660, 560);
    in[4].setCoords(660, 210);
    in[5].setCoords(300, 70);
    int cnt = clip::polygonClipSuthHodg(winMin, winMax, 6, in, out);
    for (int i = 0; i < cnt; i++) {
        lines::lineBresenhamImproved(out[i].getX(), out[i].getY(), out[(i + 1) % cnt].getX(), out[(i + 1) % cnt].getY());
    }
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
    glutCreateWindow("�ü��㷨ʵϰ");
    // ���������ɫ
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800, 0.0, 600.0);

    glClear(GL_COLOR_BUFFER_BIT);

    // ������ʾ�ص�����
    glutDisplayFunc(displayEvent);

    glutMainLoop();
    return 0;
}
