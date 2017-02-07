#include <math.h>
#include <glut.h>

#define PI acos(-1.0)

// ͨ�����̸ı�Ƕ�
float theta, phi;

void prepare_lighting() {
    theta = fmodf(theta, 2 * PI);
    phi = fmodf(phi, 2 * PI);

    // ��Դ����λ��
    GLfloat light_position_x[] = { -1.0f,-1.0f,0.0f,0.0f };
    GLfloat light_position_y[] = { -1.0f,1.0f,0.0f,0.0f };
    GLfloat light_position_z[] = { 1.0f,1.0f,0.0f,0.0f };
    GLfloat light_diffuse_x[] = { 1.0f, 0.0f,0.0f,1.0f };
    GLfloat light_diffuse_y[] = { 0.0f, 1.0f,0.0f,1.0f };
    GLfloat light_diffuse_z[] = { 0.0f, 0.0f,1.0f,1.0f };


    GLfloat light_position[] = {sinf(theta) * cosf(phi), cosf(theta), -sinf(theta) * sinf(phi), 0.0f};
    // �ù�Դ�������Ĺ⣬�����ǳ���εķ���������������������ջ����е�ǿ��
    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    // ��Դ�������Ĺ⣬���䵽�ֲڱ���ʱ���������䣬���õ��Ĺ��ǿ��
    GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.5f };
    // ��Դ�������Ĺ⣬���䵽�⻬����ʱ�������淴�䣬���õ��Ĺ��ǿ��
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_x);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position_y);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position_z);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse_x);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse_y);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse_z);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // ����ָ��
    GLfloat mat_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    // ���� 0 �Ź�Դ
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
}

GLfloat size = 0.1f;
int move_x = 0;
int move_y = 0;
int click_x, click_y;


GLfloat delta_x = 0.0f;
GLfloat delta_z = 1.0f;

void keyboard_event(unsigned char key, int x, int y) {
    switch (key) {
        case 'W':
        case 'w':
            theta -= .05;
            glutPostRedisplay();
            break;

        case 'S':
        case 's':
            theta += .05;
            glutPostRedisplay();
            break;

        case 'A':
        case 'a':
            phi -= .05;
            glutPostRedisplay();
            break;

        case 'D':
        case 'd':
            phi += .05;
            glutPostRedisplay();
            break;

        case '+':
            size += 0.01f;
            glutPostRedisplay();
            break;
        case '-':
            if (size > 0.01f) {
                size -= 0.01f;
            }
            glutPostRedisplay();
            break;

        case 'i':
            delta_z += 0.1;
            glutPostRedisplay();
            break;

        case 'k':
            if (delta_z > 1.1) {
                delta_z -= 0.1;
            }
            glutPostRedisplay();
            break;

        case 'j':
            delta_x += 0.1;
            glutPostRedisplay();
            break;

        case 'l':
            delta_x -= 0.1;
            glutPostRedisplay();
            break;

        default:
            break;
    }
}

void display_event() {
    // ��Դ�趨
    prepare_lighting();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, 1, 0.1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
    // ���ù���
    glEnable(GL_LIGHTING);
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    // ��������϶���λ����ת
    glRotated(move_x, 0, 1, 0);
    glRotated(move_y, 1, 0, 0);
    // ���Ʋ��
    //glutSolidTeapot(size);
    glutSolidCone(size, size, 100, 100);
    glPopMatrix();
    glutSwapBuffers();
}

void click_event(int button, int state, int x, int y) {
    click_x = x;
    click_y = y;
}

void move_event(int x, int y) {
    int dx = x - click_x;
    int dy = y - click_y;
    move_x += dx;
    move_y += dy;
    glutPostRedisplay();
    click_x = x;
    click_y = y;
}

int main(int argc, char** argv) {
    theta = PI / 2;
    phi = -PI / 2;
    // GLUT ��ʼ��
    glutInit(&argc, argv);
    // ������ʾģʽ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    // ���ô��ڴ�С
    glutInitWindowSize(640, 640);
    // ���ô��ڱ���
    glutCreateWindow("Lighting");
    // ��ʾ�ص�����
    glutDisplayFunc(display_event);
    // ���̻ص�����
    glutKeyboardFunc(keyboard_event);
    // ������ص�����
    glutMouseFunc(click_event);
    // ����ƶ��ص�����
    glutMotionFunc(move_event);
    // ѭ��
    glutMainLoop();
    return 0;
}
