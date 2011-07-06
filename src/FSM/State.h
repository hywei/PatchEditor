#ifndef FSM_STATE_H_
#define FSM_STATE_H_

#include "FSM.h"
#include "EventInterface.h"
#iinclude <boost/any.hpp>

namespace FSM{

    class Trackball_ST :: public State<EventInterface, StateKey, boost::any>{
    public:
        Trackball_ST();

        virtual void enter(StateKey& next_state, boost::any& param);
        virtual void leave();

        virtual void OnReshape(int width, int height);
        virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
        virtual void OnMouse(int button, int state, int x, int y);
        virtual void OnMotion(int x, int y);

        virtual StateKey& key(void) const;
        
    private:
        

    };

}
#endif
