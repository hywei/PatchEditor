#ifndef EH_EDITOR_H_
#define EH_EDITOR_H_

#include "EventHandlerInterface.h"
#include <set>

class EH_EDITOR : public EventHandlerInterface
{
 public:
    EH_Param();
    ~EH_Param();

 public:
    void Init(ViewerQT* _qt_viewer);
    bool OnLButtonDown(unsigned int nFlags, int point_x, int point_y);
    bool OnMouseMove(unsigned int nFlags, int point_x, int point_y);

    handler_key HandleKey() { return "PatchEditorHandler"; }
    bool IsMouseNeedHandle(MouseMode mouse_mode);

 private:
    std::set<MouseMode> m_ValidMouseMode;
};

#endif
