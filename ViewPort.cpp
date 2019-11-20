#include "ViewPort.h"
#include <vector>
#include <QOpenGLFunctions>
#include <cmath>
#include <GL/glu.h>

using namespace std;

ViewPort::ViewPort()
    :QOpenGLFunctions()
{
}

ViewPort::ViewPort(double phi, double theta, double dist, double x, double y)
    :QOpenGLFunctions()
    ,_phi{phi}
    ,_theta{theta}
    ,_dist{dist}
    ,_x_at{x}
    ,_y_at{y}
{

}

void ViewPort::Initialize(){
    initializeOpenGLFunctions();
}

void ViewPort::updateDown(){}

void ViewPort::updateCamera(){}

void ViewPort::LookAtView(){
    vector<double> eye(3);
    // ############ Camera ##############
    // Set up the viewing transformation. The viewer is at a distance
    // dist from (x_at, y_ay, 2.0) in the direction (theta, phi) defined
    // by two angles. They are looking at (x_at, y_ay, 2.0) and z is up.
    // Initial viewing parameters.

    eye[0] = _x_at + _dist * cos(_theta * M_PI / 180.0) * cos(_phi * M_PI / 180.0);
    eye[1] = _y_at + _dist * sin(_theta * M_PI / 180.0) * cos(_phi * M_PI / 180.0);
    eye[2] = 2.0 + _dist * sin(_phi * M_PI / 180.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], _x_at, _y_at, 2.0, 0.0, 0.0, 1.0);

}
