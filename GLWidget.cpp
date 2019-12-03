#include "GLWidget.h"
#include <math.h>
#include <vector>
#include <GL/glu.h>
#include <QTimer>
#include <QMouseEvent>
#include <algorithm>
#include "Cube.h"
#include "Ground.h"
#include "Flag.h"
#include "Cone.h"
#include "Forest.h"
#include "Track.h"
#include "Subdivide.h"
#include <iostream>
#include <QMatrix4x4>
using namespace std;
GLWidget::GLWidget(QWidget* parent)
    : QGLWidget{QGLFormat(/* Additional format options*/),parent}
{
    connect(&coasterTimer, &QTimer::timeout,this, &GLWidget::updateCoaster);
    setMouseTracking(false); // Only track when button pressed

    auto track = make_shared<Track>();
    auto building = make_shared<Cube>(10,QImage(":/brick_wall.jpg"));
    auto smallbuilding = make_shared<Cube>(4,QImage(":/wood_wall.jpg"));
    auto forest = make_shared<Forest>(size_x/2, 10);
    auto smallforest = make_shared<Forest>(size_x/2, 10, 4);
    sphere = make_shared<Subdivide>(4,Qt::blue);
    flag = make_shared<Flag>(QImage(":/Flag_of_Taiwan.jpg"),2,3);

    // Initialize our objects
    drawables.push_back(make_shared<Ground>(QImage(":/grass.tga"), 50, 50));
    drawables.push_back(track);
    drawables.push_back(building);
    drawables.push_back(smallbuilding);
    drawables.push_back(flag);
    drawables.push_back(forest);
    drawables.push_back(smallforest);
    drawables.push_back(sphere);

    for(const auto& d: drawables){
        if( d->isAnimated() )
            animated.push_back(d);
    }

    // Idea for later, gldrawable attachview(Viewport*)

    // Add the default view
    views.push_back(ViewPort(45.0,0.0,100.0,0.0,0.0));
    views.push_back(ViewPort());

    track->attachView(&views[1]);
    currentView = &views[0];

    building->setPosition({30,30,0});
    smallbuilding->setPosition({-30,-24,0});
    flag->setPosition({-30,-30,0});
    flag->setRotation({90,{0,0,1}});
    sphere->setPosition({0,0,4});
    forest->setPosition({0,-40,0});
    smallforest->setPosition({-4,-36,0});
}

