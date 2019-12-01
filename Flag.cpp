#include "Flag.h"
#include <cmath>

Flag::Flag(const QImage &image, const int32_t x, const int32_t y)
    : GLDrawable{}
    , plane{image, x, y, true}
    , pole{0.5,30}
{
    plane.setRotation({90, {0,1,0}});
    plane.setPosition({y/2.0f+2.0f,x/2.0f+2.0f,0});
}

bool Flag::Initialize(){
    initializeOpenGLFunctions();
    pole.Initialize();
    plane.Initialize();
    initialized = true;
    return initialized;
}

void Flag::Draw(){
    glPushMatrix();
    glRotatef(_rot.angle, _rot.axis.x(), _rot.axis.y(), _rot.axis.z());
    glTranslatef(_pos.x(), _pos.y(), _pos.z());
    pole.Draw();
    glTranslatef(0,0,30);
    plane.Draw();
    glPopMatrix();
}
