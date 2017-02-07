#include <cmath>
#include <ctime>
#include <glut.h>
#include <iostream>
#include <windows.h>
#include "image_loader.h"

#define PI acos(-1.0)
#define MAX_SIZE 10
#define MIN_SIZE 0.1

#define SCREEN_WIDTH glutGet(GLenum(GLUT_SCREEN_WIDTH))
#define SCREEN_HEIGHT glutGet(GLenum(GLUT_SCREEN_HEIGHT))
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define IS_EQUAL(x, y) fabs(x - y) < 1e-4

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,
                              0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}

// 贴图
GLuint img_brick;
GLuint img_grass;
GLuint img_wall;
GLuint img_gate;
GLuint img_road;
GLuint img_door;
GLuint img_sun;
GLuint img_sun2;
GLuint img_ground2;

GLfloat eye_position_x = 2.0, eye_position_y = 1.5, eye_position_z = 2.0;
GLfloat cur_size = 3.5f;
GLfloat horizontal_angle = 0.716;   // 视角水平夹角
GLfloat vertical_angle = 0.403;     // 视角垂直夹角

enum { CLOCKWISE, ANTI_CLOCKWISE };

enum { HORIZONTAL, VERTICAL };

GLint direction = ANTI_CLOCKWISE;
GLint rotate_direction = HORIZONTAL;

void yard() {
    // 绘制庭院草地
    glPushMatrix(); {
        glRotated(90, 1, 0, 0);
        glTranslatef(0, 0, -0.3);
        glBegin(GL_QUADS); {
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-0.3f, -0.3f, 0.0f);
            glTexCoord2f(0.0f, 5.0f);
            glVertex3f(-0.3f, 0.3f, 0.0f);
            glTexCoord2f(5.0f, 5.0f);
            glVertex3f(0.3f, 0.3f, 0.0f);
            glTexCoord2f(5.0f, 0.0f);
            glVertex3f(0.3f, -0.3f, 0.0f);
        }
        glEnd();
    }
    glPopMatrix();
}

