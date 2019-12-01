#ifndef PLANE_H
#define PLANE_H

#include "GLDrawable.h"
#include <QOpenGLTexture>
#include <QVector3D>
#include <QImage>

class Plane : public GLDrawable
{
public:
  // Constructor. Can't do initialization here because we are
  // created before the OpenGL context is set up.
  Plane( const QImage& image, uint32_t x = 1, uint32_t y = 1);

  // Destructor. Frees the display lists and texture object.
  ~Plane() override;

  // Initializer. Creates the display list.
  bool    Initialize(void) override;

  // Does the drawing.
  void    Draw(void) override;

  // Translate
  void setPosition(const QVector3D& pos) override {_pos = pos;}
  void setRotation(const Rotation& rot) override {_rot = rot;}
private:
  GLuint display_list{0};   // The display list that does all the work.
  GLuint  texture_obj{0};    // The object for the grass texture.
  bool    initialized{false};    // Whether or not we have been initialised.
  QImage _image;
  QOpenGLTexture* texture;
  QVector3D _pos{0,0,0};
  Rotation _rot{0, {0,0,1}};

  int32_t size_x;
  int32_t size_y;
};

#endif // PLANE_H
