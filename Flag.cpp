#include "Flag.h"
#include <cmath>

Flag::Flag(const QImage &image, const int32_t x, const int32_t y)
    : GLDrawable{}
    , plane{image, x, y, true}
    , pole{0.5,30}
{
    plane.setRotation({90, {0,1,0}});
    plane.setPosition({y/2.0f+1.0f,x/2.0f+2.0f,0});
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
    glTranslatef(_pos.x(), _pos.y(), _pos.z());
    glRotatef(_rot.angle, _rot.axis.x(), _rot.axis.y(), _rot.axis.z());
    pole.Draw();
    glTranslatef(tplane.x(),tplane.y(),tplane.z());
    glPushMatrix();
    glScalef(1.5,1.5,1.5);
    plane.Draw();
    glPopMatrix();
    glPopMatrix();
}

void Flag::Update(double dt){
    if( !animated )
        return;

     tplane = tplane + (QVector3D{0,0,1}*speed*dt) * (dir ? 1.0 : -1.0);

     if( tplane.z() > 30 ){
         tplane = {0,0,30};
         dir = !dir;
     }

     if( tplane.z() < 10  ){
         tplane = {0,0,10};
         dir = !dir;
     }
}
