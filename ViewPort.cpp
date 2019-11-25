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
    // We want to rethink how we want to store this information.
    // This was good for the original one, but makes the representation
    // too cohesive to a particular use.
    //            [    eye     ] [    center  ] [     up       ]
    // gluLookAt( e_x, e_y, e_z, c_x, c_y, c_z, up_x, up_y, up_z )

    // x_at and y_at is the position we want to be looking at, not sure
    // why z=2.0 always
    // We want z to always be up?

    // An idea:
    // Store it as a translation and as a rotation, basically our x,y,z and phi,theta, tilt
    // We can have a look_at take a vector and then calculate the phi, theta
    // or we store the look_at as another vector... the tilt is determined by the up vector
    // we can calculate the phi,theta from eye and look_at vectors

    // we can store the rotation as a matrix
    // the question is where should we care to store the calculations
    //vector<double> eye(3);
    // ############ Camera ##############
    // Set up the viewing transformation. The viewer is at a distance
    // dist from (x_at, y_at, 2.0) in the direction (theta, phi) defined
    // by two angles. They are looking at (x_at, y_ay, 2.0) and z is up.
    // Initial viewing parameters.

//    eye[0] = _x_at + _dist * cos(_theta * M_PI / 180.0) * cos(_phi * M_PI / 180.0);
//    eye[1] = _y_at + _dist * sin(_theta * M_PI / 180.0) * cos(_phi * M_PI / 180.0);
//    eye[2] = 2.0 + _dist * sin(_phi * M_PI / 180.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(eye[0], eye[1], eye[2], _x_at, _y_at, 2.0, 0.0, 0.0, 1.0);
    gluLookAt(_eye.x(), _eye.y(), _eye.z(), _at.x(), _at.y(), _at.z(), _up.x(), _up.y(), _up.z());
}
