#include "EH_Editor.h"

EH_Editor::EH_Editor()
{
    this->m_MouseMode = MOUSE_MODE_UNDEFINED;
    this->m_KeyMode = KEY_MODE_UNDEFINED;

    m_ValidMouseMode.insert(MOUSE_MODE_SELECT_VERTEX);
}

EH_Editor::~EH_Editor() {}

void EH_Editor::Init(ViewerQT* _qt_viewer)
{
    qt_viewer = _qt_viewer;
}

bool EH_Editor::OnLButtonDown(unsigned int nFlags, int point_x, int point_y)
{
    switch(m_MouseMode){
    case MOUSE_MODE_SELECT_VERTEX:

        break;
    default:
        break;
    }
    return true;
}

bool EH_Editor::OnMouseMove(unsigned int nFlags, int point_x, int point_y)
{
    switch(m_MouseMode){
    case MOUSE_MODE_SELECT_VERTEX:
        break;
    default:
        break;
    }

    return true;
}

bool EH_Editor::IsMouseNeedHandle(MouseMode mouse_mode)
{
    this->m_MouseMode = mouse_mode;
    return m_ValidMouseMode.find(mouse_mode) != m_ValidMouseMode.end();
}
