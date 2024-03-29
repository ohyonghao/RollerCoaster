#ifndef CYLINDER_H
#define CYLINDER_H

#include <QObject>
#include "GLDrawable.h"

#include <QColor>
#include <QImage>
#include <QOpenGLTexture>

class Cylinder : public GLDrawable
{
public:
    Cylinder(const double radius=1.0, const double length=1.0, const QColor& color=Qt::white);

    // Initializer. Creates the display list.
    bool Initialize() override;

    // Does the drawing.
    void Draw() override;

    void setTexture(const QImage& ) {}
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}

private:
    void setTexture();
    double _length{1.0};
    double _r{1.0};
    QVector3D _pos{0,0,0};
    Rotation _rot{0,{0,0,0}};

    QColor _color{Qt::white};

    GLuint Cylinder_list;  // The display list for the Cube.

    QImage _image;
    QOpenGLTexture* texture{nullptr};

    bool tex{false};
};

#endif // CYLINDER_H
