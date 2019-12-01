/*
 * Track.cpp: A class that draws the train and its track.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Track.h"
#include <cmath>
#include <GL/glu.h>
#include <QOpenGLFunctions>
#include <iostream>
#include <QVector3D>

using namespace std;

// The control points for the track spline.
const int   Track::TRACK_NUM_CONTROLS = 4;
const float Track::TRACK_CONTROLS[TRACK_NUM_CONTROLS][3] =
		{ { -20.0, -20.0, -18.0 }, { 20.0, -20.0, 40.0 },
		  { 20.0, 20.0, -18.0 }, { -20.0, 20.0, 40.0 } };

// The carriage energy and mass
const float Track::TRAIN_ENERGY = 250.0f;

Track::Track()
    : GLDrawable{}
    , train{0.75, Qt::red}
{
}

// Normalize a 3d vector.
static void
Normalize_3(double v[3])
{
    double  l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    if (!( l > 0.0) )
	return;

    v[0] /= l;
    v[1] /= l;
    v[2] /= l;
}


// Destructor
Track::~Track()
{
    if ( initialized )
    {
        glDeleteLists(track_list, 1);
        glDeleteLists(rail_list[0], 1);
        glDeleteLists(rail_list[1], 1);
    }
}


// Initializer. Would return false if anything could go wrong.
bool
Track::Initialize(void)
{
    initializeOpenGLFunctions();
    CubicBspline refined(3, true);
    int		     n_refined;
    double	     p[3];

    texture = make_unique<QOpenGLTexture>(QImage(":/wood_board.png"));

    // Create the track spline.
    track = new CubicBspline(3, true);
    for ( int i = 0 ; i < TRACK_NUM_CONTROLS ; i++ )
	track->Append_Control(TRACK_CONTROLS[i]);

    // Refine it down to a fixed tolerance. This means that any point on
    // the track that is drawn will be less than 0.1 units from its true
    // location. In fact, it's even closer than that.
    track->Refine_Tolerance(refined, 0.1f);
    n_refined = refined.N();

    // Create the display list for the track - just a set of line segments
    // We just use curve evaluated at integer paramer values, because the
    // subdivision has made sure that these are good enough.
    vector<QVector3D> rails[2];
    track_list = glGenLists(1);

    glNewList(track_list, GL_COMPILE);

    glColor3f(1.0f, 1.0, 1.0f);
    glEnable(GL_TEXTURE_2D);
    texture->bind();
    glBegin(GL_QUAD_STRIP);
        for ( int i = 0 ; i <= n_refined ; i++ )
        {
            QVector3D pt1;
            refined.Evaluate_Point(i, p);
            pt1.setX(p[0]);
            pt1.setY(p[1]);
            pt1.setZ(p[2]);

            refined.Evaluate_Point(i+1, p);
            QVector3D pt2;
            pt2.setX(p[0]);
            pt2.setY(p[1]);
            pt2.setZ(p[2]);

            auto dir = pt2 - pt1;
            dir = QVector3D::crossProduct(dir, {0,0,1});
            dir.normalize();
            auto up = pt2 - pt1;
            up = QVector3D::crossProduct(up, dir);
            up.normalize();
            up*=0.0625;

            // Calculate the rail points
            rails[0].push_back(pt1+dir*0.25);
            rails[0].push_back(pt1+dir*0.25+up);

            rails[1].push_back(pt1-dir*0.25);
            rails[1].push_back(pt1-dir*0.25+up);

            // Then the boards
            pt1 = pt1 + dir*0.5;
            p[0] = pt1.x();
            p[1] = pt1.y();
            p[2] = pt1.z();
            glTexCoord2f(i, 1);
            glVertex3dv(p);
            pt1 = pt1 - dir;
            p[0] = pt1.x();
            p[1] = pt1.y();
            p[2] = pt1.z();
            glTexCoord2f(i, 0);
            glVertex3dv(p);
        }
    glEnd();
    texture.release();
    glDisable(GL_TEXTURE_2D);
    glEndList();

    for( int i = 0; i < 2; ++i ){
        rail_list[i] = glGenLists(1);
        glNewList(rail_list[i], GL_COMPILE);
        glColor3f(169.0f/255.0f, 169.0f/255.0f, 169.0f/255.0f);
        glBegin(GL_QUAD_STRIP);
        {
            for( const auto& pt : rails[i] ){
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        }
        glEnd();
        glEndList();
    }

    // Set up the train. At this point a cube is drawn. NOTE: The
    // x-axis will be aligned to point along the track. The origin of the
    // train is assumed to be at the bottom of the train.

    train.Initialize();

    initialized = true;

    if( _view != nullptr )
        attachView(_view);
    return true;
}


// Draw
void
Track::Draw(void)
{
    double   posn[3];
    double   tangent[3];
    double  angle;

    if ( ! initialized )
        return;

    glPushMatrix();

    // Draw the track
    glDisable(GL_CULL_FACE);
    glCallList(track_list);

    // Draw rails
    glPushMatrix();
    glTranslatef(0,0,0.125);
    glCallList(rail_list[0]);
    glCallList(rail_list[1]);
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    glPushMatrix();

    // Figure out where the train is
    track->Evaluate_Point(posn_on_track, posn);

    // Translate the train to the point
    glTranslated(posn[0], posn[1], posn[2]);

    // ...and what it's orientation is
    track->Evaluate_Derivative(posn_on_track, tangent);
    Normalize_3(tangent);

    // Rotate it to point along the track, but stay horizontal
    angle = atan2(tangent[1], tangent[0]) * 180.0 / M_PI;
    glRotatef((float)angle, 0.0f, 0.0f, 1.0f);

    // Another rotation to get the tilt right.
    angle = asin(-tangent[2]) * 180.0 / M_PI;
    glRotatef((float)angle, 0.0f, 1.0f, 0.0f);

    // Draw the train
    train.Draw();

    glPopMatrix();
    glPopMatrix();

}


void
Track::Update(double dt)
{
    double   point[3];
    double   deriv[3];
    double  length;
    double  parametric_speed;

    if ( ! initialized )
        return;

    // First we move the train along the track with its current speed.

    // Get the derivative at the current location on the track.
    track->Evaluate_Derivative(posn_on_track, deriv);

    // Get its length.
    length = sqrt(deriv[0]*deriv[0] + deriv[1]*deriv[1] + deriv[2]*deriv[2]);
    if ( length == 0.0 )
        return;

    // The parametric speed is the world train speed divided by the length
    // of the tangent vector.
    parametric_speed = speed / length;

    // Now just evaluate dist = speed * time, for the parameter.
    posn_on_track += (float)(parametric_speed * dt);

    // If we've just gone around the track, reset back to the start.
    if ( posn_on_track > track->N() )
        posn_on_track -= track->N();

    // As the second step, we use conservation of energy to set the speed
    // for the next time.
    // The total energy = z * gravity + 1/2 speed * speed, assuming unit mass
    track->Evaluate_Point(posn_on_track, point);
    if ( TRAIN_ENERGY - 9.81 * point[2] < 0.0 )
        speed = 0.0;
    else
        speed = (float)sqrt(2.0 * ( TRAIN_ENERGY - 9.81 * point[2] ));

    updateView( parametric_speed*dt*50 );
}

void Track::updateView(double dist ){
    if( _view == nullptr) return;

    double pnt[3];
    track->Evaluate_Point(posn_on_track, pnt);
    QVector3D point;
    point.setX(pnt[0]);
    point.setY(pnt[1]);
    point.setZ(pnt[2]+1);
    _view->setEye(point);
    track->Evaluate_Point(posn_on_track+dist > track->N() ? posn_on_track+dist - track->N() : posn_on_track+dist, pnt);
    point.setX(pnt[0]);
    point.setY(pnt[1]);
    point.setZ(pnt[2]-1);
    _view->setLookAt(point);
    _view->moveEyeBy((_view->eye() - _view->lookAt()).normalized()*(5.0));
}

void Track::attachView(ViewPort *view){
     _view = view;
     // Now setup the initial points

     if( initialized && _view != nullptr ){
         double posn[3];
         track->Evaluate_Point(posn_on_track, posn);

         _view->x() = posn[0];
         _view->y() = posn[1];
         _view->dist() = posn[2];
     }
}
