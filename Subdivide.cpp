#include "Subdivide.h"
#include <cmath>
#include <QOpenGLFunctions>
#include <vector>

using namespace std;


Subdivide::Subdivide(const double length, const QColor &color)
    : GLDrawable{}
    , _length{length}
    , _color{color}
    , tindices{{1,4,0}, {4,9,0}, {4,5,9}, {8,5,4}, {1,8,4},
{1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
{3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
{10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}}
    , vdata{
{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
          }
{


}

bool Subdivide::Initialize(){
    initializeOpenGLFunctions();

    if( Subdivide_list != 0 ){
        glDeleteLists(Subdivide_list, 1);
        Subdivide_list = 0;
    }
    Subdivide_list = glGenLists(1);
    glNewList(Subdivide_list, GL_COMPILE);
    glColor3d(_color.redF(), _color.greenF(), _color.blueF());

    vector<QVector3D> data(vdata.size());

    transform(vdata.begin(), vdata.end(), data.begin(), [this](auto &v){
        return v*_length;
    });

    for( auto& index: tindices ){
        subdivide(data[index[0]], data[index[1]], data[index[2]], levels);
    }

    glEndList();
    return true;
}

const Subdivide& Subdivide::operator++(int){
    ++_length;
    Initialize();
    return *this;
}

const Subdivide& Subdivide::operator--(int){
    --_length;
    Initialize();
    return *this;
}

Subdivide& Subdivide::operator++(){
    ++levels;
    Initialize();
    return *this;
}

Subdivide& Subdivide::operator--(){
    if( levels == 0)
        return *this;
    --levels;
    Initialize();
    return *this;
}

void Subdivide::Draw(){
    glPushMatrix();
    glTranslatef(_pos.x(), _pos.y(), _pos.z());
    glCallList(Subdivide_list);
    glPopMatrix();
}

void Subdivide::subdivide(QVector3D &v1, QVector3D &v2, QVector3D v3, int depth){

    QVector3D v12, v23, v31;

    if (depth == 0) {
        drawTriangle(v1, v2, v3);
        return;
    }

    /* calculate midpoints of each side */

    v12 = (v1+v2)/2.0;
    v23 = (v2+v3)/2.0;
    v31 = (v3+v1)/2.0;

    /* extrude midpoints to lie on unit sphere */
    v12.normalize();
    v23.normalize();
    v31.normalize();

    v12 *= _length;
    v23 *= _length;
    v31 *= _length;
    /* recursively subdivide new triangles */
    subdivide(v1, v12, v31, depth-1);
    subdivide(v2, v23, v12, depth-1);
    subdivide(v3, v31, v23, depth-1);
    subdivide(v12, v23, v31, depth-1);
}


/* normalized cross product of non-parallel vectors */
void Subdivide::normCrossProd (QVector3D& u, QVector3D& v, QVector3D& n)
{
  n[0] = u[1]*v[2] - u[2]*v[1];
  n[1] = u[2]*v[0] - u[0]*v[2];
  n[2] = u[0]*v[1] - u[1]*v[0];
  n.normalize();
  n *= _length;
}

void Subdivide::normFace(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3])
{
  QVector3D d1, d2, n;

  int k;
  for (k = 0; k < 3; k++) {
    d1[k] = v1[k] - v2[k];
    d2[k] = v2[k] - v3[k];
  }
  normCrossProd(d1, d2, n);
  glNormal3f(n.x(), n.y(), n.z());
}


void Subdivide::drawTriangle(QVector3D &v1, QVector3D &v2, QVector3D v3){
    glBegin(GL_TRIANGLES);
    GLfloat f1[] = {v1.x(), v1.y(), v1.z()};
    GLfloat f2[] = {v2.x(), v2.y(), v2.z()};
    GLfloat f3[] = {v3.x(), v3.y(), v3.z()};
        normFace(f1, f2, f3);
        glVertex3f(v1.x(), v1.y(), v1.z());
        glVertex3f(v2.x(), v2.y(), v2.z());
        glVertex3f(v3.x(), v3.y(), v3.z());
      glEnd();
}
