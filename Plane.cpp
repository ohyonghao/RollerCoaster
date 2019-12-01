#include "Plane.h"
#include "GLDrawable.h"

#include <GL/glu.h>
#include <QOpenGLFunctions>
#include <QImage>
#include <QOpenGLTexture>
#include <iostream>

using namespace std;
Plane::Plane(const QImage& image, uint32_t x, uint32_t y)
    :GLDrawable{ }
    , _image{image}
    , size_x{x}
    , size_y{y}
{ }
// Destructor
Plane::~Plane()
{
    if ( initialized )
    {
    glDeleteLists(display_list, 1);
    delete texture;
    }
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Plane::Initialize(void)
{
    if(_image.isNull()){
        cout << "Failed to load" << endl;
        return false;
    }
    initializeOpenGLFunctions();
    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.

    texture = new QOpenGLTexture(_image.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Now do the geometry. Create the display list.
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE);
    // Use white, because the texture supplies the color.
    glColor3f(1.0, 1.0, 1.0);

    // The surface normal is up for the Plane.
    glNormal3f(0.0, 0.0, 1.0);

    // Turn on texturing and bind the grass texture.
    glEnable(GL_TEXTURE_2D);
    texture->bind();

    // Draw the Plane as a quadrilateral, specifying texture coordinates.
    glBegin(GL_QUADS);
        glTexCoord2f(100.0, 100.0);
        glVertex3f(size_x, size_y, 0.0);
        glTexCoord2f(-100.0, 100.0);
        glVertex3f(-size_x, size_y, 0.0);
        glTexCoord2f(-100.0, -100.0);
        glVertex3f(-size_x, -size_y, 0.0);
        glTexCoord2f(100.0, -100.0);
        glVertex3f(size_x, -size_y, 0.0);
    glEnd();

    // Turn texturing off again, because we don't want everything else to
    // be textured.
    glDisable(GL_TEXTURE_2D);
    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display list we set up earlier.
void
Plane::Draw(void)
{
    glPushMatrix();
    glCallList(display_list);
    glPopMatrix();
}