GLWidget::~GLWidget(){
}


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

    // Initialize drawables
    for( auto& d: drawables )
        d->Initialize();

    // Initialize views
    for( auto& v: views ){
        v.Initialize();
    }
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
    double fov_y = 360.0 / M_PI * atan(static_cast<double>(height) * tan(FOV_X * M_PI / 360.0) / static_cast<double>(width));
    gluPerspective(fov_y,
                   static_cast<double>(width) / static_cast<double>(height), 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::paintGL(){
    //initializeGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentView->LookAtView();

    // ########### Light ###############
    // Position the light source. This has to happen after the viewing
    // transformation is set up, so that the light stays fixed in world
    // space. This is a directional light - note the 0 in the w component.
    glLightfv(GL_LIGHT0, GL_POSITION, vector<float>{1.0,1.0,1.0,0.0}.data());


    // Draw Objects
    for( auto& d: drawables)
        d->Draw();
}

void GLWidget::updateCoaster(){
    // Animate the train.
    auto dt = coasterTimer.interval()/1000.0;
    for( auto &a: animated)
        a->Update(dt);
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    downPos = event->pos();

    phi_down = currentView->phi();
    theta_down = currentView->theta();
    dist_down = currentView->dist();
    x_at_down = currentView->x();
    y_at_down = currentView->y();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    double dx = (downPos.x() - event->x())/static_cast<double>(width());
    double dy = (downPos.y() - event->y())/static_cast<double>(height());

    QVector3D eye;
    QVector3D at;
    switch ( event->buttons() )
    {
    case Qt::LeftButton:
        // Left button changes the direction the viewer is looking from.
        currentView->theta() = theta_down + 360.0 * dx;
        while ( currentView->theta() >= 360.0 )
            currentView->theta() -= 360.0;
        while ( currentView->theta() < 0.0 )
            currentView->theta() += 360.0;
        currentView->phi() = phi_down + 90.0 * dy;
        if ( currentView->phi() > 89.0 )
            currentView->phi() = 89.0;
        if ( currentView->phi() < -5.0 )
            currentView->phi() = -5.0;
        break;
    case Qt::MiddleButton:
        // Middle button moves the viewer in or out.
        currentView->dist() = dist_down - ( 0.5 * dist_down * dy );
        if ( currentView->dist() < 1.0 )
            currentView->dist() = 1.0;
        break;
    case Qt::RightButton: {
        // Right mouse button moves the look-at point around, so the world
        // appears to move under the viewer.
        double	x_axis[2];
        double	y_axis[2];

        x_axis[0] = -sin(currentView->theta() * M_PI / 180.0);
        x_axis[1] = cos(currentView->theta() * M_PI / 180.0);
        y_axis[0] = x_axis[1];
        y_axis[1] = -x_axis[0];

        at.setX( x_at_down + 100.0 * ( x_axis[0] * dx + y_axis[0] * dy ) );
        at.setY( y_at_down + 100.0 * ( x_axis[1] * dx + y_axis[1] * dy ) );
        at.setZ(2.0);
        currentView->setLookAt(at);
        }
        break;
    default:
        return; // only handle mouse button events
    }
    auto lookat = currentView->lookAt();
    eye.setX( lookat.x() + currentView->dist() * cos(currentView->theta() * M_PI / 180.0) * cos(currentView->phi() * M_PI / 180.0) );
    eye.setY( lookat.y() + currentView->dist() * sin(currentView->theta() * M_PI / 180.0) * cos(currentView->phi() * M_PI / 180.0) );
    eye.setZ( lookat.z() + currentView->dist() * sin(currentView->phi() * M_PI / 180.0) );
    currentView->setEye(eye);
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    //cout << "key pressed: " << static_cast<char>(event->key()) << endl;
    switch(event->key()){
    case 'O':
    {
        auto dir{currentView->eye() - currentView->lookAt()};
        dir.normalize();
        currentView->moveEyeBy(dir);
        currentView->moveLookAtBy(dir);
        break;
    }
    case ',':
    {
        auto dir{currentView->lookAt() - currentView->eye()};
        dir.normalize();
        currentView->moveEyeBy(dir);
        currentView->moveLookAtBy(dir);
        break;
    }
    case 'A':
    {
        auto dir{currentView->eye() - currentView->lookAt()};
        dir = QVector3D::crossProduct(dir, currentView->up());
        dir.normalize();
        currentView->moveEyeBy(dir);
        currentView->moveLookAtBy(dir);
        break;
    }
    case 'E':
    {
        auto dir{currentView->lookAt() - currentView->eye()};
        dir = QVector3D::crossProduct(dir, currentView->up());
        dir.normalize();
        currentView->moveEyeBy(dir);
        currentView->moveLookAtBy(dir);
        break;
    }
    case '\'':
    {
        // dir cross up
        // Translate to origin
        auto dir{currentView->lookAt() - currentView->eye()};
        QMatrix4x4 matrix;
        matrix.translate(-currentView->eye());
        matrix.rotate(-1.0, QVector3D::crossProduct(currentView->up(), dir ) );
        matrix.translate(currentView->eye());

        currentView->setEye(matrix*currentView->eye());
        currentView->setLookAt(matrix*currentView->lookAt());
        //currentView->setUp(matrix*currentView->up());
        break;
    }
    case '.':
    {
        // dir cross up
        // Translate to origin
        auto dir{currentView->lookAt() - currentView->eye()};
        QMatrix4x4 matrix;
        matrix.translate(-currentView->eye());
        matrix.rotate(1.0, QVector3D::crossProduct(currentView->up(), dir ) );
        matrix.translate(currentView->eye());

        currentView->setEye(matrix*currentView->eye());
        currentView->setLookAt(matrix*currentView->lookAt());
        //currentView->setUp(matrix*currentView->up());
        break;
    }
    case ';':
    {
        // Translate to origin
        auto dir{currentView->lookAt() - currentView->eye()};
        dir /=2.0;
        QMatrix4x4 matrix;
        matrix.translate(-dir);
        matrix.rotate(-1.0, currentView->up() );
        matrix.translate(dir);
        currentView->setEye(matrix*currentView->eye());
        currentView->setLookAt(matrix*currentView->lookAt());
        break;
    }
    case 'J':
    {
        // Translate to origin
        auto dir{currentView->lookAt() - currentView->eye()};
        dir /=2.0;
        QMatrix4x4 matrix;
        matrix.translate(-dir);
        matrix.rotate(1.0, currentView->up() );
        matrix.translate(dir);
        currentView->setEye(matrix*currentView->eye());
        currentView->setLookAt(matrix*currentView->lookAt());
        break;
    }
    case 'G':
        ++*sphere;
        break;
    case 'C':
        --*sphere;
        break;
    case 'H':
        (*sphere)++;
        sphere->setPosition(sphere->getPosition() + QVector3D{0,0,2});
        break;
    case 'T':
        (*sphere)--;
        sphere->setPosition(sphere->getPosition() - QVector3D{0,0,2});
        break;
    case '1':
        currentView = &views[0];
        break;
    case '2':
        currentView = &views[1];
        break;
    case 'F':
        flag->toggleAnimation();
        break;
    default:
        QGLWidget::keyPressEvent(event);
        return;
    }
}
