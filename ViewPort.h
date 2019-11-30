#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QObject>
#include <QPoint>
#include <QOpenGLFunctions>
#include <QVector3D>

class ViewPort : protected QOpenGLFunctions
{
public:
    ViewPort();
    ViewPort(double phi, double theta, double dist, double x, double y);

    void Initialize();
    void LookAtView();
    void updateDown();
    void updateCamera();

    double& phi(){return _phi;}
    double& theta(){return _theta;}
    double& dist(){return _dist;}
    double& x(){return _x_at;}
    double& y(){return _y_at;}

    void moveEyeBy(const QVector3D& dir){_eye += dir;}
    void moveLookAtBy(const QVector3D& dir){_at += dir;}
    void moveUpBy(const QVector3D& dir){_up += dir;}

    void setEye(const QVector3D& eye){_eye = eye;}
    void setLookAt(const QVector3D& lookat){_at = lookat;}
    void setUp(const QVector3D& up){_up = up;}

    const QVector3D eye() const{return _eye;}
    const QVector3D lookAt() const{return _at;}
    const QVector3D up() const{return _up;}

private:
    // Camera
    double _phi{0};
    double _theta{0};
    double _dist{0};
    double _x_at{0};
    double _y_at{0};

    QVector3D _eye{70,1,75};
    QVector3D _at{1,1,2};
    QVector3D _up{0,0,1};

};

#endif // VIEWPORT_H
