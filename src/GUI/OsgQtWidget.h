#include "AdapterWidget.h"
/*
  \class OsgQtViewer 
 */

class OsgQtViewer : public ViewerQT
{
 public:
    OsgQtViewer(QWidget* parent=0, const char* name =0, const QGLWidget* shareWidget = 0, WindowFlags f = 0);

 protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

 private:
    
    
 public:
    bool m_in_trackball_mode;

    
};
