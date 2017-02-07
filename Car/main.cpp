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
    // 防止除数即高度为0
    // (你可以设置窗口宽度为0).
    if (h == 0) {
        h = 1;
    }

    float ratio = 1.0 * w / h;

    // 单位化投影矩阵。
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 设置视口大小为增个窗口大小
    glViewport(0, 0, w, h);

    // 设置正确的投影矩阵
    gluPerspective(45, ratio, 1, 1000);

    //下面是设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
}

float angle = 0.0;
float trans = 0.0;

void renderScene(float x, float y, float z) {
    //注意我们这里清除了深度缓冲区。
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //保存当前模型视图矩阵。
    glPushMatrix();
    //4X4旋转矩阵生成函数
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1, 0, 0);
    glTranslated(x, y, z);
    glutSolidTorus(2, 5, 5, 100);

    // 弹出堆栈
    glPopMatrix();

    // 交换缓冲区
    glutSwapBuffers();

    // 让angle自动增加。
    //angle++;
}

//定义键盘控制函数
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
    glutPostRedisplay(); // 刷新窗口
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
    //glRotatef(xRot, 1.0f, 0.0f, 0.0f);  //绕x轴旋转
    //glRotatef(yRot, 0.0f, 1.0f, 0.0f);  //绕y轴旋转
    //车下半部
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
    //车上半部
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
    //车轮
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
    glutSwapBuffers(); //交换缓冲
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);

    // 在这里设置双缓冲区。
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("3D Tech- GLUT Tutorial");

    init();
    glutSpecialFunc(SpecialKeys); //注册功能键回调函数
    glutDisplayFunc(Display);
    glutTimerFunc(2, timefuncupdate, 0);
    //glutReshapeFunc(changeSize);

    //开启深度测试。
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
