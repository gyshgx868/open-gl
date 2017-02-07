#include <glut.h>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <iostream>

#if 0
#define PI acos(-1.0)
#define R 0.8f

void displayEvent() {
    // ��ȡϵͳʱ��
    auto t = time(nullptr);
    auto *now = localtime(&t);

    glClear(GL_COLOR_BUFFER_BIT);

    // ����
    glColor3ub(0xf0, 0xe6, 0x8c);
    glBegin(GL_POLYGON);
    int n = 12;
    for (int i = 0; i < n; i++) {
        glVertex2f(R * cos(2 * PI / n * i), R * sin(2 * PI / n * i));
    }
    glEnd();

    // �̶�
    glColor3ub(0x66, 0x00, 0x00);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 12; i++) {
        glVertex2f(0.75 * cos(2 * PI / 12 * i), 0.75 * sin(2 * PI / 12 * i));
    }
    glEnd();

    int h = now->tm_hour;
    int m = now->tm_min;
    int s = now->tm_sec;

    // ʱ��
    glPushMatrix();
        glRotatef(-(h - 12 + m / 60.0 + s / 3600.0) * 30.0, 0.0, 0.0, 1.0);
        glLineWidth(5.0f);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, R * 0.55);
        glEnd();
    glPopMatrix();

    // ����
    glPushMatrix();
        glLineWidth(4.0f);
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(-(m + s / 60.0) * 6.0, 0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, R * 0.65);
        glEnd();
    glPopMatrix();

    // ����
    glPushMatrix();
        glLineWidth(3.0f);
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(-s * 6.0, 0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, R * 0.85);
        glEnd();
    glPopMatrix();

    // ʱ������
    char text[10];
    text[0] = '0' + h / 10;
    text[1] = '0' + h % 10;
    text[2] = ':';
    text[3] = '0' + m / 10;
    text[4] = '0' + m % 10;
    text[5] = ':';
    text[6] = '0' + s / 10;
    text[7] = '0' + s % 10;
    text[8] = '\0';
    //std::cout << text << std::endl;
    // ��λ���λ��
    glRasterPos2d(-0.15, -0.5);
    // �����ַ�����ɫ
    glColor3f(0, 0, 0);
    // ����ַ�
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
    }

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(300, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Clock");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glutDisplayFunc(&displayEvent);
    glutIdleFunc(&displayEvent);
    glutMainLoop();
    return 0;
}
#endif

#if 1
#define SOLID 1
#define WIRE 2

int moveX, moveY;
int spinX = 0;
int spinY = 0;
int des = 0;

void init() {
//�����Դ����ɫ��λ��
    GLfloat ambient[] = {0.5, 0.8, 0.1, 0.1};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {-80.0, 50.0, 25.0, 1.0};
//ѡ�����ģ��
    GLfloat lmodel_ambient[] = {0.4, 0.4, 0.4, 1.0};
    GLfloat local_view[] = {0.0};
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
//���û�����
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//���������
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//���ù�Դλ��
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
//��������
    glEnable(GL_LIGHTING);
//���ù�Դ
    glEnable(GL_LIGHT0);
}

//����
void drawBall(double R, double x, double y, double z, int MODE) {
    glPushMatrix();
    glTranslated(x, y, z);
    if (MODE == SOLID) {
        glutSolidSphere(R, 20, 20);
    } else if (MODE == WIRE) {
        glutWireSphere(R, 20, 20);
    }
    glPopMatrix();
}

//������
void drawHalfBall(double R, double x, double y, double z, int MODE) {
    glPushMatrix();
    glTranslated(x, y, z);
    GLdouble eqn[4] = {0.0, 1.0, 0.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0, eqn);
    glEnable(GL_CLIP_PLANE0);
    if (MODE == SOLID) {
        glutSolidSphere(R, 20, 20);
    } else if (MODE == WIRE) {
        glutWireSphere(R, 20, 20);
    }
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}

