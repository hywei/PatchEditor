#ifndef EDITORSTATE_H_
#define EDITORSTATE_H_

#include "FSM.h"
#include "EventInterface.h"
#include <boost/any.hpp>

/*
  \class AddPNode_ST : Add a new corner state
 */
class AddPNode_ST : public State<EventInterface, StateKey, boost::any>
{
 public:
    AddPNode_ST();

    virtual void Enter(StateKey& next_state, boost::any& param);

    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);

 private:

};

/*
  \class DelPNode_ST : Delete a existing corner state
 */
class DelPNode_ST : public State<EventInterface, StateKey, boost::any>
{
 public:
    DelPNode_ST();

    virtual void Enter(StateKey& next_state, boost::any& param);

    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);
};

/*
  \class SelPNode_ST : Select a existing corner state
 */
class SelPNode_ST : public State<EventInterface, StateKey, boost::any>
{
 public:
    SelPNode_ST();

    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);
};

/*
  \class AddPEdge_ST : Add a new patch edge state
 */
class AddPEdge_ST
{
 public:
    AddPEdge_ST();
    
    virtual void Enter(StateKey& next_state, boost::any& param);
    
    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);
};

/*
  \class EndPEdge_ST : End a new patch edge state and will form a new edge
 */
class EndPEdge_ST
{
 public:
    EndPEdge_ST();

    virtual void Enter(StateKey& next_state, boost::any& param);
    
    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);
};

/*
  \class SelPEdge_ST : Select a patch edge state
 */
class SelPEdge_ST
{
 public:
    SelPEdge_ST();

    virtual void Enter(StateKey& next_state, boost::any& param);
    
    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);
};

/*
  \class DelPEdge_ST : Delete a existing patch edge state
 */
class DelPEdge_ST
{
 public:
    DelPEdge_ST();

    virtual void Enter(StateKey& next_state, boost::any& param);
    
    virtual void OnReshape(int width, int height);
    virtual void OnKeyboard(unsigned char key, int x=0, int y=0);
    virtual void OnMouse(int button, int state, int x, int y);
    virtual void OnMotion(int x, int y);
    
}
#endif
