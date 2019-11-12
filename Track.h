/*
 * Track.h: Header file for a class that draws the train and its track.
 *
 * (c) 2001: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef TRAINTRACK_H
#define TRAINTRACK_H

#include "GLDrawable.h"
#include "CubicBspline.h"

class Track : public GLDrawable {
  private:
    GLubyte 	    track_list;	    // The display list for the track.
    GLubyte 	    train_list;	    // The display list for the train.
    bool    	    initialized{false};    // Whether or not we have been initialized.
    CubicBspline    *track;	    // The spline that defines the track.
    float	    posn_on_track{0.0f};  // The train's parametric position on the
				    // track.
    float	    speed{0.0f};	    // The train's speed, in world coordinates

    static const int	TRACK_NUM_CONTROLS;	// Constants about the track.
    static const float 	TRACK_CONTROLS[][3];
    static const float 	TRAIN_ENERGY;

  public:
    // Constructor
    Track(void):GLDrawable{} { }

    // Destructor
    ~Track(void) override;

    bool    Initialize() override;	// Gets everything set up for drawing.
    void    Update(double) override;	// Updates the location of the train
    void    Draw() override;		// Draws everything.
    bool    isAnimated() override {return true;}
};


#endif

