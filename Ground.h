/*
 * Ground.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef GROUND_H
#define GROUND_H

#include "GLDrawable.h"
#include <QOpenGLTexture>

class Ground : public GLDrawable {
  private:
    GLubyte display_list{0};   // The display list that does all the work.
    GLuint  texture_obj{0};    // The object for the grass texture.
    bool    initialized{false};    // Whether or not we have been initialised.
    QOpenGLTexture* texture;

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
};


#endif
