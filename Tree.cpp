#include "Tree.h"
#include "GLDrawable.h"
#include "Cone.h"
#include "Cylinder.h"
#include <QOpenGLFunctions>

Tree::Tree(double size)
    : GLDrawable{}
    , _size{size}
    , treetop{0.5*size, size, {107,142,35}}
    , trunk{0.125*size, size, {139,69,19}}
{

}

bool Tree::Initialize(){
    initializeOpenGLFunctions();

    trunk.Initialize();
    treetop.Initialize();

    return true;
}

void Tree::Draw(){
    glPushMatrix();
    glTranslatef(_pos.x(), _pos.y(), _pos.z());
    trunk.Draw();
    glTranslatef(0, 0, _pos.z()+2.0*_size);
    glRotatef(180, 0,1,0);
    treetop.Draw();
    glPopMatrix();
}
