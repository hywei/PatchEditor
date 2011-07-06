
#include "PatchEditorControl.h"
#include <QGroupBox>
#include <QPushButton>
#include <QStateMachine>
#include <QLayout>
#include <QEventTransition>
#include <QLabel>
#include <QCheckBox>
#include <iostream>
#include "MainWindow.h"

PatchEditorControl::PatchEditorControl(MainWindow* _main_win, QWidget* parent)
    : main_win(_main_win)
{
    std::cout <<"Create Patch Editor Control" << std::endl;
    QGroupBox* pnode_editor_gp = CreatePNodeGroup(this);
    QGroupBox* pedge_editor_gp = CreatePEdgeGroup(this);
    QGroupBox* visual_gp = CreateVisualizeGroup(this);
    QGroupBox* io_gp = CreateIOGroup(this);

    my_label = new QLabel(this);
    my_label->setText(tr("Status"));
    
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setMargin(3);
    main_layout->setSpacing(30);
    main_layout->addWidget(pnode_editor_gp);
    main_layout->addWidget(pedge_editor_gp);
    main_layout->addWidget(visual_gp);
    main_layout->addWidget(my_label);
    main_layout->addWidget(io_gp);
    main_layout->addStretch(1);

    BuildMachine();
}

QGroupBox* PatchEditorControl::CreatePNodeGroup(QWidget* parent)
{
    QGroupBox* pnode_editor_gp = new QGroupBox(this);
    pnode_editor_gp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    pnode_editor_gp->setTitle(tr("Patch Corner Editor"));

    add_pnode_pb = new QPushButton(pnode_editor_gp);
    add_pnode_pb->setText(tr("Add Corner"));
    sel_pnode_pb = new QPushButton(pnode_editor_gp);
    sel_pnode_pb->setText(tr("Select Corner"));
    del_pnode_pb = new QPushButton(pnode_editor_gp);
    del_pnode_pb->setText(tr("Delete Corner"));

    QVBoxLayout* pnode_editor_layout = new QVBoxLayout(pnode_editor_gp);
    pnode_editor_layout->addWidget(add_pnode_pb);
    pnode_editor_layout->addWidget(sel_pnode_pb);
    pnode_editor_layout->addWidget(del_pnode_pb);

    //TODO: connections
        
    return pnode_editor_gp;
}

QGroupBox* PatchEditorControl::CreatePEdgeGroup(QWidget* parent)
{
    QGroupBox* pedge_editor_gp = new QGroupBox(this);
    pedge_editor_gp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    pedge_editor_gp->setTitle(tr("Patch Edge Editor"));
    
    QPushButton* add_pedge_pb = new QPushButton(pedge_editor_gp);
    add_pedge_pb->setText(tr("Add Edge"));
    QPushButton* end_pedge_pb = new QPushButton(pedge_editor_gp);
    end_pedge_pb->setText(tr("End Edge"));
    QPushButton* sel_pedge_pb = new QPushButton(pedge_editor_gp);
    sel_pedge_pb->setText(tr("Select Edge"));
    QPushButton* del_pedge_pb = new QPushButton(pedge_editor_gp);
    del_pedge_pb->setText(tr("Delete Edge"));
    
    QVBoxLayout* pedge_editor_layout = new QVBoxLayout(pedge_editor_gp);
    pedge_editor_layout->addWidget(add_pedge_pb);
    pedge_editor_layout->addWidget(end_pedge_pb);
    pedge_editor_layout->addWidget(sel_pedge_pb);
    pedge_editor_layout->addWidget(del_pedge_pb);

    //TODO: connections
    return pedge_editor_gp;
}

QGroupBox* PatchEditorControl::CreateVisualizeGroup(QWidget* parent)
{
    QGroupBox* visual_gp = new QGroupBox(parent);
    visual_gp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    visual_gp->setTitle(tr("Visualization"));

    QCheckBox* pnode_cb = new QCheckBox(visual_gp);
    QCheckBox* pedge_cb = new QCheckBox(visual_gp);
    QCheckBox* patch_cb = new QCheckBox(visual_gp);

    pnode_cb->setText(tr("Patch Corner"));
    pedge_cb->setText(tr("Patch Edge"));
    patch_cb->setText(tr("Patch Face"));

    pnode_cb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pedge_cb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    patch_cb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout* visual_layout = new QVBoxLayout(visual_gp);
    visual_layout->addWidget(pnode_cb);
    visual_layout->addWidget(pedge_cb);
    visual_layout->addWidget(patch_cb);

    connect(pnode_cb, SIGNAL(toggled(bool)), main_win, SLOT(SetPNodeSwitch(bool)));
    connect(pedge_cb, SIGNAL(toggled(bool)), main_win, SLOT(SetPEdgeSwitch(bool)));
    connect(patch_cb, SIGNAL(toggled(bool)), main_win, SLOT(SetPatchSwitch(bool)));
    return visual_gp;
}

QGroupBox* PatchEditorControl::CreateIOGroup(QWidget* parent)
{
    QGroupBox* io_gp = new QGroupBox(parent);
    io_gp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    io_gp->setTitle(tr("IO"));

    QPushButton* load_patch_file = new QPushButton(io_gp);
    load_patch_file->setText(tr("Load Patch File"));
    QPushButton* save_patch_file = new QPushButton(io_gp);
    save_patch_file->setText(tr("Save Patch File"));

    QVBoxLayout* io_layout = new QVBoxLayout(io_gp);
    io_layout->addWidget(load_patch_file);
    io_layout->addWidget(save_patch_file);

    connect(load_patch_file, SIGNAL(clicked()), main_win, SLOT(loadPatchFile()));
    connect(save_patch_file, SIGNAL(clicked()), main_win, SLOT(savePatchFile()));
    return io_gp;
}

void PatchEditorControl::BuildMachine()
{
    machine = new QStateMachine;
    QState *arcball_st = new QState(machine);
    arcball_st->assignProperty(this, "status", "Arcball State");
    
    QState *addpnode_st = new QState(machine);
    addpnode_st->assignProperty(this, "status", "Add Corner State");
    addpnode_st->assignProperty(my_label, "text", "In addpnode_state");

    QState *selpnode_st = new QState(machine);
    selpnode_st->assignProperty(this, "status", "Select Corner State");

    QState *delpnode_st = new QState(machine);
    delpnode_st->assignProperty(this, "status", "Delete Corner State");

    arcball_st->addTransition(add_pnode_pb, SIGNAL(clicked()), addpnode_st);
    arcball_st->addTransition(sel_pnode_pb, SIGNAL(clicked()), selpnode_st);
    
    QEventTransition* addpnode_et = new QEventTransition(add_pnode_pb, QEvent::Enter);
    addpnode_et->setTargetState(addpnode_st);
    arcball_st->addTransition(addpnode_et);

    QEventTransition* selpnode_et = new QEventTransition(sel_pnode_pb, QEvent::Enter);
    selpnode_et->setTargetState(selpnode_st);
    arcball_st->addTransition(selpnode_et);

    QEventTransition* delpnode_et = new QEventTransition(del_pnode_pb, QEvent::Enter);
    delpnode_et->setTargetState(delpnode_st);
    arcball_st->addTransition(delpnode_et);

    machine->setInitialState(arcball_st);
    machine->start();
}
    
