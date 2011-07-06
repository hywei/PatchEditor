#include "EH_Trackball.h"
#include "../MainWindow/ViewerQT.h"

#include <iostream>
 
//////////////////////////////////////////////////////////////////////
// EH_Trackball Construction/Destruction
//////////////////////////////////////////////////////////////////////
EH_Trackball::EH_Trackball()
{
	this->m_MouseMode = MOUSE_MODE_UNDEFINED;
	this->m_KeyMode = KEY_MODE_UNDEFINED;

	//
}
EH_Trackball::~EH_Trackball(){}

void EH_Trackball::Init(ViewerQT* _qt_viewer)
{
    qt_viewer = _qt_viewer;
}

bool EH_Trackball::OnLButtonDown(unsigned int nFlags, int point_x, int point_y)
{
	switch(m_MouseMode){
    default:
        break;
	}
	return true;
}

bool EH_Trackball::OnMouseMove(unsigned int nFlags, int point_x, int point_y)
{
	// LButton is pressed (dragging)
	if((nFlags&Qt::LeftButton) != Qt::LeftButton) return false;

    switch(m_MouseMode){
        
    default:
        break;
    }

	return true;
}
bool EH_Trackball::OnMouseWheel(unsigned int nFlags, short zDelta, int point_x, int point_y)
{
    return true;
}
bool EH_Trackball::IsMouseNeedHandle(MouseMode mouse_mode)
{
	this->m_MouseMode = mouse_mode;
	return m_ValidMouseMode.find(mouse_mode) != m_ValidMouseMode.end();
}
