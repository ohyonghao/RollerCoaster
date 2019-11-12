#ifndef GLDRAWABLE_H
#define GLDRAWABLE_H

#include <QObject>
#include <QOpenGLFunctions>

class GLDrawable : protected QOpenGLFunctions
{
public:
    GLDrawable();
    virtual ~GLDrawable()=default;

    virtual void Draw()=0;
    virtual bool Initialize()=0;
    virtual void Update(double){}
    virtual bool isAnimated(){return false;}
};

#endif // GLDRAWABLE_H
