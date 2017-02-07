#include <glut.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;

#define POINT_NUM 6

struct Edge {
    float x;
    float deltaX, yMax;
    Edge* next;

    Edge(float x = 0.0f, float deltaX = 0.0f, float yMax = 0.0f, Edge* next = nullptr) {
        this->x = x;
        this->deltaX = deltaX;
        this->yMax = yMax;
        this->next = next;
    }
};

typedef Edge AET;
typedef Edge NET;

struct Point {
    float x;
    float y;
};

Point polygon[POINT_NUM];
void initPolygon() {
    polygon[0] = { 20,20 };
    polygon[1] = { 20,70 };
    polygon[2] = { 50,50 };
    polygon[3] = { 110,80 };
    polygon[4] = { 110,30 };
    polygon[5] = { 50,10 };
    for (int i = 0; i < POINT_NUM; i++) {
        polygon[i].x *= 5;
        polygon[i].y *= 5;
        cout << "x = " << polygon[i].x << ", y = " << polygon[i].y << endl;
    }

/*
    polygon[0] = { 250,50 };
    polygon[1] = { 550,150 };
    polygon[2] = { 550,400 };
    polygon[3] = { 250,250 };
    polygon[4] = { 100,350 };
    polygon[5] = { 100,100 };
    polygon[6] = { 120,30 };
*/
}

int getMaxY() {
    int yMax = 0;
    for (int i = 0; i < POINT_NUM; i++) {
        yMax = max(yMax, static_cast<int>(polygon[i].y));
    }
    return yMax;
}

void buildNET(NET** ptrNET, int maxY) {
    for (int i = 0; i <= maxY; i++) {
        for (int j = 0; j < POINT_NUM; j++) {
            // 将 ymin = i 的边放入 NET 表
            if (polygon[j].y == i) {
                if (polygon[(j - 1 + POINT_NUM) % POINT_NUM].y > polygon[j].y) {
                    float x = polygon[j].x;
                    float yMax = polygon[(j - 1 + POINT_NUM) % POINT_NUM].y;
                    float deltaX = (polygon[(j - 1 + POINT_NUM) % POINT_NUM].x - polygon[j].x) / (polygon[(j - 1 + POINT_NUM) % POINT_NUM].y - polygon[j].y);
                    Edge* next = ptrNET[i]->next;
                    ptrNET[i]->next = new NET(x, deltaX, yMax, next);

                }
                if (polygon[(j + 1) % POINT_NUM].y > polygon[j].y) {
                    float x = polygon[j].x;
                    float yMax = polygon[(j + 1) % POINT_NUM].y;
                    float deltaX = (polygon[(j + 1) % POINT_NUM].x - polygon[j].x) / (polygon[(j + 1) % POINT_NUM].y - polygon[j].y);
                    Edge* next = ptrNET[i]->next;
                    ptrNET[i]->next = new NET(x, deltaX, yMax, next);
                }
            }
        }
    }
}

void removeEdge(AET *ptrAET, int y) {
    AET* q = ptrAET;
    AET* p = q->next;
    while (p != nullptr) {
        if (p->yMax == y) {
            q->next = p->next;
            delete p;
            p = q->next;
        } else {
            q = q->next;
            p = q->next;
        }
    }
}

void scanLineFill() {
    initPolygon();
    int maxY = getMaxY(); // 计算最高点的 y 坐标
    AET* ptrAET = new AET(); // 初始化 AET 表

    // 初始化 NET 表
    NET* ptrNET[1024];
    for (int i = 0; i <= maxY; i++) {
        ptrNET[i] = new NET();
    }
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);

    buildNET(ptrNET, maxY); // 建立 NET 表
    for (int i = 0; i <= maxY; i++) {
        // 计算新的交点 x, 更新AET
        NET* p = ptrAET->next;
        while (p != nullptr) {
            p->x = p->x + p->deltaX;
            p = p->next;
        }
        // 使用插入排序对 AET 进行排序
        AET* tq = ptrAET;
        p = ptrAET->next;
        tq->next = nullptr;
        while (p != nullptr) {
            while (tq->next && p->x >= tq->next->x) {
                tq = tq->next;
            }
            NET* s = p->next;
            p->next = tq->next;
            tq->next = p;
            p = s;
            tq = ptrAET;
        }
        // 从 AET 表中删除 ymax = i 的结点
        removeEdge(ptrAET, i);
        // 将 NET 中的新点加入 AET, 并用插入法按 X 值递增排序
        p = ptrNET[i]->next;
        Edge* q = ptrAET;
        while (p != nullptr) {
            while (q->next && p->x >= q->next->x) {
                q = q->next;
            }
            NET* s = p->next;
            p->next = q->next;
            q->next = p;
            p = s;
            q = ptrAET;
        }

        // 配对填充颜色
        p = ptrAET->next;
        while (p != nullptr && p->next != nullptr) {
            for (float j = p->x; j <= p->next->x; j++) {
                glVertex2i(static_cast<int>(j), i);
            }
            p = p->next->next;
        }
    }
    glEnd();
    glFlush();
}

int main(int argc, char* argv) {
    // 初始化glut
    glutInit(&argc, &argv);
    // 设置显示模式：单个缓存和使用RGB模型
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // 设置窗口的高度和宽度
    glutInitWindowSize(400, 300);
    // 创建显示窗口
    glutCreateWindow("扫描线区域填充算法");
    // 窗口的背景颜色设置为白色
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 450.0);

    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(scanLineFill);
    glutMainLoop();
    return 0;
}
