#ifndef GLDRAWABLE_H
#define GLDRAWABLE_H

#include <QObject>
#include <QOpenGLFunctions>
#include "ViewPort.h"

struct Rotation{
    float angle;
    QVector3D axis;
};

class GLDrawable : protected QOpenGLFunctions
{
public:
    GLDrawable();
    virtual ~GLDrawable()=default;

    virtual void Draw()=0;
    virtual bool Initialize()=0;
    virtual void setPosition(const QVector3D&)=0;
    virtual void setRotation(const Rotation& ) = 0;
    virtual void Update(double){}
    virtual bool isAnimated(){return false;}
    virtual void attachView(ViewPort*){}
};

#endif // GLDRAWABLE_H
