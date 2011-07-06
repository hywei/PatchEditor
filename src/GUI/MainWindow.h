#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui/QMainWindow>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <osg/Geometry>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <boost/shared_ptr.hpp>

class QActionGroup;
class QAction;
class QMenu;
class QTextEdit;

class ViewerQT;
class PatchEditorControl;

namespace PolyPatch{
    class PolyPatch;
}

typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent=0);
	~MainWindow();
    
    const TriMesh& GetMesh() const { return mesh; }
    boost::shared_ptr<PolyPatch::PolyPatch> GetPolyPatch() { return p_poly_patch; }
    
private:
	void createActions();
	void createMenus();
	void createToolBars();
    void createStatusBar();

    void CreateOsgMeshNode(TriMesh& mesh);
    osg::ref_ptr<osg::Group> CreateOsgSolidGroup(osg::ref_ptr<osg::Geode>);
    osg::ref_ptr<osg::Group> CreateOsgPointGroup(osg::ref_ptr<osg::Geode>);
    osg::ref_ptr<osg::Group> CreateOsgWireGroup(osg::ref_ptr<osg::Geode>);

    void CreateOsgPatchNode();
    osg::ref_ptr<osg::Geode> CreateOsgPNodeGeode();
    osg::ref_ptr<osg::Geode> CreateOsgPEdgeGeode();

 private slots:
	void openModel();
	void saveModel();
	void about();

    void solidDisplay();
    void wireDisplay();
    void pointDisplay();
    void flatwireDisplay();
    //void smooth_solidDisplay();

 public slots:
    void loadPatchFile();
    void savePatchFile();
    void SetPNodeSwitch(bool);
    void SetPEdgeSwitch(bool);
    void SetPatchSwitch(bool);
    
    
private:    
	QAction* openModelAct, *saveModelAct, *exitAct, *aboutAct;
	QAction* toolBarAct, *stateBarAct;
	// mouse actions
	QAction* solidDisplayAct, *wireDisplayAct, *pointDisplayAct, *smooth_solidDisplayAct, *flatwireDisplayAct;

 private:
    osg::ref_ptr<osg::Group> osg_root;
    osg::ref_ptr<osg::Switch> osg_mesh_switch;
    osg::ref_ptr<osg::Switch> osg_patch_switch;
    osg::ref_ptr<osg::Group> osg_solid_gp;
    osg::ref_ptr<osg::Group> osg_wire_gp;
    
    TriMesh mesh;
    boost::shared_ptr<PolyPatch::PolyPatch> p_poly_patch;
    
public:
    ViewerQT* viewer_window;
    PatchEditorControl* editor_control;
};

#endif