//��������
void drawSkewed(double l, double w, double h, double x, double y, double z, int MODE) {
    glPushMatrix();
    glScaled(l, w, h);
    glTranslated(x, y, z);
    if (MODE == SOLID) {
        glutSolidCube(1);
    } else if (MODE == WIRE) {
        glutWireCube(1);
    }
    glPopMatrix();
}

void display_event(void) {
//�����������ɫ
    glClear(GL_COLOR_BUFFER_BIT);
//�����ɫ
    glColor3f(1.0, 1.0, 1.0);
//Բ�����������
    glLoadIdentity();
//���ĸ��ط���
    gluLookAt(-2.0, -1.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPushMatrix();
    glRotated(spinX, 0, 1, 0);
    glRotated(spinY, 1, 0, 0);
    glTranslated(0, 0, des);
//ͷ
    drawBall(2, 0, 1, 0, SOLID);
//����
    drawSkewed(5, 4.4, 4, 0, -0.75, 0, SOLID);
//���
    drawHalfBall(1, 3.5, -2.1, 0, SOLID);
    drawHalfBall(1, -3.5, -2.1, 0, SOLID);
//�첲
    drawSkewed(1, 3, 1, 3.5, -1.3, 0, SOLID);
    drawSkewed(1, 3, 1, -3.5, -1.3, 0, SOLID);
//��
    drawBall(1, 3.5, -6.4, 0, SOLID);
    drawBall(1, -3.5, -6.4, 0, SOLID);
//��
    drawSkewed(1.2, 3, 2, 1, -2.4, 0, SOLID);
    drawSkewed(1.2, 3, 2, -1, -2.4, 0, SOLID);
//��
    drawSkewed(1.5, 1, 3, 0.9, -9.2, 0, SOLID);
    drawSkewed(1.5, 1, 3, -0.9, -9.2, 0, SOLID);
    glPopMatrix();
    glutSwapBuffers();
}

//������¼�
void mouseClick(int btn, int state, int x, int y) {
    moveX = x;
    moveY = y;
    printf("(%d, %d)\n", x, y);
//GLfloat ambient[] = { (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.1 };
//���û�����
//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//���ù�Դ
//glEnable(GL_LIGHT0);
}

//�����¼�
void keyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            spinX -= 2;
            break;
        case 'd':
            spinX += 2;
            break;
        case 'w':
            des += 2;
            break;
        case 's':
            des -= 2;
            break;
    }
    glutPostRedisplay();
}

// ����ƶ��¼� 
void mouseMove(int x, int y) {
    int dx = x - moveX;
    int dy = y - moveY;
//printf("dx;%dx,dy:%dy\n", dx, dy);
    spinX += dx;
    spinY += dy;
    glutPostRedisplay();
    moveX = x;
    moveY = y;
}

void reshape(int w, int h) {
//�����ӿڴ�С
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//ͶӰ��ʾ
    glMatrixMode(GL_PROJECTION);
//����ԭ������Ļ����
    glLoadIdentity();
//����ģ���Ӿ�
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
//��ʼ��
    glutInit(&argc, argv);
//������ʾģʽ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//��ʼ�����ڴ�С
    glutInitWindowSize(500, 500);
//�������ϽǴ���λ��
    glutInitWindowPosition(100, 100);
//��������
    glutCreateWindow(argv[0]);
//��ʼ��
    init();
//��ʾ����
    glutDisplayFunc(display_event);
//���ڴ�С�ı�ʱ����Ӧ
    glutReshapeFunc(reshape);
//������¼�������������ɿ�ʱ����
    glutMouseFunc(mouseClick);
//����ƶ��¼�����갴�²��ƶ�ʱ����
    glutMotionFunc(mouseMove);
//�����¼�
    glutKeyboardFunc(keyPressed);
//ѭ��
    glutMainLoop();
    return 0;
}
#endif
