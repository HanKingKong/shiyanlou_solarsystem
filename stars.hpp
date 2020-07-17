#include <GL/glut.h>

class Star {
public:
    // 星球的运行半径
    GLfloat radius;
    // 公转、自转速度
    GLfloat speed, selfSpeed;
    // 星球的中心与父节点星球中心的距离
    GLfloat distance;
    GLfloat rgbaColor[4];

    // 父节点星球
    Star* parentStar;

    Star(GLfloat radius, GLfloat distance,
          GLfloat speed,  GLfloat selfSpeed,
          Star* parent);

    // 对一般的星球的移动、旋转等活动进行绘制
    void drawStar();

    virtual void draw() { drawStar(); }
    virtual void update(long timeSpan);
protected:
    GLfloat alphaSelf, alpha;

};


// 特殊材质的星球 
class Planet : public Star {
public:
    // 构造函数
    Planet(GLfloat radius, GLfloat distance,
           GLfloat speed,  GLfloat selfSpeed,
           Star* parent, GLfloat rgbColor[3]);
    // 增加对具备自身材质的行星绘制材质
    void drawPlanet();

    virtual void draw() { drawPlanet(); drawStar(); }
};

// 会发光的星球
class LightPlanet : public Planet {
public:
    LightPlanet(GLfloat radius, GLfloat distance,
                GLfloat speed,  GLfloat selfSpeed,
                Star* parent, GLfloat rgbColor[]);

    void drawLight();
    virtual void draw() { drawLight(); drawPlanet(); drawStar(); }
    
};
