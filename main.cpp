//
// main.cpp
// solarsystem
//

// 使用GLUT的基本头文件
#include <GL/glut.h>
#include "solarsystem.hpp"

// 创建图形窗口的基本宏
#define WINDOW_X_POS 50
#define WINDOW_Y_POS 50
#define WIDTH 700
#define HEIGHT 700

SolarSystem solarsystem;

// 用于注册GLUT的回调
void onDisplay(void){
    solarsystem.onDisplay();
}

void onUpdate(void){
    solarsystem.onUpdate();
}

void onKeyboard(unsigned char key, int x, int y){
    solarsystem.onKeyboard(key, x, y);
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS);
    glutCreateWindow("SolarSystem at Shiyanlou");
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onUpdate);
    glutKeyboardFunc(onKeyboard);
    glutMainLoop();

    return 0;
}

