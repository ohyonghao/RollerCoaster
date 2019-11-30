#include "Cube.h"
#include "GLDrawable.h"
#include <GL/glu.h>

Cube::Cube(double size, const QVector3D &pos, const QColor &color )
    : GLDrawable{}
    , _size{size}
    , _pos{pos}
    , _color{color}
{
}
Cube::Cube(double size, const QImage& image, const QVector3D &pos )
    : GLDrawable{}
    , _size{size}
    , _pos{pos}
    , _color{Qt::white}
    , _image{image}
    , tex{true}
{

}

void Cube::setTexture(const QImage &image){
    _image = image;
    tex = true;
    // Reinitialize
    Initialize();
}
void Cube::setTexture(){
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
bool Cube::Initialize(){
    initializeOpenGLFunctions();

    // Load texture
    setTexture();

    float s = static_cast<float>(_size);
    if( Cube_list != 0 ){
        glDeleteLists(Cube_list,1);
        Cube_list = 0;
    }
    Cube_list = glGenLists(1);
    glNewList(Cube_list, GL_COMPILE);

    glColor3d(_color.redF(), _color.greenF(), _color.blueF());
    if( tex ){
        // Turn on the texturing
        glEnable(GL_TEXTURE_2D);
        texture->bind();
    }
    // Translate the position
    glTranslatef( _pos.x(), _pos.y(), _pos.z() );

    // An idea for this is to store the texture pointers as an
    // array of 6, one for each side. We need a way to set
    // some of them equal to each other and maintain this
    // setting easily.

    // Perhaps having a power enum for each face, then we can use
    // a set of these when we call setTexture.
    // say, setTexture( QImage, FACE_FLAGS ){
    // perhaps simply roll it out as
    // if( FACE_FLAGS | TOP ) tex[TOP] = texture
    // }

    // It would be nice to come up with a better version of this, but this
    // should suffice for up to a 32-gon

    // draw the cube
    glBegin(GL_QUADS);
    // Top
    glNormal3f(0.0f, 0.0f, 1.0f);
    if(tex) glTexCoord2f(s, s);
    glVertex3f(s, s, 2*s);
    if(tex) glTexCoord2f(-s, s);
    glVertex3f(-s, s, 2*s);
    if(tex) glTexCoord2f(-s, -s);
    glVertex3f(-s, -s, 2*s);
    if(tex) glTexCoord2f(s, -s);
    glVertex3f(s, -s, 2*s);

    // Bottom
    glNormal3f(0.0f, 0.0f, -1.0f);
    if(tex) glTexCoord2f(s, -s);
    glVertex3f(s, -s, 0.0f);
    if(tex) glTexCoord2f(-s, -s);
    glVertex3f(-s, -s, 0.0f);
    if(tex) glTexCoord2f(-s, s);
    glVertex3f(-s, s, 0.0f);
    if(tex) glTexCoord2f(s, s);
    glVertex3f(s, s, 0.0f);

    // Front
    glNormal3f(1.0f, 0.0f, 0.0f);
    if(tex) glTexCoord2f(1.0, -1.0);
    glVertex3f(s, s, 0.0f);
    if(tex) glTexCoord2f(1.0, 1.0);
    glVertex3f(s, s, 2*s);
    if(tex) glTexCoord2f(-1.0, 1.0);
    glVertex3f(s, -s, 2*s);
    if(tex) glTexCoord2f(-1.0, -1.0);
    glVertex3f(s, -s, 0.0f);

    // Back
    glNormal3f(-1.0f, 0.0f, 0.0f);
    if(tex) glTexCoord2f(1.0, -1.0);
    glVertex3f(-s, s, 2*s);
    if(tex) glTexCoord2f(1.0, 1.0);
    glVertex3f(-s, s, 0.0f);
    if(tex) glTexCoord2f(-1.0, 1.0);
    glVertex3f(-s, -s, 0.0f);
    if(tex) glTexCoord2f(-1.0, -1.0);
    glVertex3f(-s, -s, 2*s);

    // Right
    glNormal3f(0.0f, 1.0f, 0.0f);
    if(tex) glTexCoord2f(1.0, -1.0);
    glVertex3f(s, s, 2*s);
    if(tex) glTexCoord2f(1.0, 1.0);
    glVertex3f(s, s, 0.0f);
    if(tex) glTexCoord2f(-1.0, 1.0);
    glVertex3f(-s, s, 0.0f);
    if(tex) glTexCoord2f(-1.0, -1.0);
    glVertex3f(-s, s, 2*s);

    // Left
    glNormal3f(0.0f, -1.0f, 0.0f);
    if(tex) glTexCoord2f(1.0, -1.0);
    glVertex3f(s, -s, 0.0f);
    if(tex) glTexCoord2f(1.0, 1.0);
    glVertex3f(s, -s, 2*s);
    if(tex) glTexCoord2f(-1.0, 1.0);
    glVertex3f(-s, -s, 2*s);
    if(tex) glTexCoord2f(-1.0, -1.0);
    glVertex3f(-s, -s, 0.0f);
    glEnd();

    // Turn off texturing
    if( tex )
        glDisable(GL_TEXTURE_2D);
    glEndList();

    return true;
}
void Cube::Draw(){
    glPushMatrix();
    glCallList(Cube_list);
    glPopMatrix();
}
