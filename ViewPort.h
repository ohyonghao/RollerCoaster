#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QObject>
#include <QPoint>
#include <QOpenGLFunctions>

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

private:
    // Camera
    double _phi{0};
    double _theta{0};
    double _dist{0};
    double _x_at{0};
    double _y_at{0};


};

#endif // VIEWPORT_H
