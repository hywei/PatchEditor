#ifndef EVENTINTERFACE_H_
#define EVENTINTERFACE_H_

#include <string>

namespace FSM{

    typedef std::string StateKey;

class EventInterface
{
 public:
    EventInterface( const StateKey& sk = "") : key_(sk){}
    virtual ~EventInterface(){}

    virtual void OnReshape(int width, int height){}
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0) {}
    virtual void OnMouse(int button, int state, int x, int y) {}
    virtual void OnMotion(int x, int y) {}
    
    StateKey key_;
};

}
#endif
