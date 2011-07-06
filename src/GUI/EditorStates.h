#ifndef EDITORSTATES_H_
#define EDITORSTATES_H_

#include <QState>
class ViewerQT;

class Arcball_ST : public QState
{
    Q_OBJECT
        
 public:
    Arcball_ST(ViewerQT*);

    void OnEntry(QEvent* event);
    void OnExit(QEvent* event);
    
 private:
    ViewerQT* viewer_win;
    
};

class AddPNode_ST : public QState
{
    Q_OBJECT
        
 public:
    AddPNode_ST(ViewerQT*);

    void OnEntry(QEvent* event);
    void OnExit(QEvent* event);
 private:
    ViewerQT* viewer_win;
};

class SelPNode_ST : public QState
{
    Q_OBJECT
 public:
    SelPNode_ST(ViewerQT*);

    void OnEntry(QEvent* event);
    void OnExit(QEvent* event);
 private:
    ViewerQT* viewer_win;
};


#endif
