/*
 * Ground.cpp: A class for drawing the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Ground.h"
#include <GL/glu.h>
#include <QOpenGLFunctions>
#include <QImage>
#include <QOpenGLTexture>
#include <iostream>

using namespace std;
Ground::Ground(const QImage& image, uint32_t x, uint32_t y)
    : GLDrawable{}
    , plane{image, x, y}
{

}

// Destructor
Ground::~Ground()
{
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Ground::Initialize(void)
{
    initializeOpenGLFunctions();
    plane.Initialize();
    initialized = true;

    return true;
}


// Draw just calls the display list we set up earlier.
void
Ground::Draw(void)
{
    plane.Draw();
}


