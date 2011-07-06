#ifndef EVENT_HANDLER_TRACKBALL_H
#define EVENT_HANDLER_TRACKBALL_H

#include "EventHandlerInterface.h"
#include <set>

class EH_Trackball : public EventHandlerInterface
{
public:
	EH_Trackball();
	~EH_Trackball();

public:
	void Init(ViewerQT* _qtViewer);
	bool OnLButtonDown(unsigned int nFlags, int point_x, int point_y);
	bool OnMouseMove(unsigned int nFlags, int point_x, int point_y);
	bool OnMouseWheel(unsigned int nFlags, short zDelta, int point_x, int point_y);
	handler_key HandlerKey() {return "TrackballHandler";} 
	bool IsMouseNeedHandle(MouseMode mouse_mode);


private:
	 std::set<MouseMode> m_ValidMouseMode;  
};
#endif