void gate_pillar(GLfloat thick, GLfloat length) {
    // 绘制大门门柱
    glPushMatrix(); {
        glTranslated(0, length / 2, 0);
        glScaled(thick, length, thick);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void building_body() {
    // 房子顶板
    glPushMatrix(); {
        glTranslated(0, 0.3, 0);
        glScaled(0.7, 0.01, 0.7);
        glutSolidCube(1.0);
    }
    glPopMatrix();
    // 房子地板
    glPushMatrix(); {
        glTranslated(0, 0, 0);
        glScaled(0.6, 0.02, 0.6);
        glutSolidCube(1.0);
    }
    glPopMatrix();
    // 房屋主体
    glPushMatrix(); {
        glScaled(1, 0.5, 1);
        glTranslated(0, 0.3, 0);
        glutSolidCube(0.55);
    }
    glPopMatrix();
    // 绘制房子的四个边
    glPushMatrix(); {
        glTranslated(0.27, 0, 0.27);
        gate_pillar(0.02, 0.3);
        glTranslated(0, 0, -0.54);
        gate_pillar(0.02, 0.3);
        glTranslated(-0.54, 0, 0.54);
        gate_pillar(0.02, 0.3);
        glTranslated(0, 0, -0.54);
        gate_pillar(0.02, 0.3);
    }
    glPopMatrix();
}

void building() {
    glPushMatrix(); {
        glTranslated(0.4, 0, 0.4);
        glBindTexture(GL_TEXTURE_2D, img_brick);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        building_body();
        glTranslated(0, 0.65, 0);

        glPushMatrix(); {
            glTranslated(0, 0, 0.35);
            glBegin(GL_TRIANGLES);
            glVertex3f(0.0f, 0.00f, 0.0f); // 上顶点
            glVertex3f(-0.35f, -0.35f, 0.0f); // 左下
            glVertex3f(0.35f, -0.35f, 0.0f); // 右下
            glEnd();
        }
        glPopMatrix();

        glPushMatrix(); {
            glTranslated(0, 0, -0.35);
            glBegin(GL_TRIANGLES);
            glVertex3f(0.0f, 0.00f, 0.0f); // 上顶点
            glVertex3f(-0.35f, -0.35f, 0.0f); // 左下
            glVertex3f(0.35f, -0.35f, 0.0f); // 右下
            glEnd();
        }
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);

        glPushMatrix(); {
            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT, GL_AMBIENT);
            glColor4f(0, 0.5, 1, 0.0);
            glRotated(45, 0, 0, 1);
            glTranslated(0, -0.25, 0);
            glScaled(0.01, 0.5, 0.7);
            glutSolidCube(1.0);
            glDisable(GL_COLOR_MATERIAL);
        }
        glPopMatrix();
        glPushMatrix(); {
            glRotated(-45, 0, 0, 1);
            glTranslated(0, -0.25, 0);
            glScaled(0.01, 0.5, 0.7);
            glutSolidCube(1.0);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_gate_roof() {
    // 绘制屋檐
    glPushMatrix(); {
        glRotated(90, 1, 0, 0);
        glScaled(0.6, 0.5, 0.02);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void gate_roof_left() {
    // 绘制左侧屋檐
    glPushMatrix();
    glRotated(60, 0, 0, 1);
    draw_gate_roof();
    glPopMatrix();
}

void gate_roof_right() {
    // 绘制右侧屋檐
    glPushMatrix();
    glRotated(60, 0, 0, 1);
    gate_roof_left();
    glPopMatrix();
}

void draw_gate() {
    // 左侧屋檐
    glPushMatrix(); {
        glScaled(1, 0.8, 0.5);
        glTranslated(-0.15, 0.6, 0.55);
        gate_roof_left();
    }
    glPopMatrix();
    // 右侧屋檐
    glPushMatrix(); {
        glScalef(1, 0.8, 0.5);
        glTranslated(0.15, 0.6, 0.55);
        gate_roof_right();
    }
    glPopMatrix();
    // 两边的柱子
    glPushMatrix(); {
        glTranslated(0.27, 0, 0.27);
        gate_pillar(0.17, 0.3);
        glTranslated(0, 0, -0.54);
        glTranslated(-0.54, 0, 0.54);
        gate_pillar(0.17, 0.3);
    }
    glPopMatrix();
}

void gate() {
    glPushMatrix(); {
        glTranslated(-0.225, 0.02, 1.0);
        glScalef(0.7, 0.7, 0.7);
        glBindTexture(GL_TEXTURE_2D, img_gate);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        draw_gate();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
    glPopMatrix();
}

void draw_solid_cylinder() {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluCylinder(quadric, 0.17f, 0.17f, 0.15f, 20, 20);
    glRotatef(180, 1, 0, 0);
    gluDisk(quadric, 0.0f, 0.17f, 20, 1);
    glRotatef(180, 1, 0, 0);
    glTranslatef(0.0f, 0.0f, 0.15f);
    gluDisk(quadric, 0.0f, 0.17f, 20, 1);
    glTranslatef(0.0f, 0.0f, -0.15f);
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quadric);
}

void solid_cylinder() {
    glPushMatrix(); {
        glScalef(0.5, 1.3, 0.5);
        glTranslated(0.35, 0.151, 1.35);
        glPushMatrix(); {
            glRotated(90, 1, 0, 0);
            glScaled(1, 1, 1.2);
            glTranslated(2.0, 1.0, 0);
            draw_solid_cylinder();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void pillar() {
    glPushMatrix();
    solid_cylinder();
    glTranslated(-2.35, 0, 0);
    solid_cylinder();
    glPopMatrix();
}

void draw_road() {
    // 绘制路面
    glPushMatrix(); {
        glTranslated(0, 0.3, -0.1);
        glRotated(90, 1, 0, 0);
        glScaled(0.5, 0.7, 0.01);
        glutSolidCube(1);
    }
    glPopMatrix();
}

void road() {
    // 绘制路面
    glPushMatrix(); {
        glBindTexture(GL_TEXTURE_2D, img_road);
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        draw_road();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
    glPopMatrix();
}

void draw_door() {
    // 绘制楼门
    glPushMatrix(); {
        glBindTexture(GL_TEXTURE_2D, img_door);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        draw_road();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
    glPopMatrix();
}

void door() {
    // 绘制楼门
    glPushMatrix(); {
        glTranslated(-0.22, -0.18, -0.9);
        glScalef(1, 0.8, 1);
        glPushMatrix(); {
            glRotated(90, 1, 0, 0);
            glScalef(0.5, 1, 1);
            glTranslated(0, 0.5, -0.5);
            draw_door();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_window() {
    // 绘制窗户
    glPushMatrix(); {
        glScalef(0.5, 0.5, 0.5);
        glTranslated(1, 0.2, 0);
        door();
        glPushMatrix(); {
            glRotated(-90, 1, 0, 0);
            glScalef(1, 1, 0.8);
            glTranslated(0, 0, 0.65);
            door();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void window() {
    // 绘制窗户
    glPushMatrix(); {
        glTranslated(-0.28, 0.1, -0.07);
        draw_window();
    }
    glPopMatrix();

    glPushMatrix(); {
        glTranslated(-0.95, 0.1, -0.07);
        draw_window();
    }
    glPopMatrix();
}

#define ROBOT_BODY_HEIGHT 0.3
#define ROBOT_BODY_RADIUS 0.1
#define ROBOT_HEAD_HEIGHT 0.11
#define ROBOT_HEAD_RADIUS 0.07
#define ROBOT_ARM_HEIGHT 0.15
#define ROBOT_ARM_WIDTH 0.04
#define ROBOT_HAND_HEIGHT 0.1
#define ROBOT_HAND_WIDTH 0.04
#define ARM_TRANSLATION 0.22/12

GLUquadricObj* obj;

void draw_robot_body() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {0.5f, 0.5f,0.5f,0.0f};
        GLfloat mat_diffuse[] = {0.5f, 0.5f,0.5f,0.0f};
        GLfloat mat_specular[] = {0.5f, 0.5f,0.5f,0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glRotatef(-90.0, 1.0, 0.0, 0.0);
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluCylinder(obj, ROBOT_BODY_RADIUS, ROBOT_BODY_RADIUS, ROBOT_BODY_HEIGHT, 20, 20);
    }
    glPopMatrix();

    glPushMatrix(); {
        gluQuadricDrawStyle(obj, GLU_FILL);
        glTranslatef(0.0, ROBOT_BODY_HEIGHT, 0.0);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        gluDisk(obj, 0.0, ROBOT_BODY_RADIUS, 20, 20);
        glTranslatef(0.0, 0.0, -ROBOT_BODY_HEIGHT);
        gluDisk(obj, 0.0, ROBOT_BODY_RADIUS, 20, 20);
    }
    glPopMatrix();
}

void draw_robot_neck() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {0.8f, 0.8f, 0.8f,0.0f};
        GLfloat mat_diffuse[] = {0.8f, 0.8f,0.8f,0.0f};
        GLfloat mat_specular[] = {0.8f, 0.8f,0.8f,0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glTranslatef(0.0, ROBOT_BODY_HEIGHT, 0.0);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluCylinder(obj, ROBOT_HEAD_RADIUS / 2, ROBOT_HEAD_RADIUS / 2, ROBOT_HEAD_HEIGHT, 8, 6);
    }
    glPopMatrix();
}

// Bezier 曲线三个控制点
GLfloat ctrl_points[3][3] = {
    {-0.02f, 0.0f, 0.0f},
    {0.0f, -0.04f, 0.0f},
    {0.02f, 0.0f, 0.0f}
};

void draw_robot_head() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {0.5f, 0.0f,0.0f,0.0f};
        GLfloat mat_diffuse[] = {0.0f, 1.0f,0.0f,0.0f};
        GLfloat mat_specular[] = {0.0f, 0.0f,1.0f,0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glRotatef(-90.0, 1.0, 0.0, 0.0);
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluCylinder(obj, ROBOT_HEAD_RADIUS, ROBOT_HEAD_RADIUS, ROBOT_HEAD_HEIGHT, 20, 20);

        glPushMatrix(); {
            gluDisk(obj, 0.0, ROBOT_HEAD_RADIUS, 20, 20);
            glTranslatef(0.0, 0.0, ROBOT_HEAD_HEIGHT);
            gluDisk(obj, 0.0, ROBOT_HEAD_RADIUS, 20, 20);
        }
        glPopMatrix();

        // 绘制两只眼睛
        glPushMatrix(); {
            glTranslatef(0.035, -0.75 / 13, 0.075);
            mat_ambient[0] = 1.0f;
            mat_ambient[1] = 1.0f;
            mat_ambient[2] = 1.0f;
            mat_diffuse[0] = 1.0f;
            mat_diffuse[1] = 1.0f;
            mat_diffuse[2] = 1.0f;
            mat_specular[0] = 1.0f;
            mat_specular[1] = 1.0f;
            mat_specular[2] = 1.0f;
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

            gluQuadricDrawStyle(obj, GLU_FILL);
            gluSphere(obj, 0.015, 6, 6);
        }
        glPopMatrix();

        glPushMatrix(); {
            glTranslatef(-0.035, -0.75 / 13, 0.075);
            // 此处的Material和上方保持一致
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

            gluQuadricDrawStyle(obj, GLU_FILL);
            gluSphere(obj, 0.015, 6, 6);
        }
        glPopMatrix();

        glPushMatrix(); {
            glTranslatef(0.0, -ROBOT_HEAD_RADIUS, 0.04);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glPointSize(1.5f); 
            glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrl_points[0][0]);
            glEnable(GL_MAP1_VERTEX_3);
            glBegin(GL_POINTS);
            for (int i = 0; i <= 300; i++) {
                glEvalCoord1f(i / 300.0f);
            }
            glEnd();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_robot_left_arm() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {0.5f, 0.4f, 0.3f, 0.0f};
        GLfloat mat_diffuse[] = {0.5f, 0.4f, 0.3f, 0.0f};
        GLfloat mat_specular[] = {0.5f, 0.4f, 0.3f, 0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glTranslatef(0.0, 0.5 * ROBOT_ARM_HEIGHT, ARM_TRANSLATION);
        glScalef(ROBOT_ARM_WIDTH, ROBOT_ARM_HEIGHT, ROBOT_ARM_WIDTH);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void draw_robot_left_hand() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {1.0f, 1.0f, 1.0f, 0.0f};
        GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
        GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glTranslatef(0.0, 0.5 * ROBOT_HAND_HEIGHT, ARM_TRANSLATION);
        glScalef(ROBOT_HAND_WIDTH, ROBOT_HAND_HEIGHT, ROBOT_HAND_WIDTH);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void draw_robot_right_arm() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {0.5f, 0.4f, 0.3f, 0.0f};
        GLfloat mat_diffuse[] = {0.5f, 0.4f, 0.3f, 0.0f};
        GLfloat mat_specular[] = {0.5f, 0.4f, 0.3f, 0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glTranslatef(0.0, 0.5 * ROBOT_ARM_HEIGHT, -ARM_TRANSLATION);
        glScalef(ROBOT_ARM_WIDTH, ROBOT_ARM_HEIGHT, ROBOT_ARM_WIDTH);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void draw_robot_right_hand() {
    glPushMatrix(); {
        GLfloat mat_ambient[] = {1.0f, 1.0f, 1.0f, 0.0f};
        GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
        GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 0.0f};
        GLfloat mat_shiness[] = {50.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

        glTranslatef(0.0, 0.5 * ROBOT_HAND_HEIGHT, -ARM_TRANSLATION);
        glScalef(ROBOT_HAND_WIDTH, ROBOT_HAND_HEIGHT, ROBOT_HAND_WIDTH);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void robot() {
    // 绘制机器人
    glPushMatrix(); {
        glTranslatef(0, 0, 0);
        glRotatef(0, 0.0, 1.0, 0.0);
        // 绘制机器人身体
        draw_robot_body();
        // 绘制机器人脖子
        draw_robot_neck();
    }
    glPopMatrix();

    // 绘制机器人头部
    glPushMatrix(); {
        glTranslatef(0.0, ROBOT_BODY_HEIGHT + ROBOT_HEAD_HEIGHT / 2, 0.0);
        draw_robot_head();
    }
    glPopMatrix();

    glPushMatrix(); {
        glTranslatef(ROBOT_BODY_RADIUS, ROBOT_BODY_HEIGHT - ROBOT_BODY_RADIUS / 2, 0.0);
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        draw_robot_left_arm();
        glTranslatef(0.0, ROBOT_ARM_HEIGHT, 0.0);
        draw_robot_left_hand();
    }
    glPopMatrix();

    glPushMatrix(); {
        glTranslatef(-ROBOT_BODY_RADIUS, ROBOT_BODY_HEIGHT - ROBOT_BODY_RADIUS / 2, 0.0);
        glRotatef(180.0f, 0.0, 0.0, 1.0);
        glRotatef(270.0f, 0.0, 1.0, 0.0);
        draw_robot_right_arm();
        glTranslatef(0.0, ROBOT_ARM_HEIGHT, 0.0);
        draw_robot_right_hand();
    }
    glPopMatrix();
}

void draw_clock() {
    // 获取系统时间
    auto t = time(nullptr);
    auto* now = localtime(&t);
    GLfloat radius = 0.8f;

    // 钟盘
    glBegin(GL_POLYGON);
    int n = 12;
    for (int i = 0; i < n; i++) {
        glVertex3f(radius * cos(2 * PI / n * i), radius * sin(2 * PI / n * i), 5);
    }
    glEnd();

    // 刻度
    GLfloat mat_ambient[] = {0, 0, 0, 1.0f};
    GLfloat mat_defused[] = {0, 0, 0, 1.0f};
    GLfloat mat_specular[] = {0, 0, 0, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 12; i++) {
        glVertex3f(0.75 * cos(2 * PI / 12 * i), 0.75 * sin(2 * PI / 12 * i), 0);
    }
    glEnd();

    int h = now->tm_hour;
    int m = now->tm_min;
    int s = now->tm_sec;

    // 时针
    glPushMatrix(); {
        glRotatef(-(h - 12 + m / 60.0 + s / 3600.0) * 30.0, 0.0, 0.0, 1.0);
        glLineWidth(5.0f);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, radius * 0.55, 0);
        glEnd();
    }
    glPopMatrix();

    // 分针
    glPushMatrix(); {
        glLineWidth(4.0f);
        glRotatef(-(m + s / 60.0f) * 6.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, radius * 0.65f);
        glEnd();
    }
    glPopMatrix();

    // 秒针
    glPushMatrix(); {
        glLineWidth(3.0f);
        glRotatef(-s * 6.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, radius * 0.85f);
        glEnd();
    }
    glPopMatrix();
}

// 机器人的位置
GLfloat robot_move_x = 0.4f;
GLfloat robot_move_y = 0.4f;
GLfloat grass_points[3][3] = {
    { -0.2f, 0.0f, 0.0f },
    { 0.0f, -0.4f, 0.0f },
    { 0.2f, 0.0f, 0.0f }
};

void display_function() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat light_diffuse0[] = {0.8f, 0.5f, 0.0f, 1.0f};
    GLfloat light_ambient0[] = {0.8f, 0.5f, 0.0f, 0.0f};
    GLfloat light_specular0[] = {0.8f, 0.5f, 0.0f, 0.0f};
    GLfloat light_position0[] = {1.8f, 0.8f, -2.8f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
    glEnable(GL_LIGHT0);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspectR = 1.0f * WINDOW_WIDTH / WINDOW_HEIGHT;
    gluPerspective(50.0f, aspectR, 0.05f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_position_x, eye_position_y, eye_position_z, 0, 0, 0, 0, 1, 0);

    // 绘制楼
    glPushMatrix(); {
        glTranslated(-1, 0, -0.8);
        glScalef(2, 2, 1);
        building();
    }
    glPopMatrix();

    obj = gluNewQuadric();
    
    GLfloat mat_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_defused[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix(); {
        glBindTexture(GL_TEXTURE_2D, img_sun);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-5.0f, -0.0f, -3.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-5.0f, 5.0f, -3.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(5.0f, 5.0f, -3.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(5.0f, -0.0f, -3.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glTranslatef(-2, 0, 2);
        glRotated(90, 0, 1, 0);
        glBindTexture(GL_TEXTURE_2D, img_sun2);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-5.0f, -0.0f, -3.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-5.0f, 5.0f, -3.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(5.0f, 5.0f, -3.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(5.0f, -0.0f, -3.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glTranslatef(2, 0, 2);
        glRotated(-90, 0, 1, 0);
        glBindTexture(GL_TEXTURE_2D, img_sun2);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-5.0f, -0.0f, -3.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-5.0f, 5.0f, -3.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(5.0f, 5.0f, -3.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(5.0f, -0.0f, -3.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glBindTexture(GL_TEXTURE_2D, img_ground2);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-5.0f, -0.04f, -3.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(5.0f, -0.04f, -3.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(5.0f, -0.04f, 7.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-5.0f, -0.04f, 7.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
  
    // 绘制烟囱
    glPushMatrix(); {
        glScalef(0.06, 0.5, 0.06);
        glTranslatef(1, 2, -7.5);
        glutSolidCube(1);
    }
    glPopMatrix();

    // 绘制草

        glPushMatrix(); {
            glTranslatef(-1, 0, 0.5);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glPointSize(1.5f);
            glEnable(GL_COLOR_MATERIAL);

            glColorMaterial(GL_FRONT, GL_AMBIENT);
            glColor3f(0, 1, 0);
            glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &grass_points[0][0]);
            glEnable(GL_MAP1_VERTEX_3);
            glBegin(GL_POINTS);
            for (int i = 0; i <= 300; i++) {
                glEvalCoord1f(i / 300.0f);
            }
            glEnd();
            glDisable(GL_COLOR_MATERIAL);
        }
        glPopMatrix();

    // 绘制树
    glPushMatrix(); {
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT);

        glColor3ub(0x27, 0x4e, 0x13);
        glTranslatef(0.75f, 0.2f, -0.3f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.2, 0.8, 20, 4);

        glTranslatef(0.0f, 0.0f, -0.23f);
        glColor3ub(0x78, 0x3f, 0x04);
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluCylinder(obj, ROBOT_HEAD_RADIUS / 2, ROBOT_HEAD_RADIUS / 2, 0.3, 8, 6);

        glDisable(GL_COLOR_MATERIAL);
    }
    glPopMatrix();

    // 绘制机器人
    glPushMatrix(); {
        glTranslated(robot_move_x, 0, robot_move_y);
        robot();
    }
    glPopMatrix();

    glPushMatrix(); {
        glScaled(0.3, 0.3, 0);
        glTranslated(-0.65, 3, -1.0);
        draw_clock();
    }
    glPopMatrix();

    mat_ambient[0] = {0.7f};
    mat_ambient[1] = {0.7f};
    mat_ambient[2] = {0.7f};
    mat_defused[0] = {0.6f};
    mat_defused[1] = {0.6f};
    mat_defused[2] = {0.6f};
    mat_specular[0] = {1.0f};
    mat_specular[1] = {1.0f};
    mat_specular[2] = {1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    // 地面
    glPushMatrix(); {
        glScalef(4, 1, 4);
        glTranslated(0, -0.33, 0);
        glBindTexture(GL_TEXTURE_2D, img_grass);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        yard();
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // 四面的围栏
    glPushMatrix(); {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, img_wall);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, -0.03f, -1.15f);
        glTexCoord2f(2.4, 0.0f);
        glVertex3f(1.2f, -0.03f, -1.15f);
        glTexCoord2f(2.4, 0.18);
        glVertex3f(1.2f, 0.15f, -1.15f);
        glTexCoord2f(0.0f, 0.18);
        glVertex3f(-1.2f, 0.15f, -1.15f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(2.4f, 0.0f);
        glVertex3f(-1.2f, -0.03f, -1.21f);
        glTexCoord2f(2.4f, 0.18f);
        glVertex3f(-1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.18f);
        glVertex3f(1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.2f, -0.03f, -1.21f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.06f);
        glVertex3f(-1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, 0.15f, -1.15f);
        glTexCoord2f(2.4f, 0.0f);
        glVertex3f(1.2f, 0.15f, -1.15f);
        glTexCoord2f(2.4f, 0.06f);
        glVertex3f(1.2f, 0.15f, -1.21f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glRotated(90, 0, 1, 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, img_wall);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, -0.03f, -1.15f);
        glTexCoord2f(2.4, 0.0f);
        glVertex3f(1.2f, -0.03f, -1.15f);
        glTexCoord2f(2.4, 0.18);
        glVertex3f(1.2f, 0.15f, -1.15f);
        glTexCoord2f(0.0f, 0.18);
        glVertex3f(-1.2f, 0.15f, -1.15f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(2.4f, 0.0f);
        glVertex3f(-1.2f, -0.03f, -1.21f);
        glTexCoord2f(2.4f, 0.18f);
        glVertex3f(-1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.18f);
        glVertex3f(1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.2f, -0.03f, -1.21f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.06f);
        glVertex3f(-1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, 0.15f, -1.15f);
        glTexCoord2f(2.4f, 0.0f);
        glVertex3f(1.2f, 0.15f, -1.15f);
        glTexCoord2f(2.4f, 0.06f);
        glVertex3f(1.2f, 0.15f, -1.21f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glRotated(-90, 0, 1, 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, img_wall);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, -0.03f, -1.15f);
        glTexCoord2f(2.4, 0.0f);
        glVertex3f(1.2f, -0.03f, -1.15f);
        glTexCoord2f(2.4, 0.18);
        glVertex3f(1.2f, 0.15f, -1.15f);
        glTexCoord2f(0.0f, 0.18);
        glVertex3f(-1.2f, 0.15f, -1.15f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(2.4f, 0.0f);
        glVertex3f(-1.2f, -0.03f, -1.21f);
        glTexCoord2f(2.4f, 0.18f);
        glVertex3f(-1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.18f);
        glVertex3f(1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.2f, -0.03f, -1.21f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.06f);
        glVertex3f(-1.2f, 0.15f, -1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, 0.15f, -1.15f);
        glTexCoord2f(2.4f, 0.0f);
        glVertex3f(1.2f, 0.15f, -1.15f);
        glTexCoord2f(2.4f, 0.06f);
        glVertex3f(1.2f, 0.15f, -1.21f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, img_wall);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, -0.03f, 1.15f);
        glTexCoord2f(0.8, 0.0f);
        glVertex3f(-0.4f, -0.03f, 1.15f);
        glTexCoord2f(0.8, 0.18);
        glVertex3f(-0.4f, 0.15f, 1.15f);
        glTexCoord2f(0.0f, 0.18);
        glVertex3f(-1.2f, 0.15f, 1.15f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(0.8f, 0.0f);
        glVertex3f(-1.2f, -0.03f, 1.21f);
        glTexCoord2f(0.8f, 0.18f);
        glVertex3f(-1.2f, 0.15f, 1.21f);
        glTexCoord2f(0.0f, 0.18f);
        glVertex3f(-0.4f, 0.15f, 1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.4f, -0.03f, 1.21f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.06f);
        glVertex3f(-1.2f, 0.15f, 1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.2f, 0.15f, 1.15f);
        glTexCoord2f(0.8f, 0.0f);
        glVertex3f(-0.4f, 0.15f, 1.15f);
        glTexCoord2f(0.8f, 0.06f);
        glVertex3f(-0.4f, 0.15f, 1.21f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glPushMatrix(); {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, img_wall);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, -0.03f, 1.15f);
        glTexCoord2f(1.2f, 0.0f);
        glVertex3f(1.2f, -0.03f, 1.15f);
        glTexCoord2f(1.2f, 0.18);
        glVertex3f(1.2f, 0.15f, 1.15f);
        glTexCoord2f(0.0f, 0.18);
        glVertex3f(0.0f, 0.15f, 1.15f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(1.2f, 0.0f);
        glVertex3f(0.0f, -0.03f, 1.21f);
        glTexCoord2f(1.2f, 0.18f);
        glVertex3f(0.0f, 0.15f, 1.21f);
        glTexCoord2f(0.0f, 0.18f);
        glVertex3f(1.2f, 0.15f, 1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.2f, -0.03f, 1.21f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.06f);
        glVertex3f(0.0f, 0.15f, 1.21f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.15f, 1.15f);
        glTexCoord2f(1.2f, 0.0f);
        glVertex3f(1.2f, 0.15f, 1.15f);
        glTexCoord2f(1.2f, 0.06f);
        glVertex3f(1.2f, 0.15f, 1.21f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // 庭院大门
    glPushMatrix(); {
        glTranslated(0, -0.04, 0);
        gate();
    }
    glPopMatrix();

    // 庭院柱子
    glPushMatrix(); {
        pillar();
        glTranslated(0, 0, -2.36);
        pillar();
    }
    glPopMatrix();

    // 绘制路面
    glPushMatrix(); {
        glScalef(1, 1, 2.75);
        glTranslated(-0.23, -0.32, 0.25);
        road();
    }
    glPopMatrix();

    // 绘制房屋门
    glPushMatrix(); {
        door();
    }
    glPopMatrix();

    // 绘制窗户
    glPushMatrix(); {
        window();
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void calc_eye_position() {
    GLfloat temp = cur_size * cos(vertical_angle);
    eye_position_x = temp * cos(horizontal_angle);
    eye_position_y = cur_size * sin(vertical_angle);
    eye_position_z = temp * sin(horizontal_angle);
}

void horizontal_rotate() {
    if (horizontal_angle > PI * 2) {
        horizontal_angle = 0.0;
    }
    if (direction == ANTI_CLOCKWISE) {
        horizontal_angle += 0.05;
    } else {
        horizontal_angle -= 0.05;
    }
}

void vertical_rotate() {
    if (vertical_angle > PI * 2) {
        vertical_angle = 0.0;
    }
    if (direction == ANTI_CLOCKWISE) {
        vertical_angle += 0.05;
    } else {
        vertical_angle -= 0.05;
    }
}

void keyboard_move() {
    calc_eye_position();
    if (rotate_direction == HORIZONTAL) {
        horizontal_rotate();
    } else {
        vertical_rotate();
    }
}

void zoom_func(unsigned char key) {
    // 放大和缩小函数
    if (key == '-') {
        if (cur_size > MAX_SIZE) {
            cur_size = MAX_SIZE;
        } else {
            cur_size += 0.2;
        }
    } else if (key == '+') {
        if (cur_size <= MIN_SIZE) {
            cur_size = MIN_SIZE;
        } else {
            cur_size -= 0.2;
        }
    }
    calc_eye_position();
}

GLint click_position_x;    // 鼠标点击的位置
GLint click_position_y;

void click_function(GLint button, GLint state, GLint x, GLint y) {
    // 鼠标点击事件
    if ((button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN) {
        click_position_x = x;
        click_position_y = y;
    }
}

void move_function(GLint x, GLint y) {
    // 鼠标移动事件
    int dx = x - click_position_x;
    int dy = y - click_position_y;
    if (!(x - click_position_x)) {
        horizontal_angle -= dx / 500.0;
    } else {
        horizontal_angle += dx / 500.0;
    }
    if (!(y - click_position_y)) {
        vertical_angle -= dy / 500.0;
    } else {
        vertical_angle += dy / 500.0;
    }
    // 改变视角坐标
    calc_eye_position();
    // 进行重绘
    glutPostRedisplay();
    click_position_x = x;
    click_position_y = y;
}

void keybord_function(unsigned char key, int x, int y) {
    if (key == '-' || key == '+') {
        zoom_func(key);
    } else if (key == 'd' || key == 'D') {
        direction = CLOCKWISE;
        rotate_direction = HORIZONTAL;
        keyboard_move();
    } else if (key == 'a' || key == 'A') {
        direction = ANTI_CLOCKWISE;
        rotate_direction = HORIZONTAL;
        keyboard_move();
    } else if (key == 's' || key == 'S') {
        direction = CLOCKWISE;
        rotate_direction = VERTICAL;
        keyboard_move();
    } else if (key == 'w' || key == 'W') {
        direction = ANTI_CLOCKWISE;
        rotate_direction = VERTICAL;
        keyboard_move();
    } else if (key == 'i' || key == 'I') {
        robot_move_y -= 0.1;
    } else if (key == 'k' || key == 'K') {
        robot_move_y += 0.1;
    } else if (key == 'j' || key == 'J') {
        robot_move_x -= 0.1;
    } else if (key == 'l' || key == 'L') {
        robot_move_x += 0.1;
    } else if (key == 27) {
        // ESC
        exit(0);
    }
    glutPostRedisplay();
}

void timer_function(int value) {
    if (IS_EQUAL(ctrl_points[1][1], -0.04f)) {
        ctrl_points[1][1] = -0.02f;
    } else {
        ctrl_points[1][1] = -0.04f;
    }

    if (IS_EQUAL(grass_points[2][0], 0.2f)) {
        grass_points[2][0] = 0.1f;
    } else {
        grass_points[2][0] = 0.2f;
    }
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrl_points[0][0]);
    glutPostRedisplay();
    glutTimerFunc(500, timer_function, 1);
}

void initialize() {
    // 加载贴图素材
    Image* image = loadBMP("brick.bmp");
    img_brick = loadTexture(image);
    image = loadBMP("ground.bmp");
    img_grass = loadTexture(image);
    image = loadBMP("brick.bmp");
    img_wall = loadTexture(image);
    image = loadBMP("gate.bmp");
    img_gate = loadTexture(image);
    image = loadBMP("road.bmp");
    img_road = loadTexture(image);
    image = loadBMP("door.bmp");
    img_door = loadTexture(image);
    image = loadBMP("sun.bmp");
    img_sun = loadTexture(image);
    image = loadBMP("sun2.bmp");
    img_sun2 = loadTexture(image);
    image = loadBMP("ground2.bmp");
    img_ground2 = loadTexture(image);
    delete image;
    // 开启光照
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    // 设置初始视角
    calc_eye_position();

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrl_points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // 定义窗口大小, 并让窗口居中显示
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition((SCREEN_WIDTH - WINDOW_WIDTH) / 2, (SCREEN_HEIGHT - WINDOW_HEIGHT) / 2);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Welcome to my department!");

    std::cout << "********************* 操作说明 *********************" << std::endl;
    std::cout << "1. 使用 W、A、S、D 四个键可以转换视角" << std::endl;
    std::cout << "2. 使用 I、J、K、L 四个键可以控制机器人移动" << std::endl;
    std::cout << "3. 使用 +、- 可以放大缩小视角" << std::endl;
    std::cout << "4. 使用鼠标拖动可以转换视角" << std::endl;
    std::cout << "5. 按下 ESC 键退出程序" << std::endl;
    std::cout << "                                BY 1004141110 高翔" << std::endl;
    std::cout << "****************************************************" << std::endl;

    // 初始化
    initialize();
    // 键盘事件
    glutKeyboardFunc(keybord_function);
    // 显示事件
    glutDisplayFunc(display_function);
    // 鼠标点击回调函数
    glutMouseFunc(click_function);
    // 鼠标移动回调函数
    glutMotionFunc(move_function);
    // 动画时间
    glutTimerFunc(500, timer_function, 1);
    glutMainLoop();
    return 0;
}
