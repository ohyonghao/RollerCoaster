/*
 * Ground.cpp: A class for drawing the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Ground.h"
#include "libtarga.h"
#include <GL/glu.h>
#include <QOpenGLFunctions>
#include <QImage>
#include <QOpenGLTexture>
#include <iostream>

using namespace std;
// Destructor
Ground::~Ground()
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
Ground::Initialize(void)
{

    QImage image(":/grass.tga");
    if(image.isNull()){
        cout << "Failed to load" << endl;
        return false;
    }
    initializeOpenGLFunctions();
    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.

    texture = new QOpenGLTexture(image.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Now do the geometry. Create the display list.
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// The surface normal is up for the ground.
	glNormal3f(0.0, 0.0, 1.0);

	// Turn on texturing and bind the grass texture.
    glEnable(GL_TEXTURE_2D);
    texture->bind();

	// Draw the ground as a quadrilateral, specifying texture coordinates.
	glBegin(GL_QUADS);
	    glTexCoord2f(100.0, 100.0);
	    glVertex3f(50.0, 50.0, 0.0);
	    glTexCoord2f(-100.0, 100.0);
	    glVertex3f(-50.0, 50.0, 0.0);
	    glTexCoord2f(-100.0, -100.0);
	    glVertex3f(-50.0, -50.0, 0.0);
	    glTexCoord2f(100.0, -100.0);
	    glVertex3f(50.0, -50.0, 0.0);
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
Ground::Draw(void)
{
    glPushMatrix();
    glCallList(display_list);
    glPopMatrix();
}


