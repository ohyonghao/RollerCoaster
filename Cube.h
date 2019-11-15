#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QVector3D>
#include <QColor>
#include <QImage>
#include <QOpenGLTexture>
#include "GLDrawable.h"
class Cube : public GLDrawable
{
public:
    Cube(uint32_t size, const QVector3D& pos={0.0,0.0,0.0}, const QColor& color={0,0,0});
    Cube(uint32_t size, const QImage& texture, const QVector3D& pos={0.0,0.0,0.0});
    ~Cube() override{}


    // Initializer. Creates the display list.
    bool    Initialize(void) override;

    // Does the drawing.
    void    Draw(void) override;

    void setTexture(const QImage& );

private:
    void setTexture();
    uint32_t _size;
    QVector3D _pos;
    QColor _color;

    GLuint Cube_list;  // The display list for the Cube.

    QImage _image;
    QOpenGLTexture* texture{nullptr};

    bool tex{false};
};

#endif // Cube_H
