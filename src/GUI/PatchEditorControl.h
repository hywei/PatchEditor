#ifndef PATCHEdITORCONTROL_H_
#define PATCHEdITORCONTROL_H_

#include <QWidget>

class MainWindow;
class QStateMachine;
class QTransiton;
class QGroupBox;
class QPushButton;
class QLabel;

class PatchEditorControl : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(QString status READ status WRITE setStatus)

 public:
    PatchEditorControl(MainWindow*, QWidget* parent=0);

    QString status() const;
    void setStatus(const QString& status);

 private:
    void CreateMainLayout();
    QGroupBox* CreatePNodeGroup(QWidget* parent = NULL);
    QGroupBox* CreatePEdgeGroup(QWidget* parent = NULL);
    QGroupBox* CreateVisualizeGroup(QWidget* parent = NULL);
    QGroupBox* CreateIOGroup(QWidget* parent = NULL);
    
    //! create the state machine
    void BuildMachine();
    
 public:
    MainWindow* main_win;

    QStateMachine* machine;
    QString cur_state;

    QPushButton *add_pnode_pb, *sel_pnode_pb, *del_pnode_pb;

    QLabel* my_label;
};

inline QString PatchEditorControl::status() const { return cur_state; }
inline void PatchEditorControl::setStatus(const QString& status) { cur_state = status; }
#endif
