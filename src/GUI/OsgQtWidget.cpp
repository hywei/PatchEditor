#include "OsgQtWidget.h"

OsgQtViewer::OsgQtViewer(QWidget* parent, const char* name, const QGLWidget* shareWidget, WindowFlags f) : ViewerQT(parent, name, shareWidget, f)
{
    m_in_trackball_mode = true;
}

void OsgQtViewer::initializeGL()
{
    AdapterWidget::initializeGL();
}

void OsgQtViewer::resizeGL(int width, int height)
{
    AdapterWidget::resizeGL(width, height);
}

void OsgQtViewer::paintGL()
{
    ViewerQT::paintGL();
}

void OsgQtViewer::keyPressEvent(QKeyEvent* event)
{
    AdapterWidget::keyPressEvent(event);
}

void OsgQtViewer::keyReleaseEvent(QKeyEvent* event)
{
    AdapterWidget::keyReleaseEvent(event);
}

void OsgQtViewer::mousePressEvent(QMouseEvent* event)
{
    if(m_in_trackball_mode){
        AdapterWidget::mousePressEvent(event);
    }else{
        
    }
}

void OsgQtViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_in_trackball_mode){
        AdapterWidget::mouseReleaseEvent(event);
    }else{
        
    }
}

void OsgQtViewer::mouseMoveEvent(QMouseEvent* event)
{
    if(m_in_trackball_mode){
        AdapterWidget::mouseMoveEvent(event);
    }else{
        
    }
}



