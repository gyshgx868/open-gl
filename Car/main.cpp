#include <windows.h>
#include <GL/glu.h>
#include <glut.h>

const int screenWidth = 640; // width of screen window in pixels
const int screenHeight = 480; // height of screen window in pixels
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glOrtho(-100, 100, -100, 100, -100, 100);
}

void changeSize(int w, int h) {
    // ��ֹ�������߶�Ϊ0
    // (��������ô��ڿ��Ϊ0).
    if (h == 0) {
        h = 1;
    }

    float ratio = 1.0 * w / h;

    // ��λ��ͶӰ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // �����ӿڴ�СΪ�������ڴ�С
    glViewport(0, 0, w, h);

    // ������ȷ��ͶӰ����
    gluPerspective(45, ratio, 1, 1000);

    //����������ģ����ͼ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
}

float angle = 0.0;
float trans = 0.0;

void renderScene(float x, float y, float z) {
    //ע�����������������Ȼ�������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //���浱ǰģ����ͼ����
    glPushMatrix();
    //4X4��ת�������ɺ���
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1, 0, 0);
    glTranslated(x, y, z);
    glutSolidTorus(2, 5, 5, 100);

    // ������ջ
    glPopMatrix();

    // ����������
    glutSwapBuffers();

    // ��angle�Զ����ӡ�
    //angle++;
}

//������̿��ƺ���
void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        xRot -= 5.0f;
    if (key == GLUT_KEY_DOWN)
        xRot += 5.0f;
    if (key == GLUT_KEY_LEFT) {
        yRot -= 5.0f;
        trans += 2.0;
    }
    if (key == GLUT_KEY_RIGHT) {
        yRot += 5.0f;
        trans -= 2.0;
    }
    glutPostRedisplay(); // ˢ�´���
}

void timefuncupdate(int value) {
    if (trans <= -50) {
        trans = -50.0;
        trans += 2.0;
    } else if (trans >= 50) {
        trans = 50.0;
        trans -= 2.0;
    } else
        trans += 2.0;
    angle++;
    glutPostRedisplay();
    glutTimerFunc(2000, timefuncupdate, 0);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(trans, 0.0, 0.0);
    glPushMatrix();
    //glRotatef(xRot, 1.0f, 0.0f, 0.0f);  //��x����ת
    //glRotatef(yRot, 0.0f, 1.0f, 0.0f);  //��y����ת
    //���°벿
    glBegin(GL_POLYGON);
    glColor3f(0.8, 0.8, 0.1);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glColor3f(1, 0.5, 0.8);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.2, 0.3);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.7, 0.9, 0.7);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.1, 0.1, 0.8);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glColor3f(1, 0.5, 0.8);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.8, 0.5, 0.2);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glEnd();
    //���ϰ벿
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex3f(-20.0f, 0.0f, 15.0f);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glVertex3f(25.0f, 0.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex3f(-20.0f, 0.0f, -15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 1, 1);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.8, 0.8);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(-20.0f, 0.0f, 15.0f);
    glVertex3f(-20.0f, 0.0f, -15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0.5, 0.5);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glVertex3f(-30.0f, -15.0f, 15.0f);
    glVertex3f(-30.0f, -15.0f, -15.0f);
    glVertex3f(30.0f, -15.0f, -15.0f);
    glVertex3f(30.0f, -15.0f, 15.0f);
    glEnd();
    //����
    /*renderScene(-20.0f,-15.0f,15.0f);
    renderScene(0.0f,0.0f,-30.0f);
    renderScene(45.0f,0.0f,0.0f);
    renderScene(0.0f,0.0f,30.0f);
    */
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslated(-20.0f, -15.0f, 15.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glutSolidTorus(2, 5, 5, 100);
    glPopMatrix();
    glPushMatrix();
    //glTranslated(0.0f,0.0f,-30.0f);
    glTranslated(-20.0f, -15.0f, -15.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glutSolidTorus(2, 5, 5, 100);
    glPopMatrix();
    glPushMatrix();
    glTranslated(25.0f, -15.0f, -15.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glutSolidTorus(2, 5, 5, 100);
    glPopMatrix();
    glPushMatrix();
    glTranslated(25.0f, -15.0f, 15.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glutSolidTorus(2, 5, 5, 100);
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers(); //��������
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);

    // ����������˫��������
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("3D Tech- GLUT Tutorial");

    init();
    glutSpecialFunc(SpecialKeys); //ע�Ṧ�ܼ��ص�����
    glutDisplayFunc(Display);
    glutTimerFunc(2, timefuncupdate, 0);
    //glutReshapeFunc(changeSize);

    //������Ȳ��ԡ�
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
