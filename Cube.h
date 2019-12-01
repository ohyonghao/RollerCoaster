#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QVector3D>
#include <QColor>
#include <QImage>
#include <QOpenGLTexture>
#include "GLDrawable.h"
class Cube : public GLDrawable
{
public:
    Cube(double size, const QColor& color={0,0,0});
    Cube(double size, const QImage& texture, const QVector3D& pos={0.0,0.0,0.0});
    ~Cube() override{}


    // Initializer. Creates the display list.
    bool    Initialize(void) override;

    // Does the drawing.
    void    Draw(void) override;

    void setTexture(const QImage& );
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}

private:
    void setTexture();
    double _size{1.0};
    QVector3D _pos{0.0,0.0,0.0};
    Rotation _rot{0,{0,0,0}};

    QColor _color{Qt::black};

    GLuint Cube_list;  // The display list for the Cube.

    QImage _image;
    QOpenGLTexture* texture{nullptr};

    bool tex{false};
};

#endif // Cube_H
