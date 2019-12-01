#ifndef FLAG_H
#define FLAG_H

#include "GLDrawable.h"
#include "Plane.h"
#include "Cylinder.h"

class Flag : public GLDrawable
{
public:
    Flag(const QImage& image, const int32_t x = 1, const int32_t y = 1);

    // Destructor. Frees the display lists and texture object.
    ~Flag() override{}

    // Initializer. Creates the display list.
    bool    Initialize() override;

    // Does the drawing.
    void    Draw() override;

    // Animate the thing
    void Update(double dt) override;
    bool isAnimated() override {return true;}

    void toggleAnimation(){animated = !animated;}

    // Translate
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}

private:
    Plane plane;
    Cylinder pole;
    QVector3D _pos{0,0,0};
    Rotation _rot{0, {0,0,1}};
    bool    initialized{false};    // Whether or not we have been initialised.

    bool animated{false};
    float pole_posn{30.0};
    const float speed{4.0};
    bool dir{1};
    QVector3D tplane{0,0,30};
};

#endif // FLAG_H
