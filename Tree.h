#ifndef TREE_H
#define TREE_H
#include "GLDrawable.h"
#include "Cone.h"
#include "Cylinder.h"
#include <QVector3D>
#include <QImage>

class Tree : public GLDrawable
{
public:
    Tree(double size=1);
    // Initializer. Creates the display list.
    bool Initialize() override;

    // Does the drawing.
    void Draw() override;

    void setTexture(const QImage& ) {}
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}

private:
    void setTexture();
    QVector3D _pos{0,0,0};
    Rotation _rot{0,{0,0,0}};

    double _size;

    Cone treetop;
    Cylinder trunk;
};

#endif // TREE_H
