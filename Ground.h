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

class Ground : public GLDrawable {
  private:
    GLubyte display_list{0};   // The display list that does all the work.
    GLuint  texture_obj{0};    // The object for the grass texture.
    bool    initialized{false};    // Whether or not we have been initialised.
    QOpenGLTexture* texture;
    QVector3D _pos{0,0,0};
    Rotation _rot{0, {0,0,1}};

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Ground():GLDrawable{} { }

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
