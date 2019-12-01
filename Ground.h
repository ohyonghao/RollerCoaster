/*
 * Ground.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef GROUND_H
#define GROUND_H

#include "GLDrawable.h"
#include <QOpenGLTexture>
#include <QVector3D>
#include <QImage>

#include "Plane.h"

class Ground : public GLDrawable {
  private:
    bool    initialized{false};    // Whether or not we have been initialised.
    QVector3D _pos{0,0,0};
    Rotation _rot{0, {0,0,1}};
    Plane plane;

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Ground(const QImage& image, const int32_t x = 1, const int32_t y = 1);

    // Destructor. Frees the display lists and texture object.
    ~Ground() override;

    // Initializer. Creates the display list.
    bool    Initialize(void) override;

    // Does the drawing.
    void    Draw(void) override;

    // Translate
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}
};


#endif
