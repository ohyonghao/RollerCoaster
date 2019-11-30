#include "Cone.h"
#include <cmath>
#include <QOpenGLFunctions>

Cone::Cone( double radius, double length)
    : GLDrawable{}
    , _length{length}
    , _r{radius}
{

}

void Cone::setTexture(){
    if( _image.isNull() ){
        tex = false;
        return;
    }
    if( texture != nullptr){
        delete texture;
        texture = nullptr;
    }
    texture = new QOpenGLTexture(_image.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    tex = true;
}
bool Cone::Initialize(){
    initializeOpenGLFunctions();

    // Load texture
    setTexture();

    if( Cone_list != 0 ){
        glDeleteLists(Cone_list,1);
        Cone_list = 0;
    }
    Cone_list = glGenLists(1);
    glNewList(Cone_list, GL_COMPILE);

    glColor3d(_color.redF(), _color.greenF(), _color.blueF());
    if( tex ){
        // Turn on the texturing
        glEnable(GL_TEXTURE_2D);
        texture->bind();
    }
    // Translate the position
    glTranslatef( _pos.x(), _pos.y(), _pos.z() );

    // draw the Cone

    // Draw the Tube
    glBegin(GL_TRIANGLE_STRIP);
    float angle = 0.0;
    float angle_stepsize = 0.1;
    float x = 0.0;
    float y = 0.0;
    while( angle < 2*M_PI){
        x = _r * cos(angle);
        y = _r * sin(angle);
        glVertex3f(x, y, _length);
        glVertex3f(0, 0, 0.0);
        angle = angle + angle_stepsize;
    }
    glVertex3f( _r, 0.0, _length);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();

    // Draw the circle on top of Cone
    glColor3d(_color.redF(), _color.greenF(), _color.blueF());
    if( tex ){
        // Turn on the texturing
        glEnable(GL_TEXTURE_2D);
        texture->bind();
    }
    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*M_PI){
        x = _r * cos(angle);
        y = _r * sin(angle);
        glVertex3f(x, y, _length);
        angle = angle + angle_stepsize;
    }
    glVertex3f(_r, 0.0, _length);
    glEnd();
    // Turn off texturing
    if( tex )
        glDisable(GL_TEXTURE_2D);
    glEndList();

    return true;
}
void Cone::Draw(){
    glPushMatrix();
    glCallList(Cone_list);
    glPopMatrix();
}
