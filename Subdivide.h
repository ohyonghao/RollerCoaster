#ifndef SUBDIVIDE_H
#define SUBDIVIDE_H

#include "GLDrawable.h"
#include <QColor>
#include <QVector3D>

class Subdivide : public GLDrawable
{
public:

    Subdivide(const float length=1.0, const QColor& color=Qt::white);
    // Initializer. Creates the display list.
    bool Initialize() override;

    // Does the drawing.
    void Draw() override;

    void setTexture(const QImage& ) {}
    void setPosition(const QVector3D& pos) override {_pos = pos;}
    const QVector3D& getPosition(){return _pos;}
    void setRotation(const Rotation& rot) override {_rot = rot;}
    void toggleMesh(){_mesh = !_mesh; Initialize();}

    Subdivide& operator++();
    const Subdivide& operator++(int);
    Subdivide& operator--();
    const Subdivide& operator--(int);

private:
    static constexpr float X{0.525731112119133696f};
    static constexpr float Z{0.850650808352039932f};


    void normCrossProd (const QVector3D &u, const QVector3D &v, QVector3D& n);
    void normFace(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3);

    void subdivide( QVector3D& v1, QVector3D& v2, QVector3D v3, int depth );
    void drawTriangle(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3);
    float _length{1.0};
    int32_t levels{0};
    Rotation _rot{0,{0,0,0}};
    QVector3D _pos{0,0,0};

    QColor _color{Qt::white};

    GLuint Subdivide_list;  // The display list for the Cube.

    /* triangle indices */
    const std::vector<std::vector<size_t>> tindices;
    /* vertex data array */
    const std::vector<QVector3D> vdata;

    bool _mesh{false};
};

#endif // SUBDIVIDE_H
