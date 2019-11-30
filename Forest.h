#ifndef FOREST_H
#define FOREST_H

#include "GLDrawable.h"
#include "Tree.h"

class Forest : public GLDrawable
{
public:
    Forest(const int32_t x = 20, const int32_t y = 20 , const int32_t tree_size = 6);
    // Initializer. Creates the display list.
    bool Initialize() override;

    // Does the drawing.
    void Draw() override;

    void setTexture(const QImage& ) {}
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}

private:
    int32_t size_x;
    int32_t size_y;

    QVector3D _pos;
    Rotation _rot;

    Tree tree;
};

#endif // FOREST_H
