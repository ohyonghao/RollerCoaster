#ifndef SUBDIVIDE_H
#define SUBDIVIDE_H

#include "GLDrawable.h"
#include <QColor>
#include <QVector3D>

class Subdivide : public GLDrawable
{
public:

    Subdivide( const double length=1.0, const QColor& color=Qt::white);
    // Initializer. Creates the display list.
    bool Initialize() override;

    // Does the drawing.
    void Draw() override;

    void setTexture(const QImage& ) {}
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    const QVector3D& getPosition(){return _pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}

    Subdivide& operator++();
    const Subdivide& operator++(int);
    Subdivide& operator--();
    const Subdivide& operator--(int);

private:
    static constexpr double X{0.525731112119133696};
    static constexpr double Z{0.850650808352039932};


    void normCrossProd (QVector3D& u, QVector3D& v, QVector3D& n);
    void normFace(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]);

    void subdivide( QVector3D& v1, QVector3D& v2, QVector3D v3, int depth );
    void drawTriangle(QVector3D& v1, QVector3D& v2, QVector3D v3);
    double _length{1.0};
    uint32_t levels{0};
    QVector3D _pos{0,0,0};
    Rotation _rot{0,{0,0,0}};

    QColor _color{Qt::white};

    GLuint Subdivide_list;  // The display list for the Cube.

    /* triangle indices */
    const std::vector<QVector3D> tindices;
    /* vertex data array */
    const std::vector<QVector3D> vdata;

};

#endif // SUBDIVIDE_H
