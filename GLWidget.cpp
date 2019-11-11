#include "GLWidget.h"
#include <math.h>
#include <vector>
#include <GL/glu.h>
#include <QTimer>
#include <QMouseEvent>

using namespace std;
GLWidget::GLWidget(QWidget* parent)
    : QGLWidget{QGLFormat(/* Additional format options*/),parent}
{
    connect(&coasterTimer, &QTimer::timeout,this, &GLWidget::updateCoaster);
    setMouseTracking(false); // Only track when button pressed
}

GLWidget::~GLWidget(){
    delete ground;
};

void GLWidget::initializeGL(){
    // Clear to skyblue
    qglClearColor(QColor(static_cast<int>(0.53*255.0),
                         static_cast<int>(0.81*255.0),
                         static_cast<int>(0.92*255.0), 255));

    // Turn on depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Turn on back face culling. faces with normals away from viewer will not be drawn
    glEnable(GL_CULL_FACE);

    // Ambient and diffuse lighting track the current color.
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // Turn on normal vector normalization. You don't have to give unit
    // normal vector, and you can scale objects.
    glEnable(GL_NORMALIZE);

    // Set up the viewport.
    glViewport(0, 0, width(), height());



    // Set up the persepctive transformation.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double fov_y = 360.0 / M_PI * atan(static_cast<double>(height()) * tan(FOV_X * M_PI / 360.0) / static_cast<double>(width()));
    gluPerspective(fov_y,
                   static_cast<double>(width()) / static_cast<double>(height()), 1.0, 1000.0);


    // Do some light stuff. Diffuse color, and zero specular color
    // turns off specular lighting.
    glLightfv(GL_LIGHT0, GL_DIFFUSE, vector<float>{1.0f, 1.0f, 1.0f, 1.0f}.data());
    glLightfv(GL_LIGHT0, GL_SPECULAR, vector<float>{0.0f, 0.0f, 0.0f, 1.0f}.data());

    // Initialize our objects
    if( ground == nullptr )
        ground = new Ground;
    ground->Initialize();

    if( track == nullptr )
        track = new Track;
    track->Initialize();

    coasterTimer.start(10);

}

QSize GLWidget::sizeHint() const{
    return QSize{800,600};
}


void GLWidget::resizeGL(int width, int height){
    if ( height == 0 ) height = 1;

    glViewport(0,0,width,height);

    // Set up the persepctive transformation.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double fov_y = 360.0f / M_PI * atan(static_cast<double>(height) * tan(FOV_X * M_PI / 360.0) / static_cast<double>(width));
    gluPerspective(fov_y,
                   static_cast<double>(width) / static_cast<double>(height), 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::paintGL(){
    //initializeGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vector<double> eye(3);
    // ############ Camera ##############
    // Set up the viewing transformation. The viewer is at a distance
    // dist from (x_at, y_ay, 2.0) in the direction (theta, phi) defined
    // by two angles. They are looking at (x_at, y_ay, 2.0) and z is up.
    // Initial viewing parameters.

    eye[0] = x_at + dist * cos(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[1] = y_at + dist * sin(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[2] = 2.0 + dist * sin(phi * M_PI / 180.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], x_at, y_at, 2.0, 0.0, 0.0, 1.0);

    // ########### Light ###############
    // Position the light source. This has to happen after the viewing
    // transformation is set up, so that the light stays fixed in world
    // space. This is a directional light - note the 0 in the w component.
    glLightfv(GL_LIGHT0, GL_POSITION, vector<float>{1.0,1.0,1.0,0.0}.data());


    // Draw Objects
    ground->Draw();
    track->Draw();
}

void GLWidget::updateCoaster(){
    // Animate the train.
    track->Update(coasterTimer.interval()/1000.0);
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    downPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    double dx = (downPos.x() - event->x())/static_cast<double>(width());
    double dy = (downPos.y() - event->y())/static_cast<double>(height());

    switch ( event->buttons() )
    {
    case Qt::LeftButton:
        // Left button changes the direction the viewer is looking from.
        theta = theta_down + 360.0 * dx;
        while ( theta >= 360.0 )
            theta -= 360.0;
        while ( theta < 0.0 )
            theta += 360.0;
        phi = phi_down + 90.0 * dy;
        if ( phi > 89.0 )
            phi = 89.0;
        if ( phi < -5.0 )
            phi = -5.0;
        break;
    case Qt::MiddleButton:
        // Middle button moves the viewer in or out.
        dist = dist_down - ( 0.5 * dist_down * dy );
        if ( dist < 1.0 )
            dist = 1.0;
        break;
    case Qt::RightButton: {
        // Right mouse button moves the look-at point around, so the world
        // appears to move under the viewer.
        double	x_axis[2];
        double	y_axis[2];

        x_axis[0] = -sin(theta * M_PI / 180.0);
        x_axis[1] = cos(theta * M_PI / 180.0);
        y_axis[0] = x_axis[1];
        y_axis[1] = -x_axis[0];

        x_at = x_at_down + 100.0 * ( x_axis[0] * dx + y_axis[0] * dy );
        y_at = y_at_down + 100.0 * ( x_axis[1] * dx + y_axis[1] * dy );
        }
        break;
    default:
        return; // only handle mouse button events
    }
    updateGL();
}
