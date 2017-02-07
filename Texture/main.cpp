#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_loader.h"

#define WindowWidth   400
#define WindowHeight 400
#define BMP_Header_Length 54

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                              image->pixels);
    return textureId;
}

GLuint texGround;
GLuint texWall;
void display() {
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置视角
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 1, 21);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(8, 8, 8, 0, 0, 0, 0, 0, 1);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texGround);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-8.0f, -8.0f, 0.0f);
    glTexCoord2f(0.0f, 5.0f);
    glVertex3f(-8.0f, 8.0f, 0.0f);
    glTexCoord2f(5.0f, 5.0f);
    glVertex3f(8.0f, 8.0f, 0.0f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(8.0f, -8.0f, 0.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texWall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -3.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 1.0f);
    glVertex3f(6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, 0.0f);
    glEnd();

    glRotatef(-90, 0, 0, 1);
    glTranslated(-3, -3, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -3.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 1.0f);
    glVertex3f(6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, 0.0f);
    glEnd();

    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    // GLUT初始化
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutCreateWindow("Texture");
    glutDisplayFunc(&display);

    // 在这里做一些初始化
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    Image* image = loadBMP("ground.bmp");
    texGround = loadTexture(image);
    image = loadBMP("dummy.bmp");
    texWall = loadTexture(image);
    delete image;

    // 开始显示
    glutMainLoop();
    return 0;
}
