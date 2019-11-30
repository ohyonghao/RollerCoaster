#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QObject>
#include <QGLWidget>
#include <QTimer>

#include <vector>
#include <memory>

#include "GLDrawable.h"
#include "ViewPort.h"

class GLWidget : public QGLWidget//, protected QOpenGLFunctions
{
    Q_OBJECT
    const double FOV_X {45.0};

    public:
        explicit GLWidget(QWidget* parent = nullptr);
        virtual ~GLWidget() override;
        QSize sizeHint() const override;

    protected:
        void initializeGL() override;
        void resizeGL(int width, int height) override;
        void paintGL() override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
    private:
        void createForest();

        std::vector<std::shared_ptr<GLDrawable> > drawables;
        std::vector<std::shared_ptr<GLDrawable> > animated;

        std::vector<ViewPort> views;
        ViewPort *currentView;

        QTimer coasterTimer;

        // Plane Size
        uint32_t size_x{50};
        uint32_t size_y{50};

        // Mouse
        double  phi_down;   // The view inclination angle when the mouse
                            // button was pushed
        double  theta_down; // The view azimuthal angle when the mouse
                            // button was pushed
        double	dist_down;  // The distance when the mouse button is pushed.
        double	x_at_down;  // The x-coord to look at when the mouse went down.
        double	y_at_down;  // The y-coord to look at when the mouse went down.
        QPoint downPos; // location of last click

public slots:

        void updateCoaster();
};

#endif // GL_WIDGET_H
